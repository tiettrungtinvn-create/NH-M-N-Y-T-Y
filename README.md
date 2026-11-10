# CareQueue UTC2 — Xếp hàng khám bệnh & cấp cứu (C++ Console)

Bài tập lớn C++ về **Queue, Priority Queue, Deque**. Chương trình chạy trên terminal bằng C++17. Mã nguồn, tài liệu và script chạy nằm ngay ở thư mục gốc repository.

## Bắt đầu theo yêu cầu bài tập lớn

Đọc [bảng đối chiếu YC01–YC04](docs/QUY_DINH_YC01_YC04.md) trước khi nộp. Các mục quan trọng:

- **YC01 GitHub:** [quy trình commit và pull request](docs/QUY_TRINH_GITHUB.md). Mã nguồn đã ở [repository chính thức](https://github.com/tiettrungtinvn-create/NH-M-N-Y-T-Y); nhóm cần đối chiếu lịch sử đóng góp bằng tài khoản riêng.
- **YC02 Jira:** [15 nhiệm vụ cụ thể cho 5 thành viên](docs/KE_HOACH_JIRA.md) cùng 4 nhiệm vụ chung đã có trên Jira. Nhóm cần đối chiếu trạng thái, thời hạn và minh chứng thực tế.
- **Jira đang theo dõi:** [YC01 NNT-58](https://nguyentienthanh11207.atlassian.net/browse/NNT-58), [YC02 NNT-59](https://nguyentienthanh11207.atlassian.net/browse/NNT-59), [YC03 NNT-60](https://nguyentienthanh11207.atlassian.net/browse/NNT-60), [YC04 NNT-61](https://nguyentienthanh11207.atlassian.net/browse/NNT-61). Trạng thái Done không thay thế link commit/PR hoặc kết quả nghiệm thu.
- **YC03 LaTeX:** [nguồn báo cáo](bao_cao.tex) và [hướng dẫn Overleaf](docs/bao_cao/README.md). Điền thông tin nhóm, biên dịch PDF và xác nhận 40–50 trang trước khi nộp.
- **PDF tham khảo đã chỉnh sửa:** [bản 44 trang](docs/BAO_CAO_CHINH_SUA.pdf) đồng bộ mã console, Jira và YC01–YC04. Bản PDF này được dựng lại từ nội dung LaTeX và mã nguồn để sửa bản PDF cũ; nhóm cần biên dịch và đối chiếu `bao_cao.tex` bằng XeLaTeX trước khi nộp theo YC03.
- **YC04 C++:** mã ở `include/`, `src/`, [65 ca kiểm thử](tests/test_all.cpp) và [kịch bản demo](docs/KICH_BAN_DEMO.md). Bản phát hành này là ứng dụng console; chưa có giao diện web.

## Chạy nhanh

- Linux/macOS: `./run.sh` (tự biên dịch lần đầu) · `./run.sh --demo` · `./run.sh test`
- Windows: `run.bat` (cần g++ của MinGW/w64devkit hoặc cl của Visual Studio; nếu chưa có, file sẽ hướng dẫn cài)

## Biên dịch và chạy

```sh
./run.sh                       # menu tương tác (Linux/macOS/Git Bash)
./run.sh --demo                # kịch bản demo tự động
./run.sh test                  # 65 ca kiểm thử tự động
./run.sh < tests/input_demo.txt # demo bằng dữ liệu nhập sẵn
```

Trên Windows, chạy `run.bat` hoặc `run.bat --demo`. Repository hiện không có Makefile. Nếu biên dịch thủ công trên Linux/macOS:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/main.cpp src/HeThong_*.cpp -o he_thong_benh_vien
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude tests/test_all.cpp src/HeThong_*.cpp -o run_tests
./run_tests
```

Tham số: `--file <đường-dẫn.csv>` (đổi file lưu trữ), `--demo`, `--help`.
Thử với dữ liệu mẫu: `./he_thong_benh_vien --file data/mau_benhvien.csv` (chương trình sẽ ghi đè file này khi có thay đổi — nên chép ra bản khác trước).

Windows (MinGW): `g++ -std=c++17 -Iinclude src/*.cpp -o he_thong_benh_vien.exe`. Chương trình xuất chữ không dấu để hiển thị đúng trên mọi terminal.

## Cấu trúc

| Đường dẫn | Vai trò |
| --- | --- |
| `include/HangDoi.h` | **Queue** — mảng vòng, FIFO, dung lượng 100 |
| `include/HangUuTien.h` | **Priority Queue** — danh sách liên kết có thứ tự |
| `include/HangDoiHaiDau.h` | **Deque** — danh sách liên kết đôi (lịch sử gọi số) |
| `include/BenhNhan.h` | Struct bệnh nhân, nạp chồng `<`, `>`, `==`, `<<` |
| `include/HeThongBenhVien.h` | Lớp nghiệp vụ điều phối 3 cấu trúc trên |
| `src/HeThong_TiepNha.cpp` | Tiếp nhận, kiểm tra hợp lệ, tìm bệnh nhân/phòng |
| `src/HeThong_GoiSo.cpp` | Gọi số (tự động / theo phòng / theo mã), hủy, lịch sử |
| `src/HeThong_ChuyenKhoa.cpp` | Chuyển khoa cấp cứu (ưu tiên 999) |
| `src/HeThong_ThongKe.cpp` | Bảng điện tử, thời gian chờ, thống kê |
| `src/HeThong_LuuTru.cpp` | Lưu/đọc CSV, kiểm tra toàn vẹn dữ liệu |
| `src/main.cpp` | Menu tương tác + kịch bản `--demo` |
| `tests/test_all.cpp` | 65 ca kiểm thử tự động |
| `tests/input_demo.txt` | Dữ liệu nhập sẵn (có cả lỗi nhập) để demo |
| `docs/KICH_BAN_DEMO.md` | Kịch bản demo + bảng ca kiểm thử + câu hỏi phản biện |
| `bao_cao.tex` | Nguồn báo cáo LaTeX có phụ lục mã nguồn |
| `docs/KE_HOACH_JIRA.md` | WBS 5 Epic, 15 Task và tiêu chí nghiệm thu |
| `docs/QUY_TRINH_GITHUB.md` | Cách làm việc qua commit, nhánh và PR |
| `docs/SLIDE_DEMO.pptx` | Slide thuyết trình và kịch bản demo |

## Quy tắc nghiệp vụ

- Độ ưu tiên nhập **1 (thường) … 5 (khẩn nhất)**. Mức 1 vào `HangDoi` (FIFO); mức 2–5 vào `HangUuTien`.
- Chuyển khoa cấp cứu đặt ưu tiên **999** (cao nhất), vào phòng "Khoa Cap Cuu".
- Gọi số: ưu tiên cao trước; cùng mức → theo thứ tự đến (STT nhỏ trước). Gọi tự động xét tất cả phòng.
- Thời gian chờ dự kiến = **10 phút × số bệnh nhân đứng trước** trong cùng phòng (ưu tiên đứng trước thường).
- Giới hạn: 50 bệnh nhân toàn hệ thống (menu), 10 phòng, 100 bệnh nhân/hàng, lịch sử 100 lượt gần nhất.
- Tên/phòng: không rỗng, ≤ 50 ký tự, không chứa dấu phẩy hoặc xuống dòng.

## Lưu trữ dữ liệu (CSV)

```
CAREQUEUE,1          <- nhận diện + phiên bản
STT,8                <- số thứ tự đã cấp (để cấp tiếp đúng)
R,Noi                <- phòng
Q,<mã>,<tên>,<phòng>,<ưu tiên>,<stt>   <- bệnh nhân đang chờ
H,<mã>,<tên>,<phòng>,<ưu tiên>,<stt>   <- lịch sử gọi (cũ → mới)
```

- Tự lưu sau mỗi thao tác làm thay đổi dữ liệu và khi thoát; tự nạp khi khởi động.
- Ghi vào file `.tmp` trước khi đổi tên. Trên Windows, nếu phải xóa file đích để đổi tên, có một khoảng rủi ro mất bản cũ; sao lưu CSV khi dùng ngoài demo.
- Nạp file **kiểm tra toàn bộ trước khi áp dụng**: file hỏng → báo lỗi kèm số dòng, dữ liệu đang chạy giữ nguyên. Khi khởi động gặp file hỏng, chương trình sao lưu thành `*.loi` rồi bắt đầu hệ thống rỗng (không ghi đè mất file gốc).

## Xử lý ngoại lệ

- Nhập liệu: sai kiểu/ngoài khoảng/để trống → báo lỗi và hỏi lại; hết dữ liệu nhập (Ctrl+D) → lưu và thoát sạch.
- Nghiệp vụ: trả `false` kèm lý do (`loiCuoi()`), hiển thị cho người dùng.
- File: `std::runtime_error` (không mở được, sai định dạng, trùng mã, vượt giới hạn...), bắt ở tầng giao diện.

## Kiểm tra bộ nhớ

Có thể biên dịch bộ test với AddressSanitizer/UBSan hoặc chạy `valgrind ./run_tests` nếu môi trường hỗ trợ. Chỉ kết luận không có lỗi bộ nhớ khi đã chạy và lưu log trên máy kiểm thử thực tế.

