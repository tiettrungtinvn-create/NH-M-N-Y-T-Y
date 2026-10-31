ĐT02 – Hệ thống xếp hàng khám bệnh & cấp cứu bệnh viện
Phiên bản hướng đối tượng với lớp HeThongBenhVien. Ba cấu trúc dữ liệu đều tự cài đặt:
Cấu trúc
Cài đặt
Vai trò
Queue
Mảng vòng (Circular Array) – HangDoi
Hàng thường của từng phòng (FIFO)
Priority Queue
Danh sách liên kết có thứ tự – HangUuTien
Hàng ưu tiên / cấp cứu của từng phòng
Deque
Danh sách liên kết đôi – HangDoiHaiDau
Lịch sử gọi số
Nạp chồng toán tử: BenhNhan::operator<, >, ==, operator<<.
Biên dịch và chạy
make              # biên dịch tất cả file .cpp
./he_thong_benh_vien          # menu
./he_thong_benh_vien --demo   # kịch bản mẫu
Hoặc: g++ -std=c++17 -Wall -Wextra *.cpp -o he_thong_benh_vien
Phân công 10 phần
#
File
Nhiệm vụ
Phụ thuộc
1
BenhNhan.h
Hằng số dùng chung; struct bệnh nhân; nạp chồng <, >, ==, <<
–
2
HangDoi.h
Queue mảng vòng: them, lay, xemDau, xemTaiViTri, layTheoMa
1
3
HangDoiHaiDau.h
Deque DSLK đôi: themDau/Cuoi, layDau/Cuoi, xemTaiViTri
1
4
HangUuTien.h
Priority Queue DSLK có thứ tự: them (chèn đúng vị trí), lay, layTheoMa…
1
5
HeThongBenhVien.h
Khai báo struct Phong và lớp HeThongBenhVien
1–4
6
HeThong_TiepNhan.cpp
Hàm tạo, timPhong, taoPhong, timBenhNhan, tiếp nhận và phân luồng
5
7
HeThong_ChuyenKhoa.cpp
Chuyển khoa cấp cứu ưu tiên
5
8
HeThong_GoiSo.cpp
Gọi số tự động, lịch sử gọi
5
9
HeThong_ThongKe.cpp
Thời gian chờ dự kiến, bảng điện tử, thống kê phòng
5
10
main.cpp
Menu, kịch bản demo, kiểm thử
1–9
Quy ước
doUuTien càng lớn càng khẩn: 1 = thường (vào hangThuong), 2–5 = ưu tiên (vào hangCapCuu), 999 = đã chuyển Khoa Cấp cứu.
Cùng độ ưu tiên: số thứ tự nhỏ hơn (đến trước) được gọi trước.
Chờ dự kiến = số người đứng trước × PHUT_MOI_LUOT (10 phút); bệnh nhân thường phải chờ cả hàng ưu tiên của phòng.
Thứ tự commit gợi ý
1 → 2, 3, 4 → 5 → 6, 7, 8, 9 → 10

