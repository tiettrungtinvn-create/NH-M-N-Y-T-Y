# CareQueue UTC2 — Hệ thống xếp hàng khám bệnh và cấp cứu

Đồ án C++ về **Queue, Priority Queue và Deque**. Dự án có chương trình dòng lệnh để trình bày cấu trúc dữ liệu và website để tiếp nhận, điều phối bệnh nhân. Đây là bản học tập và trình diễn; không dùng để lưu hồ sơ bệnh án thật.

## Chức năng

- Tiếp nhận bệnh nhân, cấp mã phiếu và đưa vào phòng khám được chọn.
- Gọi lượt tiếp theo theo mức ưu tiên; bệnh nhân cùng mức được gọi theo FIFO.
- Chuyển bệnh nhân đang chờ sang khoa cấp cứu với mức ưu tiên cao nhất.
- Tính thời gian chờ dự kiến, đánh dấu hoàn tất/vắng mặt, hủy phiếu và chuyển khoa.
- Hiển thị màn hình chờ công khai theo mã phiếu và phòng; minh họa Queue trên trang riêng.

Website dùng lớp `HeThongBenhVien` của chương trình C++, không tạo một hàng đợi độc lập trong JavaScript. Hàng đợi thường là **mảng vòng** (`HangDoi`), hàng ưu tiên là **danh sách liên kết có thứ tự** (`HangUuTien`), lịch sử gọi là **hàng đợi hai đầu** (`HangDoiHaiDau`). `BenhNhan` nạp chồng toán tử `<` để so sánh mức ưu tiên rồi đến thứ tự đến.

## Cấu trúc dự án

| Đường dẫn | Vai trò |
| --- | --- |
| `code/include/` | Khai báo bệnh nhân, ba cấu trúc dữ liệu và hệ thống bệnh viện |
| `code/src/main.cpp` | Menu chương trình dòng lệnh và kịch bản `--demo` |
| `code/src/HeThong_*.cpp` | Tiếp nhận, chuyển khoa, gọi số và thống kê |
| `code/src/websever.cpp` | Web server C++ và API kết nối giao diện với lớp lõi |
| `wed/index.html`, `wed/app.js` | Trang điều phối và logic giao diện |
| `wed/waiting.html` | Màn hình chờ công khai |
| `wed/queue-demo.html` | Minh họa cấu trúc hàng đợi |
| `CMakeLists.txt`, `Dockerfile` | Biên dịch trên máy và triển khai bằng Docker |
| `tests/web_smoke.js` | Kiểm thử nhanh các nghiệp vụ web |

Tên thư mục giao diện trong repository là `wed` và tên file server là `websever.cpp`; dùng đúng hai tên này khi viết lệnh biên dịch.

## Chạy trên máy cá nhân

Cài **CMake 3.16 trở lên** và trình biên dịch **C++17** (GCC, Clang hoặc MSVC). Cần **Node.js 18 trở lên** nếu muốn chạy bài kiểm thử; website tự nó không cần Node.js.

Từ **thư mục gốc repository**, chạy:

```sh
cmake -S . -B build
cmake --build build --config Release
```

Chạy bản dòng lệnh để xem ví dụ:

```sh
./build/he_thong_benh_vien --demo
```

Chạy website:

```sh
./build/hospital_web
```

Mở `http://127.0.0.1:8080/`. Với Visual Studio trên Windows, hai file `.exe` có thể nằm trong `build/Release/`; với MinGW chúng thường nằm ngay trong `build/`. Chạy server khi đang ở thư mục gốc để tìm được các file trong `wed/`. Dừng server bằng `Ctrl+C`.

Các đường dẫn của website:

- `/`: trang điều phối, tiếp nhận và gọi bệnh nhân.
- `/waiting`: màn hình chờ, chỉ công khai mã phiếu và phòng khám.
- `/queue-demo`: minh họa Queue, Priority Queue và Deque.
- `/health`: kiểm tra server còn hoạt động.

Nếu không dùng CMake, trên Linux có thể biên dịch trực tiếp:

```sh
g++ -std=c++17 -Wall -Wextra -Icode/include code/src/main.cpp code/src/HeThong_*.cpp -o he_thong_benh_vien
g++ -std=c++17 -Wall -Wextra -Icode/include code/src/websever.cpp code/src/HeThong_*.cpp -o hospital_web
```

Trên Windows với MinGW, lệnh biên dịch `hospital_web` cần thêm `-lws2_32`.

## Đăng nhập và lưu dữ liệu

Khi chỉ chạy tại `127.0.0.1`, bạn có thể mở trang điều phối mà không đặt mật khẩu. Nếu muốn bật đăng nhập, đặt biến môi trường `ADMIN_PASSWORD` **trước khi chạy** server. Tên đăng nhập là `admin`; mật khẩu là giá trị bạn tự đặt. Khi mở server ra mạng bằng `HOST=0.0.0.0`, biến này là bắt buộc. Không ghi mật khẩu vào repository.

Web lưu dữ liệu tại `hospital-data.txt` trong thư mục đang chạy, hoặc tại đường dẫn do biến `HOSPITAL_DATA` chỉ định. Chương trình dòng lệnh là bản trình diễn riêng, không đọc file dữ liệu web. Màn hình chờ công khai không hiển thị tên, năm sinh hay triệu chứng, nhưng trang điều phối và API hồ sơ cần được bảo vệ bằng mật khẩu khi đưa lên Internet. Chỉ dùng dữ liệu giả cho bản demo công khai.

## Quy tắc xếp hàng

Giao diện nhập **mức 1 là nguy kịch**, **mức 5 là thông thường**. Lõi C++ dùng chiều ngược lại: `doUuTien = 6 - triageLevel`, nên mức 1 trên web có ưu tiên cao nhất. Ca được chuyển sang khoa cấp cứu nhận ưu tiên `999`. Người cùng mức ưu tiên được gọi theo thứ tự đến. Mỗi phòng chỉ có một lượt `Đang khám` tại một thời điểm.

Thời gian chờ dự kiến là **10 phút × số bệnh nhân đứng trước trong cùng phòng**, cộng thêm 10 phút nếu phòng đang có người khám. Đây là ước tính theo số lượt, không phải thời gian khám thực tế.

## Kiểm thử

Sau khi biên dịch `hospital_web`, chạy từ thư mục gốc:

```sh
node tests/web_smoke.js ./build/hospital_web
```

Trên Windows, thay đường dẫn cuối bằng vị trí thật của `hospital_web.exe`, ví dụ `./build/Release/hospital_web.exe`. Bài kiểm thử dùng cổng và file dữ liệu tạm, kiểm tra tiếp nhận, ưu tiên/FIFO, phòng bận, chuyển khoa, khôi phục dữ liệu, quyền truy cập và việc màn hình chờ không lộ tên bệnh nhân.

## Triển khai bằng Docker / Render

`Dockerfile` biên dịch chương trình C++ rồi chép chương trình cùng thư mục `wed/` vào image chạy web. Trên Render, tạo **Web Service dạng Docker** từ repository và đặt `ADMIN_PASSWORD` là mật khẩu mạnh. `Dockerfile` đã đặt `HOST=0.0.0.0`, `PORT=10000` và `HOSPITAL_DATA=/var/data/hospital-data.txt`.

Nếu cần giữ dữ liệu qua lần triển khai lại, gắn **persistent disk** tại `/var/data`. Nếu không có ổ lưu trữ lâu dài, dữ liệu sẽ mất khi instance được thay mới. Dùng HTTPS khi truy cập trang điều phối qua Internet.
