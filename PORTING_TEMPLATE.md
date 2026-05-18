# Max External Porting Template (macOS + Windows)

Use this template in any project where you want to port DSP code into Max externals and build both `.mxo` (macOS) and `.mxe64` (Windows).

## 1. Goal

Port one or more DSP plugins to Max externals using an automated generator and a reproducible build pipeline.

## 2. Expected Project Layout

```text
<project-root>/
  max_external/
    CMakeLists.txt
    scripts/
      generate_externals.py
      build_externals.sh
    src/
      generated/
        targets.cmake
        <external_target>/
          CMakeLists.txt
          <external_target>.cpp
          <PluginEngine>.hpp
    externals/
```

## 3. Prerequisites

- `python3`
- `cmake`
- `ninja`
- Max SDK base checkout (`max-sdk-base`)
- For Windows cross-build from macOS:
  - `x86_64-w64-mingw32-gcc`
  - `x86_64-w64-mingw32-g++`
  - `x86_64-w64-mingw32-windres`

Set SDK env var:

```bash
export MAX_SDK_BASE_DIR=<path-to-max-sdk-base>
```

Validate SDK path:

```bash
test -f "$MAX_SDK_BASE_DIR/script/max-pretarget.cmake"
```

## 4. Source Requirements Per Plugin

Generator expects:

```text
<source-root>/<PluginName>/
  <PluginName>.h
  <PluginName>.cpp
  <PluginName>Proc.cpp
```

Expected plugin patterns:

- parameter enum entries (`kParamA`, `kParamB`, ...)
- `getParameterName()`
- `processDoubleReplacing()`

## 5. Register Plugin Mapping

In generator mapping list:

```python
PluginSpec("<PluginClass>", "<vendor.pluginname~>")
```

Examples:

```python
PluginSpec("Console7Channel", "airfx.console7~")
PluginSpec("ToTape6", "airfx.totape6~")
```

## 6. Generate Wrappers

Generate all:

```bash
cd <project-root>/max_external
python3 scripts/generate_externals.py
```

Generate one:

```bash
python3 scripts/generate_externals.py --only <PluginClass>
```

Expected outputs:

- `max_external/src/generated/targets.cmake`
- `max_external/src/generated/<external_target>/...`

## 7. Build macOS Externals (.mxo)

```bash
cd <project-root>
cmake -S max_external -B build-max -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DMAX_SDK_BASE_DIR="$MAX_SDK_BASE_DIR" \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build-max -j
```

Output:

- `max_external/externals/*.mxo`

## 8. Build Windows Externals (.mxe64) from macOS

```bash
cd <project-root>
cmake -S max_external -B build-max-win -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DMAX_SDK_BASE_DIR="$MAX_SDK_BASE_DIR" \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER="$(command -v x86_64-w64-mingw32-gcc)" \
  -DCMAKE_CXX_COMPILER="$(command -v x86_64-w64-mingw32-g++)" \
  -DCMAKE_RC_COMPILER="$(command -v x86_64-w64-mingw32-windres)" \
  -DCMAKE_CROSSCOMPILING=ON
cmake --build build-max-win -j
```

Output:

- `max_external/externals/*.mxe64`

## 9. Recommended Runtime API in Max

Expose parameters consistently as normalized `0..1`:

- `param <value>` (sets parameter 1)
- `param <index> <value>`
- `param <symbol> <value>`
- `<symbol> <value>`
- `@<symbol> <value>` attribute
- `params` prints parameter map

## 10. Verification Checklist

- Generated files exist under `max_external/src/generated/`
- mac builds succeed and produce `.mxo`
- windows builds succeed and produce `.mxe64`
- mac binary is universal:

```bash
lipo -info <path-to-external>.mxo/Contents/MacOS/<binary-name>
```

Expected: `x86_64 arm64`

- In Max, instantiate object and confirm:
  - object loads
  - `params` outputs mapping
  - attributes appear in Inspector

## 11. Install for Local Max Testing

```bash
mkdir -p "$HOME/Documents/Max 9/Library/externals"
cp -R <project-root>/max_external/externals/*.mxo "$HOME/Documents/Max 9/Library/externals/"
```

Restart Max after copy.

## 12. Important Lessons Learned

- Push auth and commit attribution are separate.
  - SSH key/remote controls push authentication.
  - `user.email` controls GitHub commit attribution.
- Use repo-local git identity for multi-account setups.
- Architecture mismatches are common on macOS; build universal binaries by default.
- If parameters are not visible in Max Inspector, attributes were not registered (`CLASS_ATTR_*`).
- Multi-parameter plugins require generator support beyond only `kParamA`.
- Max may load stale copies from other externals paths; verify loaded file path in Max Console.
- Decide early whether compiled binaries should be committed, ignored, or published via releases.

## 13. Common Troubleshooting

### “could not load due to incorrect architecture”

Rebuild mac as universal and verify with `lipo -info`.

### Plugin sounds wrong or same as another

- Confirm plugin mapping in generator
- Compare source `Proc.cpp` between plugins
- Regenerate wrappers and rebuild
- Ensure Max loads updated bundle

### Build fails with Max SDK not found

Check `MAX_SDK_BASE_DIR` and required file:

```text
$MAX_SDK_BASE_DIR/script/max-pretarget.cmake
```
