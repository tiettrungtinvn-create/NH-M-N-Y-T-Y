# CareQueue UTC2 — Hệ thống xếp hàng khám bệnh và cấp cứu

## Bản giao nộp YC01–YC04

Thư mục [`console/`](console/README.md) là **gói C++ console độc lập** dùng để trình bày Queue mảng vòng, Priority Queue, Deque và nghiệm thu theo bốn yêu cầu chung. Trong đó có [bảng đối chiếu YC01–YC04](console/docs/QUY_DINH_YC01_YC04.md), [kế hoạch Jira](console/docs/KE_HOACH_JIRA.md), [báo cáo LaTeX](console/bao_cao.tex), [slide demo](console/docs/SLIDE_DEMO.pptx), mã nguồn và bộ kiểm thử **65/65 ca đạt**. Chạy từ thư mục `console/` bằng `run.bat` hoặc `./run.sh`; phần website và các lệnh CMake/Docker bên dưới vẫn dùng mã trong `code/`.

Các mốc GitHub/Jira và PDF 40–50 trang cần được kiểm chứng bằng lịch sử nhóm và bản biên dịch cuối, như ghi rõ trong tài liệu đối chiếu.

Jira cho bản console: [YC01 NNT-58](https://nguyentienthanh11207.atlassian.net/browse/NNT-58), [YC02 NNT-59](https://nguyentienthanh11207.atlassian.net/browse/NNT-59), [YC03 NNT-60](https://nguyentienthanh11207.atlassian.net/browse/NNT-60), [YC04 NNT-61](https://nguyentienthanh11207.atlassian.net/browse/NNT-61). [15 Task theo 5 thành viên](console/docs/KE_HOACH_JIRA.md) cũng đã được tạo. Các Task mới đang To Do để nhóm thêm minh chứng và nghiệm thu.

Bài tập lớn C++ về **Queue, Priority Queue và Deque**. Dự án có chương trình dòng lệnh để trình bày cấu trúc dữ liệu và **website công khai** (không cần đăng nhập) để tiếp nhận, điều phối bệnh nhân. Đây là bản học tập và trình diễn; không dùng để lưu hồ sơ bệnh án thật.

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
| `code/src/web_server.cpp` | Web server C++ và API kết nối giao diện với lớp lõi |
| `wed/index.html`, `wed/app.js` | Trang điều phối và logic giao diện |
| `wed/waiting.html` | Màn hình chờ công khai |
| `wed/queue-demo.html` | Minh họa cấu trúc hàng đợi |
| `CMakeLists.txt`, `Dockerfile` | Biên dịch trên máy và triển khai bằng Docker |
| `tests/web_smoke.js` | Kiểm thử nhanh các nghiệp vụ web |

Tên thư mục giao diện trong repository là `wed` (không phải `web`) và file server là `web_server.cpp`; dùng đúng hai tên này khi viết lệnh biên dịch.

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

- `/`: trang điều phối, tiếp nhận và gọi bệnh nhân (công khai, không cần đăng nhập).
- `/waiting`: màn hình chờ, chỉ hiển thị mã phiếu và phòng khám.
- `/queue-demo`: minh họa Queue, Priority Queue và Deque.
- `/health`: kiểm tra server còn hoạt động.

Nếu không dùng CMake, trên Linux có thể biên dịch trực tiếp:

```sh
g++ -std=c++17 -Wall -Wextra -Icode/include code/src/main.cpp code/src/HeThong_*.cpp -o he_thong_benh_vien
g++ -std=c++17 -Wall -Wextra -Icode/include code/src/web_server.cpp code/src/HeThong_*.cpp -o hospital_web
```

Trên Windows với MinGW, lệnh biên dịch `hospital_web` cần thêm `-lws2_32`.

## Truy cập công khai và lưu dữ liệu

Website là **bản công khai, không có đăng nhập**: bất kỳ ai có đường dẫn đều mở được trang điều phối (`/`), màn hình chờ (`/waiting`) và minh họa (`/queue-demo`), đồng thời tiếp nhận, gọi, chuyển khoa hoặc hủy phiếu. Vì vậy **chỉ dùng dữ liệu giả** cho bản demo, không nhập thông tin bệnh nhân thật.

Web lưu dữ liệu tại `hospital-data.txt` trong thư mục đang chạy, hoặc tại đường dẫn do biến `HOSPITAL_DATA` chỉ định. Chương trình dòng lệnh là bản trình diễn riêng, không đọc file dữ liệu web. Màn hình chờ công khai không hiển thị tên, năm sinh hay triệu chứng.

## Quy tắc xếp hàng

Giao diện nhập **mức 1 là nguy kịch**, **mức 5 là thông thường**. Lõi C++ dùng chiều ngược lại: `doUuTien = 6 - triageLevel`, nên mức 1 trên web có ưu tiên cao nhất. Ca được chuyển sang khoa cấp cứu nhận ưu tiên `999`. Người cùng mức ưu tiên được gọi theo thứ tự đến. Mỗi phòng chỉ có một lượt `Đang khám` tại một thời điểm.

Thời gian chờ dự kiến là **10 phút × số bệnh nhân đứng trước trong cùng phòng**, cộng thêm 10 phút nếu phòng đang có người khám. Đây là ước tính theo số lượt, không phải thời gian khám thực tế.

## Kiểm thử

Sau khi biên dịch `hospital_web`, chạy từ thư mục gốc:

```sh
node tests/web_smoke.js ./build/hospital_web
```

Trên Windows, thay đường dẫn cuối bằng vị trí thật của `hospital_web.exe`, ví dụ `./build/Release/hospital_web.exe`. Bài kiểm thử dùng cổng và file dữ liệu tạm, kiểm tra tiếp nhận, ưu tiên/FIFO, phòng bận, chuyển khoa, khôi phục dữ liệu và việc màn hình chờ không lộ tên bệnh nhân.

## Triển khai công khai bằng Docker / Render

`Dockerfile` biên dịch chương trình C++ rồi chép chương trình cùng thư mục `wed/` vào image chạy web. Trên Render, tạo **Web Service dạng Docker** từ repository; không cần đặt mật khẩu hay biến bí mật nào. `Dockerfile` đã đặt sẵn `HOST=0.0.0.0`, `PORT=10000` và `HOSPITAL_DATA=/var/data/hospital-data.txt`.

Nếu muốn giữ dữ liệu sau mỗi lần triển khai lại, gắn persistent disk tại `/var/data`; nếu không, dữ liệu sẽ mất khi instance được thay mới. Website phải chạy qua HTTPS (Render cung cấp sẵn).
