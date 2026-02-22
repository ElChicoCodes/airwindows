# Airwindows -> Max External Porting Instructions

This document describes the automated workflow from VST source code to compiled Max externals for macOS (`.mxo`) and Windows (`.mxe64`).

## 1. Prerequisites

Required tools:

- `python3`
- `cmake`
- `ninja`
- Max SDK base checkout (`max-sdk-base`)
- For Windows cross-builds on macOS:
  - `x86_64-w64-mingw32-gcc`
  - `x86_64-w64-mingw32-g++`
  - `x86_64-w64-mingw32-windres`

Environment variable (required for build):

```bash
export MAX_SDK_BASE_DIR=/Users/jokubaspreiksa/workspace/max-sdk/source/max-sdk-base
```

## 2. Source Requirements for a Plugin

The generator expects a LinuxVST source folder:

```text
plugins/LinuxVST/src/<PluginName>/
```

with files:

- `<PluginName>.h`
- `<PluginName>.cpp`
- `<PluginName>Proc.cpp`

And the plugin should follow the usual Airwindows pattern:

- `kParamA`, `kParamB`, ... enum entries in `<PluginName>.h`
- `getParameterName()` switch in `<PluginName>.cpp`
- `processDoubleReplacing()` in `<PluginName>Proc.cpp`

## 3. Register the Plugin for Auto-Porting

Edit:

- `/Users/jokubaspreiksa/Music/airwindows/max_external/scripts/generate_console7_family.py`

In `PLUGIN_SPECS`, add a new entry:

```python
PluginSpec("ToTape6", "airfx.totape6~"),
```

Format:

```python
PluginSpec("<VST class/folder>", "<max external name>"),
```

Example mapping:

- `ToTape6` -> `airfx.totape6~`

## 4. Generate Max Wrapper Code

From repo root:

```bash
cd /Users/jokubaspreiksa/Music/airwindows/max_external
python3 scripts/generate_console7_family.py
```

Generate one plugin only:

```bash
python3 scripts/generate_console7_family.py --only ToTape6
```

Generated output:

- `/Users/jokubaspreiksa/Music/airwindows/max_external/src/generated/<external_folder>/`
- `/Users/jokubaspreiksa/Music/airwindows/max_external/src/generated/targets.cmake`

## 5. Build macOS Externals (`.mxo`)

```bash
export MAX_SDK_BASE_DIR=/Users/jokubaspreiksa/workspace/max-sdk/source/max-sdk-base

cd /Users/jokubaspreiksa/Music/airwindows
cmake -S max_external -B build-max -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build-max -j
```

Artifacts:

- `/Users/jokubaspreiksa/Music/airwindows/max_external/externals/*.mxo`

## 6. Build Windows Externals (`.mxe64`) on macOS

```bash
export MAX_SDK_BASE_DIR=/Users/jokubaspreiksa/workspace/max-sdk/source/max-sdk-base

cd /Users/jokubaspreiksa/Music/airwindows
cmake -S max_external -B build-max-win -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER="$(command -v x86_64-w64-mingw32-gcc)" \
  -DCMAKE_CXX_COMPILER="$(command -v x86_64-w64-mingw32-g++)" \
  -DCMAKE_RC_COMPILER="$(command -v x86_64-w64-mingw32-windres)" \
  -DCMAKE_CROSSCOMPILING=ON
cmake --build build-max-win -j
```

Artifacts:

- `/Users/jokubaspreiksa/Music/airwindows/max_external/externals/*.mxe64`

## 7. One-Command Build (Generate + Build)

Use the automation script:

- `/Users/jokubaspreiksa/Music/airwindows/max_external/scripts/build_console7_externals.sh`

Commands:

```bash
export MAX_SDK_BASE_DIR=/Users/jokubaspreiksa/workspace/max-sdk/source/max-sdk-base
cd /Users/jokubaspreiksa/Music/airwindows/max_external
./scripts/build_console7_externals.sh --platform both
```

Options:

- `--platform mac`
- `--platform windows`
- `--platform both` (default)

## 8. Verify Architecture and Outputs

Check built files:

```bash
find /Users/jokubaspreiksa/Music/airwindows/max_external/externals -maxdepth 1 \
  \( -name '*.mxo' -o -name '*.mxe64' \) | sort
```

Verify mac universal binary:

```bash
lipo -info /Users/jokubaspreiksa/Music/airwindows/max_external/externals/airfx.console7~.mxo/Contents/MacOS/airfx.console7~
```

Expected: `x86_64 arm64`.

## 9. Use in Max

Copy `.mxo` bundles to your Max user externals folder:

```bash
mkdir -p "$HOME/Documents/Max 9/Library/externals"
cp -R /Users/jokubaspreiksa/Music/airwindows/max_external/externals/*.mxo "$HOME/Documents/Max 9/Library/externals/"
```

Restart Max, then instantiate objects (example):

- `airfx.console7~`
- `airfx.totape6~`

Parameter control methods (all normalized `0..1`):

- `param <value>`
- `param <index> <value>`
- `param <symbol> <value>`
- `<symbol> <value>`
- `@<symbol> <value>`
- `params` (prints parameter map)

## 10. Troubleshooting

### “could not load due to incorrect architecture”

Rebuild universal mac binaries and verify with `lipo -info`.


### Build fails with Max SDK error

Check `MAX_SDK_BASE_DIR` points to the folder containing:

```text
script/max-pretarget.cmake
```

## 11. Port a New Device (Checklist)

Use this exact flow when you want to port another Airwindows plugin.

1. Confirm source files exist:
`plugins/LinuxVST/src/<PluginName>/<PluginName>.h`
`plugins/LinuxVST/src/<PluginName>/<PluginName>.cpp`
`plugins/LinuxVST/src/<PluginName>/<PluginName>Proc.cpp`

2. Pick the Max external name:
- format: `airfx.<name>~`
- example: `airfx.fromtape~`

3. Add plugin mapping in:
`/Users/jokubaspreiksa/Music/airwindows/max_external/scripts/generate_console7_family.py`
- add one `PLUGIN_SPECS` line:
```python
PluginSpec("<PluginName>", "airfx.<name>~"),
```

4. Generate only that plugin first:
```bash
cd /Users/jokubaspreiksa/Music/airwindows/max_external
python3 scripts/generate_console7_family.py --only <PluginName>
```

5. Build and test it (mac + windows):
```bash
export MAX_SDK_BASE_DIR=/Users/jokubaspreiksa/workspace/max-sdk/source/max-sdk-base
cd /Users/jokubaspreiksa/Music/airwindows/max_external
./scripts/build_console7_externals.sh --platform both
```

6. Verify artifacts exist:
```bash
find /Users/jokubaspreiksa/Music/airwindows/max_external/externals -maxdepth 1 \
  \( -name 'airfx.<name>~.mxo' -o -name 'airfx.<name>~.mxe64' \)
```

7. Verify parameter mapping in Max:
- instantiate object `airfx.<name>~`
- send message: `params`
- test controls using:
  - `param 1 0.5`
  - `param <symbol> 0.5`
  - `@<symbol> 0.5`

8. Install the `.mxo` to Max externals and restart Max:
```bash
mkdir -p "$HOME/Documents/Max 9/Library/externals"
cp -R /Users/jokubaspreiksa/Music/airwindows/max_external/externals/airfx.<name>~.mxo \
  "$HOME/Documents/Max 9/Library/externals/"
```

9. If all good, regenerate all targets:
```bash
cd /Users/jokubaspreiksa/Music/airwindows/max_external
python3 scripts/generate_console7_family.py
./scripts/build_console7_externals.sh --platform both
```
