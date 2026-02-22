cmake_minimum_required(VERSION 3.20)

project(airwindows_max_external C CXX)

if(APPLE AND (NOT DEFINED CMAKE_OSX_ARCHITECTURES OR CMAKE_OSX_ARCHITECTURES STREQUAL ""))
    # Build universal binaries by default so externals load in both Intel and Apple Silicon Max.
    set(
        CMAKE_OSX_ARCHITECTURES
        "arm64;x86_64"
        CACHE STRING "Build architectures for macOS"
        FORCE
    )
endif()

set(
    MAX_SDK_BASE_DIR
    "$ENV{MAX_SDK_BASE_DIR}"
    CACHE PATH "Path to max-sdk-base (contains script/max-pretarget.cmake)"
)

if(NOT EXISTS "${MAX_SDK_BASE_DIR}/script/max-pretarget.cmake")
    message(FATAL_ERROR "MAX_SDK_BASE_DIR is invalid. Set it to your max-sdk-base path.")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/targets.cmake")
    message(FATAL_ERROR "Generated targets are missing. Run: python3 scripts/generate_console7_family.py")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/src/generated/targets.cmake")
