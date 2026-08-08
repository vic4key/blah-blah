# 📘 SETUP & BUILD LLAMA.CPP GPU ACCELERATION (OPENCL) FOR ANDROID (TERMUX)

This guide provides step-by-step instructions for environment configuration, compiling from source, and optimizing local inference performance using the **OpenCL** library for **Snapdragon processors (Adreno GPU)**.

---

## 🏗️ PART 1: ENVIRONMENT PREPARATION ON TERMUX

### 1. Install the Correct Termux Build
* Do not install Termux from the Google Play Store (that version is outdated and suffers from broken GPG repository keys).
* Download the latest Termux build directly from **F-Droid** or the official **Termux GitHub Repository**.

### 2. Update System Packages and Enable the Termux User Repository (TUR)
Open Termux and run the following command block to refresh core system headers and pull the community package repository required for specialized OpenCL dependencies:
```bash
pkg update && pkg upgrade -y
pkg install tur-repo -y
pkg update -y
```

### 3. Install Essential Build Packages
Install the required compilers, build generation tools, OpenCL headers, and command-line utilities:
```bash
pkg install opencl-headers opencl-clhpp ocl-icd git cmake ninja clang python wget nano -y
```

---

## 🔌 PART 2: PRE-BUILD SETUP (LINKING SYSTEM GRAPHICS DRIVER)

Because Termux operates inside an isolated system sandbox, you must explicitly expose the vendor-level Android OpenCL shared graphics library to the environment paths.

Run this block to initialize the vendor path folder and bind the physical Adreno driver file:
```bash
mkdir -p \$PREFIX/etc/OpenCL/vendors
echo "/vendor/lib64/libOpenCL_adreno.so" > \$PREFIX/etc/OpenCL/vendors/adreno.icd
```

---

## 🛠️ PART 3: COMPILING LLAMA.CPP FROM SOURCE

### 1. Clone the Codebase
```bash
git clone https://github.com/ggml-org/llama.cpp.git
cd llama.cpp
```

### 2. Configure CMake with Optimization Fixes (Bypassing Android Clang Bugs)
To circumvent a severe compiler bug present in Android's Clang layout (which throws *Clang Frontend Crash / Exit code 139* when handling intensive math vector extensions), we disable `GGML_NATIVE` and pass down a stable compilation optimization flag set (`-O2 -fno-vectorize`), while enabling the OpenCL processing backend:
```bash
cmake -B build -G Ninja -DGGML_OPENCL=ON -DGGML_OPENCL_EMBED_KERNELS=ON -DGGML_NATIVE=OFF -DCMAKE_C_FLAGS="-O2 -fno-vectorize" -DCMAKE_CXX_FLAGS="-O2 -fno-vectorize"
```

### 3. Compile the Source Code (Strict Thread Constraint)
Enforce a single active job allocation using `-j 1`. This safely restricts the build process to a single CPU core, preventing Android's Low Memory Killer (OOM Killer) from terminating your Termux session due to system RAM depletion:
```bash
cmake --build build --config Release -j 1
```
*Once the progress counter successfully caps at `[100%]` or `[697/697]`, all hardware-optimized binary targets will be fully compiled and accessible under the `~/llama.cpp/build/bin` subdirectory path.*

---

## ⚙️ PART 4: POST-BUILD CONFIGURATION & SYSTEM PATH ALIGNMENT

To invoke your compiled binaries from any workspace location without repeating complex directory structures, we will create a global symbolic directory link and map system flags directly to your environment shell profile.

### 1. Create a Directory Symbolic Link
Link the compiled binary directory and rename the link wrapper to `llama`:
```bash
ln -sf ~/llama.cpp/build/bin \$PREFIX/bin/llama
```

### 2. Automate Environment Mapping inside `.bashrc`
Open your interactive shell profile editor:
```bash
nano ~/.bashrc
```
Navigate to the bottom lines of the file structure and append these two distinct environment entries:
```bash
export LD_LIBRARY_PATH=/vendor/lib64:\$LD_LIBRARY_PATH
export PATH=\$PREFIX/bin/llama:\$PATH
```
*(Press `Ctrl + O` -> `Enter` to commit file modifications, then press `Ctrl + X` to exit the screen layout).*

### 3. Reload Shell Environment Parameters
```bash
source ~/.bashrc
```

---

## 🚀 PART 5: MODEL ACQUISITION & EXECUTION GUIDE

### 1. Download a Sample Mobile-Optimized GGUF Model (e.g., Llama 3.2 3B)
Fetch a 4-bit quantized layout (`Q4_K_M`) engineered to run smoothly inside smartphone

### 2. Launch the Hardware-Accelerated Local Server
Thanks to the environment variables configured earlier, you can now trigger the execution engine instantly from any active working directory path:
```bash
llama-server --port 1234 -ngl 99 -t 4 -m /sdcard/Download/Qwen3-4B-Q4_K_M.gguf
llama-server --port 1234 -ngl 99 -t 4 -c 4096 --parallel 1 -fa on --cache-type-k q8_0 --cache-type-v q8_0 -b 512 -ub 512 -m /sdcard/Download/Qwen2.5-Coder-7B-Instruct-Q4_K_M.gguf
```
**Core Parameter Explanations:**
* `--port 1234`: Hosts your local Web Interface and OpenAI-compatible API endpoint on port 1234.
* `-ngl 99` (Number of GPU Layers): Forces the runtime scheduler to offload every single network layer (e.g., 28/28 layers) away from the CPU, packing them directly into your graphics processing memory pools.
* `-t 4` (Threads): Constrains CPU processing threads strictly to the count of your chip's **High-Performance Cores** (typically 3 or 4). Do not maximize threads to all available hardware cores, as this triggers thread scheduling bottlenecks on Android architectures.

### 3. Interact via the Native Browser Interface
Leave Termux processing text threads in the background, open any native browser application (Chrome or Samsung Internet) on your handset, and visit:
👉 **`http://localhost:1234`** or **`http://127.0.0.1:1234`**

---

## 📦 PART 6: COMPILING TARBALL ARCHIVES FOR DEPLOYMENT/SHARING

If you need to distribute these binaries to external Android devices equipped with compatible Snapdragon platforms without repeating the long compiling phase, package the target folder into an archive:

```bash
# Navigate to your build directory framework
cd ~/llama.cpp/build
tar -czvf llama-bin-aarch64-snapdragon-adreno-opencl.tar.gz bin

# Move the compressed archive to your phone's visible storage area
cp llama-bin-aarch64-snapdragon-adreno-opencl.tar.gz /sdcard/Download/
```

---

## ❓ TROUBLESHOOTING (COMMON ISSUES & WORKAROUNDS)

#### Issue 1: `E: Unable to locate package libopencl` appears during initial package installations.
* **Root Cause:** OpenCL packages are hosted inside Termux's community extensions, which aren't mapped by default main repository lists.
* **Resolution:** Ensure you call `pkg install tur-repo -y` first, execute an environment refresh via `pkg update -y`, and invoke the package list using `ocl-icd` as updated in this handbook.

#### Issue 2: Compilation crashes with `build.ninja: No such file or directory` or throws missing Python warnings.
* **Root Cause:** Your initial CMake layout routine aborted due to the absence of Python 3, which acts as a dependency validation middleman. As a result, the backend configuration file `build.ninja` was never generated.
* **Resolution:** Set up the required Python dependencies with `pkg install python -y`. Next, completely clear the faulty cache using `rm -rf build`, and re-trigger the primary configurations starting with the `cmake -B build...` line.

#### Issue 3: Compilation faults out with a `clang frontend command failed with exit code 139` error.
* **Root Cause:** Android's internal Clang compiler library hits a memory segmentation loop when parsing complex, nested assembly vector loop generation functions (`+dotprod+i8mm+sve+sme`) in the CPU matrix calculation files of llama.cpp.
* **Resolution:** Pass down explicit optimization overrides to the compiler environment. Append `-DGGML_NATIVE=OFF -DCMAKE_C_FLAGS="-O2 -fno-vectorize" -DCMAKE_CXX_FLAGS="-O2 -fno-vectorize"` inside your initial CMake configuration command block.

#### Issue 4: Console displays `warning: no usable GPU found` or `library /vendor/lib64/libOpenCL_adreno.so is not accessible for the namespace`.
* **Root Cause:** Android's system security layout (Linker Namespace Isolation rules enforced on Android 10+) intentionally blocks third-party app components like Termux from linking directly to low-level native `.so` library entries inside the `/vendor` partition maps.
* **Resolution:** Inject an explicit loading environment instruction. Add `export LD_LIBRARY_PATH=/vendor/lib64:$LD_LIBRARY_PATH` into your `.bashrc` profile file to safely bypass the namespace block during execution.

***
*Tested and validated natively on Snapdragon 8 Elite hardware architecture layouts.*
