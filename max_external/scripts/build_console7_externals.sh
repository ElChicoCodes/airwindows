#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MAX_EXTERNAL_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
REPO_ROOT="$(cd "${MAX_EXTERNAL_DIR}/.." && pwd)"

PLATFORM="both"

print_usage() {
  cat <<'USAGE'
Usage:
  build_console7_externals.sh [--platform mac|windows|both]

Environment:
  MAX_SDK_BASE_DIR (required): path to max-sdk-base checkout
USAGE
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --platform)
      PLATFORM="${2:-}"
      shift 2
      ;;
    -h|--help)
      print_usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      print_usage
      exit 1
      ;;
  esac
done

if [[ -z "${MAX_SDK_BASE_DIR:-}" ]]; then
  echo "MAX_SDK_BASE_DIR is not set." >&2
  exit 1
fi

if [[ ! -f "${MAX_SDK_BASE_DIR}/script/max-pretarget.cmake" ]]; then
  echo "MAX_SDK_BASE_DIR does not look valid: ${MAX_SDK_BASE_DIR}" >&2
  exit 1
fi

case "${PLATFORM}" in
  mac|windows|both)
    ;;
  *)
    echo "Invalid --platform value: ${PLATFORM}" >&2
    exit 1
    ;;
esac

python3 "${MAX_EXTERNAL_DIR}/scripts/generate_console7_family.py"

build_mac() {
  local build_dir="${REPO_ROOT}/build-max"

  cmake -S "${MAX_EXTERNAL_DIR}" -B "${build_dir}" -G Ninja \
    -DMAX_SDK_BASE_DIR="${MAX_SDK_BASE_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"

  cmake --build "${build_dir}" -j

  local bin="${MAX_EXTERNAL_DIR}/externals/airfx.console7~.mxo/Contents/MacOS/airfx.console7~"
  if [[ -f "${bin}" ]]; then
    echo "[mac] $(lipo -info "${bin}")"
  fi
}

build_windows() {
  local cc cxx rc build_dir

  cc="$(command -v x86_64-w64-mingw32-gcc)"
  cxx="$(command -v x86_64-w64-mingw32-g++)"
  rc="$(command -v x86_64-w64-mingw32-windres)"
  build_dir="${REPO_ROOT}/build-max-win"

  cmake -S "${MAX_EXTERNAL_DIR}" -B "${build_dir}" -G Ninja \
    -DMAX_SDK_BASE_DIR="${MAX_SDK_BASE_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_C_COMPILER="${cc}" \
    -DCMAKE_CXX_COMPILER="${cxx}" \
    -DCMAKE_RC_COMPILER="${rc}" \
    -DCMAKE_CROSSCOMPILING=ON

  cmake --build "${build_dir}" -j
}

if [[ "${PLATFORM}" == "mac" || "${PLATFORM}" == "both" ]]; then
  build_mac
fi

if [[ "${PLATFORM}" == "windows" || "${PLATFORM}" == "both" ]]; then
  build_windows
fi

echo "Built artifacts:"
find "${MAX_EXTERNAL_DIR}/externals" -maxdepth 1 \( -name '*.mxo' -o -name '*.mxe64' \) | sort
