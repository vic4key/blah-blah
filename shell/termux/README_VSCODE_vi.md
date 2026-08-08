# 💻 SETUP VS CODE CHO ANDROID (TERMUX)

Tài liệu này hướng dẫn chi tiết từ bước cài đặt Visual Studio Code (bản Web code-server) cho đến các bước cấu hình mạng và sửa lỗi crash hệ thống trên Android 14/15.

---

## 📥 BƯỚC 1: CÀI ĐẶT VS CODE (CODE-SERVER)

Mở Termux trên điện thoại và chạy lệnh duy nhất sau đây để cài đặt phiên bản VS Code chính thức dành riêng cho Android (được tối ưu bởi cộng đồng Termux):

```bash
pkg update && pkg install code-server -y
```
*(Hệ thống sẽ tự động tải lõi VS Code và cấu hình môi trường Node.js trong khoảng 1-2 phút).*

---

## ⚙️ BƯỚC 2: CẤU HÌNH MỞ CỔNG MẠNG ĐỂ PC TRUY CẬP

Mặc định, VS Code sau khi cài chỉ cho phép mở nội bộ trên điện thoại. Ta cần cấu hình để mở quyền truy cập cho máy tính PC:

1. **Khởi chạy thử lần đầu để hệ thống tự sinh file cấu hình:**
   ```bash
   code-server
   ```
   *Nhìn thấy màn hình hiện vài dòng thông báo log thì bấm tổ hợp phím **`Ctrl` + `C`** để dừng lại.*

2. **Mở file cấu hình vừa sinh ra bằng nano:**
   ```bash
   nano ~/.config/code-server/config.yaml
   ```

3. **Chỉnh sửa chính xác 2 thông số sau:**
   * Tìm dòng `bind-addr` và `password` sửa như sau:
     ```yaml
      bind-addr: 127.0.0.1:8080
      auth: password
      password: <your-old-password>
      cert: false

      bind-addr: 0.0.0.0:8080
      password: <your-new-password>
     ```
   * Địa chỉ đổi thành `0.0.0.0` để cho phép PC kết nối vào cổng 8080).
   * Mật khẩu này để đăng nhập từ PC.
   * Nhấn `Ctrl + O` -> `Enter` để lưu, `Ctrl + X` để thoát.

---

## 🛠️ BƯỚC 3: ÁP DỤNG BẢN VÁ TRIỆT ĐỂ LỖI CRASH KẾT NỐI (CONNECTION CLOSED)

### 1. Nguyên nhân gây lỗi Crash
Hệ điều hành Android mới (Android 14/15) chặn hoàn toàn các ứng dụng không có quyền Root đọc địa chỉ MAC của máy. Khi PC kết nối vào, lõi VS Code cố gọi file mã nhị phân gốc của gói `@vscode/deviceid` để check MAC. Do không đọc được, nó sẽ ném ra lỗi `Unsupported platform`, khiến Android tự động đóng (`Kill`) tiến trình server ngay lập tức và làm sập kết nối SSH/ADB của bạn trên máy tính.

### 2. Lệnh vá lỗi ghi đè cấu trúc Object (Chỉ cần chạy 1 lần duy nhất)
Để khắc phục hoàn toàn, chúng ta ghi đè module định danh nhị phân lỗi kia bằng một đối tượng JavaScript thuần (Node.js CommonJS chuẩn). Module này sẽ tự động trả về một chuỗi mã ID tĩnh hợp lệ mà lõi VS Code mong muốn:

```bash
cat << 'INNER_EOF' > /data/data/com.termux/files/usr/lib/code-server/lib/vscode/node_modules/@vscode/deviceid/dist/index.js
module.exports = {
    getDeviceId: async function() {
        return "android-s26-ultra-fixed-id";
    }
};
INNER_EOF
```

---

## 🚀 BƯỚC 4: KHỞI CHẠY VÀ TRUY CẬP VS CODE TỪ PC

### 1. Khởi chạy sạch màn hình terminal từ điện thoại
Chạy lệnh gạt bỏ log cảnh báo thừa để màn hình terminal luôn gọn gàng:
```bash
code-server --auth none --disable-telemetry 2>/dev/null
```

### 2. Cách kết nối từ trình duyệt máy tính
1. **Kiểm tra IP điện thoại:** Mở một tab mới trên Termux gõ lệnh `ifconfig` để xem địa chỉ IP mạng Wi-Fi tại mục `wlan0` (Ví dụ: `192.168.1.15`).
2. **Mở trình duyệt trên máy tính PC (Chrome, Edge...):** Nhập vào thanh địa chỉ:
   👉 **`http://<IP_ĐIỆN_THOẠI>:8080`** *(Ví dụ: `http://192.168.1.15:8080`)*
3. Kết nối từ nay sẽ được duy trì vĩnh viễn, bạn có thể lập trình, lưu file (Save) mượt mà mà không bao giờ bị văng kết nối SSH/ADB nữa.

### 💡 Mẹo nhỏ: Giữ hiệu năng cao cho điện thoại
Trước khi bắt đầu làm việc, hãy chạy lệnh này trong Termux để điện thoại luôn giữ kết nối mạng và không tự động ngủ khi bạn khóa màn hình:
```bash
termux-wake-lock
```
