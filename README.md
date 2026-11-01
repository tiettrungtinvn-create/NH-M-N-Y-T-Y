# CareQueue — Hệ thống xếp hàng khám bệnh và cấp cứu

Repository giữ nguyên hai phần chính:

- `code/include/`, `code/src/`: chương trình C++ và các cấu trúc dữ liệu tự cài.
- `wed/`: trang điều phối, màn hình chờ và minh họa Queue.

Web server nằm tại `code/src/web_server.cpp`. Server gọi **cùng lớp
`HeThongBenhVien`** mà chương trình console sử dụng. Bệnh nhân thường vào Queue
mảng vòng (`HangDoi.h`), ca ưu tiên vào danh sách liên kết có thứ tự
(`HangUuTien.h`), lịch sử gọi vào Deque danh sách liên kết đôi
(`HangDoiHaiDau.h`). Không có bộ hàng đợi web riêng.
Danh sách chờ và lịch sử gọi trên web được đọc từ Queue, Priority Queue và Deque
của lớp này; nút gọi từng phiếu chỉ cho phép bệnh nhân đứng đầu phòng.

## Chạy trên máy cá nhân

Cần trình biên dịch C++17 và CMake 3.16 trở lên:

```sh
cmake -S . -B build
cmake --build build
./build/he_thong_benh_vien --demo
./build/hospital_web
```

Trên Windows, file thực thi nằm trong `build/Debug/` hoặc `build/` tùy
trình biên dịch. Mở `http://127.0.0.1:8080/` để điều phối;
`/waiting` là màn hình chờ, `/queue-demo` là minh họa cấu trúc dữ liệu.
Chạy chương trình từ thư mục gốc repository để server tìm thấy `wed/`.

Nếu không dùng CMake, có thể biên dịch trực tiếp (Linux):

```sh
g++ -std=c++17 -Wall -Wextra -Icode/include code/src/main.cpp code/src/HeThong_*.cpp -o he_thong_benh_vien
g++ -std=c++17 -Wall -Wextra -Icode/include code/src/web_server.cpp code/src/HeThong_*.cpp -o hospital_web
```

Trên Windows (MinGW), thêm `-lws2_32` vào lệnh biên dịch `hospital_web`.
Dữ liệu web được lưu tại `hospital-data.txt` theo mặc định.

## Chạy website bằng Docker / Render

Repository có `Dockerfile`. Tạo Web Service dạng **Docker** từ repository,
sau đó đặt:

- `ADMIN_PASSWORD`: mật khẩu quản trị mạnh; bắt buộc khi server mở ra Internet.
- `HOSPITAL_DATA=/var/data/hospital-data.txt`: đường dẫn dữ liệu.
- `HOST=0.0.0.0`, `PORT=10000` (Dockerfile đã đặt mặc định).
- Gắn persistent disk tại `/var/data` nếu muốn giữ dữ liệu sau khi triển khai lại.

Trang điều phối và API hồ sơ dùng HTTP Basic Auth với tên đăng nhập `admin`.
Trình duyệt sẽ hỏi mật khẩu khi mở trang chủ. `/waiting` công khai nhưng
chỉ trả mã phiếu và phòng khám, không trả tên hay triệu chứng. Hosting cần
HTTPS; không đưa dữ liệu bệnh nhân thật vào bản demo công khai. Nếu Render
không có persistent disk, dữ liệu vẫn mất khi instance được thay mới.

## Quy ước nghiệp vụ

Trong C++, `doUuTien=1` là thường, `2..5` là ưu tiên tăng dần và `999`
là ca chuyển khoa cấp cứu. Giao diện dùng cấp độ phân loại y tế theo chiều
ngược lại: `1` nguy kịch, `5` thông thường. Server chuyển bằng công thức
`doUuTien = 6 - triageLevel`. Người cùng mức được gọi theo FIFO; thời gian
chờ dự kiến là **10 phút × (số người đứng trước + lượt đang khám trong cùng phòng)**.
Mỗi phòng chỉ gọi một bệnh nhân đang khám tại một thời điểm; khi chọn “Tất cả
khoa”, hệ thống bỏ qua phòng đang bận và gọi người ưu tiên cao nhất ở phòng còn trống.

## Kiểm thử nhanh

Sau khi biên dịch `hospital_web`:

```sh
node tests/web_smoke.js ./build/hospital_web
```

Kiểm thử này kiểm tra đăng nhập, tiếp nhận, ưu tiên, gọi số, chuyển khoa,
màn hình chờ không lộ tên và khôi phục dữ liệu khi khởi động lại. Console
có kịch bản `--demo` để trình bày các cấu trúc dữ liệu.
