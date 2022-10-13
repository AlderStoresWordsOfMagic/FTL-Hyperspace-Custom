## Building on VSCode devcontainer 

Open the directory using VSCode w/ Remote Container extension enabled and active.

Use `CMake: Select a Kit` for platform target, and `CMake: Select a Variant` for Debug/Release configuration.
Note that other configurations like `RelWithDebInfo` are not tested.

## Manually building without VSCode

Requires Docker.

1. `docker build -t hyperspace .devcontainer`

2. `docker run -it --rm -v $PWD:/ftl hyperspace`
(`$PWD` may vary by your shell, e.g. if it's cmd then use `%cd%` instead)

3. `cd /ftl`

4. Configure with CMake. The exact command should depend on what build you're targeting. For example, Linux Release build for FTL 1.6.13 is:
```
cmake -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DVCPKG_HOST_TRIPLET=amd64-linux-ftl \
    -DVCPKG_TARGET_TRIPLET=amd64-linux-ftl \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/vcpkg/scripts/toolchains/amd64-linux-ftl.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DSTEAM_1_6_13_BUILD=ON \
    -S . -B build -G Ninja
```
* Change `amd64-linux-ftl` to `x86-windows-ftl` and `amd64-linux-ftl.cmake` to `x86-windows-ftl.cmake` to configure a Windows build.
* (Linux only) `-DSTEAM_1_6_13_BUILD=ON` if the target binary is FTL 1.6.13. Use `-DSTEAM_1_6_13_BUILD=OFF` instead to target FTL 1.6.12. On Windows this option does nothing and can be omitted.
* `-DCMAKE_BUILD_TYPE=Release` controls if it's a `Debug` or a `Release` build. Theoretically you may specify `RelWithDebInfo` or `MinSizeRel` as in standard CMake, but these options are not tested.
* This command line can be inferred from the `.vscode/cmake-kits.json` file.

5. `ninja -C build`

### Rebuilding steps

- On changing source code, just redo the step 5 (`ninja`).
- On changing CMakeLists.txt (adding source files or macro definitions, etc.), redo from step 4 (`cmake`).
- On changing the build target (like changing to linux build or debug build), delete build/ directory (for clean rebuild) and then redo from step 4 (`cmake`).
- On changing the compiler toolchain files in the .devcontainer folder, delete build/ directory (for clean rebuild) and then redo from step 1 (`docker build`).
