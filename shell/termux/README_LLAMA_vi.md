# 📘 CẨM NANG CÀI ĐẶT & BUILD LLAMA.CPP GPU ACCELERATION (OPENCL) TRÊN ANDROID (TERMUX)

Tài liệu hướng dẫn cấu hình môi trường, biên dịch từ nguồn (source code) và tối ưu hóa hiệu năng tối đa cho dòng chip **Snapdragon (GPU Adreno)** bằng thư viện **OpenCL**.

---

## 🏗️ PHẦN 1: CHUẨN BỊ MÔI TRƯỜNG TRÊN TERMUX

### 1. Cài đặt Termux chuẩn
* Không cài đặt Termux từ Google Play Store (bản cũ đã bị bỏ hoang và lỗi gpg).
* Tải bản Termux mới nhất từ **F-Droid** hoặc **GitHub chính thức của Termux**.

### 2. Cập nhật hệ thống và kích hoạt kho lưu trữ bổ sung (TUR)
Mở Termux và chạy lệnh sau để cập nhật các package cốt lõi và kích hoạt Repo người dùng (Termux User Repository - bắt buộc để lấy các gói OpenCL):
```bash
pkg update && pkg upgrade -y
pkg install tur-repo -y
pkg update -y
```

### 3. Cài đặt các Package bổ sung thiết yếu
Cài đặt trình biên dịch, công cụ build, thư viện OpenCL và các tiện ích dòng lệnh:
```bash
pkg install opencl-headers opencl-clhpp ocl-icd git cmake ninja clang python wget nano -y
```

---

## 🔌 PHẦN 2: SETUP TRƯỚC KHI BUILD (LIÊN KẾT DRIVER GPU)

Do Termux chạy trong môi trường sandbox cô lập, ta cần trỏ ứng dụng trực tiếp tới file driver đồ họa gốc của hệ điều hành Android nằm ở phân vùng `vendor`.

Chạy lệnh tạo thư mục cấu hình và ánh xạ driver Adreno:
```bash
mkdir -p \$PREFIX/etc/OpenCL/vendors
echo "/vendor/lib64/libOpenCL_adreno.so" > \$PREFIX/etc/OpenCL/vendors/adreno.icd
```

---

## 🛠️ PHẦN 3: BIÊN DỊCH LLAMA.CPP TỪ NGUỒN (FROM SOURCE)

### 1. Clone Source Code
```bash
git clone https://github.com
cd llama.cpp
```

### 2. Cấu hình CMake với cờ tối ưu (Fix lỗi Compiler Clang Android)
Để tránh lỗi compiler của Android (Clang Frontend Crash / Exit code 139) khi xử lý các tập lệnh vector CPU phức tạp, ta sẽ tắt `GGML_NATIVE` và cấu hình cờ tối ưu nhẹ nhàng `-O2 -fno-vectorize`, đồng thời bật toàn bộ nhân OpenCL cho GPU:
```bash
cmake -B build -G Ninja -DGGML_OPENCL=ON -DGGML_OPENCL_EMBED_KERNELS=ON -DGGML_NATIVE=OFF -DCMAKE_C_FLAGS="-O2 -fno-vectorize" -DCMAKE_CXX_FLAGS="-O2 -fno-vectorize"
```

### 3. Tiến hành Biên dịch (Giới hạn Luồng tránh tràn RAM)
Sử dụng cờ `-j 1` để ép hệ thống compile bằng đúng 1 luồng xử lý. Điều này giúp ngăn chặn trình quản lý bộ nhớ của Android (OOM Killer) giết chết tiến trình Termux do cạn kiệt RAM khi build bằng nhiều core:
```bash
cmake --build build --config Release -j 1
```
*Sau khi thanh tiến độ chạy đạt `[100%]` hoặc `[697/697]`, các file thực thi nằm trong thư mục `~/llama.cpp/build/bin` đã sẵn sàng sử dụng.*

---

## ⚙️ PHẦN 4: SETUP SAU KHI BUILD & TỐI ƯU HÓA HỆ THỐNG (PATH)

Để thuận tiện gọi lệnh từ bất kỳ thư mục nào mà không cần gõ lại đường dẫn driver rườm rà, ta sẽ tạo liên kết biểu tượng (Symbolic Link) kết hợp nạp sẵn biến môi trường vào `.bashrc`.

### 1. Tạo Symbolic Link cho toàn bộ thư mục bin
Đặt tên thư mục liên kết viết ngắn gọn là `llama`:
```bash
ln -sf ~/llama.cpp/build/bin \$PREFIX/bin/llama
```

### 2. Cấu hình tự động nạp Driver và PATH vào `.bashrc`
Mở file cấu hình khởi động:
```bash
nano ~/.bashrc
```
Di chuyển xuống cuối file và dán chính xác 2 dòng sau:
```bash
export LD_LIBRARY_PATH=/vendor/lib64:\$LD_LIBRARY_PATH
export PATH=PREFIX/bin/llama:PATH
```
*(Nhấn `Ctrl + O` -> `Enter` để lưu, `Ctrl + X` để thoát).*

### 3. Kích hoạt cấu hình ngay lập tức
```bash
source ~/.bashrc
```

---

## 🚀 PHẦN 5: HƯỚNG DẪN SỬ DỤNG VÀ CHẠY MODEL

### 1. Tải một Model GGUF mẫu (Ví dụ: Llama-3.2-3B)
Tải bản nén 4-bit (`Q4_K_M`) tối ưu cho di động

### 2. Khởi chạy Llama Server tăng tốc GPU Adreno hoàn toàn
Nhờ cấu hình PATH ở trên, giờ đây bạn có thể đứng ở bất kỳ thư mục nào và gọi lệnh trực tiếp:
```bash
llama-server --port 1234 -ngl 99 -t 4 -m /sdcard/Download/Qwen3-4B-Q4_K_M.gguf
```
**Giải thích các cờ quan trọng:**
* `--port 1234`: Chạy cổng Web UI/API tại port 1234.
* `-ngl 99` (Number of GPU Layers): Ép toàn bộ các layer của model (ví dụ 28/28 layer) chuyển hết vào GPU xử lý, giải phóng CPU.
* `-t 4` (Threads): Giới hạn số luồng CPU bằng đúng số **Nhân hiệu năng cao (Performance Cores)** của chip Snapdragon (thường là 3 hoặc 4 nhân), tuyệt đối không dùng hết tất cả core để tránh nghẽn luồng điều phối trên Android.

### 3. Trải nghiệm Chat trực quan qua trình duyệt
Mở trình duyệt Web trên điện thoại (Chrome/Samsung Internet) truy cập địa chỉ:
👉 **`http://localhost:1234`** hoặc **`http://127.0.0.1:1234`**

---

## 📦 PHẦN 6: ĐÓNG GÓI BINARY ĐỂ LƯU TRỮ HOẶC CHIA SẺ

Nếu bạn muốn giữ lại bản build này để cài cho máy khác dùng chip Snapdragon mà không cần tốn thời gian compile lại, hãy đóng gói thư mục `bin`:

```bash
# Di chuyển vào thư mục build và nén
cd ~/llama.cpp/build
tar -czvf llama-bin-aarch64-snapdragon-adreno-opencl.tar.gz bin

# Copy file nén ra thư mục Download ngoài máy để lưu trữ/gửi file
cp llama-bin-aarch64-snapdragon-adreno-opencl.tar.gz /sdcard/Download/
```

---

## ❓ SECTION FAQ: CÁC LỖI THƯỜNG GẶP VÀ CÁCH XỬ LÝ (TROUBLESHOOTING)

#### Lỗi 1: `E: Unable to locate package libopencl` khi chạy lệnh cài đặt package.
* **Nguyên nhân:** Thư viện OpenCL nằm trong repo mở rộng riêng của người dùng Termux, không có ở kho gốc.
* **Cách khắc phục:** Chạy lệnh `pkg install tur-repo -y` trước, cập nhật lại bằng `pkg update -y` rồi thay thế gói `libopencl` bằng gói `ocl-icd` (đã được cập nhật trong cẩm nang).

#### Lỗi 2: Lỗi biên dịch `build.ninja: No such file or directory` hoặc thiếu Python.
* **Nguyên nhân:** Quá trình chạy CMake ban đầu bị hủy do hệ thống thiếu Python 3 làm môi trường trung gian, dẫn tới file cấu hình build hệ thống `build.ninja` chưa được sinh ra.
* **Cách khắc phục:** Cài đặt python bằng lệnh `pkg install python -y`, sau đó thực hiện lệnh xóa thư mục build cũ `rm -rf build` và chạy lại từ lệnh cấu hình `cmake -B build...`

#### Lỗi 3: Lỗi biên dịch `clang frontend command failed with exit code 139`.
* **Nguyên nhân:** Do trình biên dịch Clang mặc định trên Android bị lỗi bug phân rã các lệnh hàm toán học vector hóa quá sâu (`+dotprod+i8mm+sve+sme`) khi xử lý file tính toán ma trận cốt lõi CPU của llama.cpp.
* **Cách khắc phục:** Thêm cờ `-DGGML_NATIVE=OFF -DCMAKE_C_FLAGS="-O2 -fno-vectorize" -DCMAKE_CXX_FLAGS="-O2 -fno-vectorize"` vào lệnh CMake để tắt tính năng tự động vectorize lỗi của CPU Clang.

#### Lỗi 4: Thông báo `warning: no usable GPU found` hoặc `library /vendor/lib64/libOpenCL_adreno.so is not accessible for the namespace`.
* **Nguyên nhân:** Cơ chế bảo mật Linker Namespace cô lập của hệ điều hành Android (bắt đầu từ Android 10 trở lên) chặn không cho các app cài ngoài như Termux tự ý load trực tiếp các file thư viện hệ thống `.so` trong phân vùng `/vendor`.
* **Cách khắc phục:** Khai báo biến môi trường `export LD_LIBRARY_PATH=/vendor/lib64:$LD_LIBRARY_PATH` vào file `.bashrc` để hệ thống cưỡng chế nạp và liên kết driver an toàn vào không gian chạy của tiến trình binary.

***
*Tài liệu được thiết lập dựa trên thực nghiệm thành công trực tiếp trên kiến trúc Snapdragon 8 Elite.*
