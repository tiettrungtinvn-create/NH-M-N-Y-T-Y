# Kịch bản demo & ca kiểm thử — CareQueue UTC2

Thời lượng gợi ý: **10–12 phút** (2 phút giới thiệu, 6 phút demo, 2 phút kiểm thử/bộ nhớ, còn lại phản biện).

## 0. Chuẩn bị

```sh
make                      # biên dịch, không được có cảnh báo
rm -f data/benhvien.csv   # bắt đầu từ hệ thống trống
```

Mở sẵn 2 cửa sổ terminal: một để chạy chương trình, một để `cat data/benhvien.csv` chứng minh dữ liệu được lưu.

## 1. Demo tự động (1 phút) — `make demo`

Chạy kịch bản có sẵn trong `main.cpp`: tiếp nhận 6 bệnh nhân, 4 ca lỗi, bảng điện tử, ước tính chờ, chuyển cấp cứu, **lưu → nạp lại bằng hệ thống mới**, gọi số, hủy, lịch sử. Dùng để cho giảng viên thấy tổng quan trước khi demo tương tác.

## 2. Demo tương tác (6 phút) — `make run`

Mỗi bước: **nhập gì → kết quả mong đợi → điểm cần nói**.

| Mã | Thao tác (menu → nhập) | Kết quả mong đợi | Điểm cần nói |
| --- | --- | --- | --- |
| TC01 | `1` → `101`, `Nguyen Van A`, `Noi`, `1` | Tiếp nhận THÀNH CÔNG | Mức 1 → vào `HangDoi` (mảng vòng) |
| TC02 | Tiếp nhận tiếp: `102 Tran Thi B / Noi / 3`, `104 Pham Thi D / Noi / 5`, `103 Le Van C / Ngoai / 1`, `105 Hoang Van E / Nhi / 2` | Cả 4 THÀNH CÔNG | Mức 2–5 → `HangUuTien` (danh sách liên kết có thứ tự) |
| TC03 | `1` → mã `101` (trùng) | THẤT BẠI "Ma benh nhan 101 da ton tai." | Kiểm tra trùng mã toàn hệ thống |
| TC04 | Ở menu gõ `abc`, rồi `99` | Báo "Gia tri khong hop le", hỏi lại | Nhập sai không làm sập chương trình |
| TC05 | `1` → mã `110`, tên để trống, rồi tên `Test`, phòng `Noi`, ưu tiên `7` | Báo "Khong duoc de trong", rồi báo ưu tiên 1..5 và hỏi lại | Kiểm tra từng trường khi nhập |
| TC06 | `1` → `111`, tên `A,B`, `Noi`, `1` | THẤT BẠI (tên chứa dấu phẩy) | Dấu phẩy sẽ phá định dạng CSV nên bị chặn |
| TC07 | `8` Bảng điện tử | Phòng Noi: 104 (0 phút), 102 (10), 101 (20); Ngoai: 103; Nhi: 105 | Ưu tiên cao đứng trước, cùng mức theo STT |
| TC08 | `7` → `101`; `7` → `999` | "can cho khoang 20 phut"; "[LOI] Khong tim thay BN 999" | 10 phút × số người đứng trước |
| TC09 | `2` → `103`; `2` → `103` lần nữa | Lần 1: THÀNH CÔNG + thông báo khẩn; lần 2: THẤT BẠI "da o Khoa Cap Cuu" | Ưu tiên 999, chuyển giữa các phòng |
| TC10 | `3` gọi tự động 5 lần | Thứ tự: Le Van C (cấp cứu) → Pham Thi D → Tran Thi B → Hoang Van E → Nguyen Van A | Chọn đầu hàng tốt nhất giữa các phòng |
| TC11 | Tiếp nhận lại vài BN; `4` → `Nhi` (gọi), `4` → `Nhi` lần nữa, `4` → `Mat` | Gọi được; "khong co benh nhan dang cho"; "khong ton tai" | Gọi theo phòng, báo lý do lỗi |
| TC12 | `5` → mã một BN đang chờ; `6` → mã khác; `6` lần nữa | Gọi thành công; Hủy thành công; Hủy lần 2 THẤT BẠI | Xóa phần tử giữa hàng (`layTheoMa`) |
| TC13 | `10` Lịch sử gọi | Danh sách các lượt đã gọi theo thứ tự cũ → mới | `HangDoiHaiDau`, giữ 100 lượt gần nhất |
| TC14 | `0` Thoát; `cat data/benhvien.csv`; chạy lại `make run` | File CSV có `STT`, `R`, `Q`, `H`; mở lại thấy đủ dữ liệu, STT tiếp tục | Lưu bền vững, STT không bị cấp lại |
| TC15 | `13` → gõ `no`; sau đó `13` → `yes` | Hủy; rồi xóa sạch và lưu | Xác nhận trước thao tác nguy hiểm |
| TC16 | Gõ `Ctrl+D` ở bất kỳ prompt | "Het du lieu nhap" — lưu và thoát sạch | Xử lý EOF |

### Demo xử lý lỗi file (1 phút)

```sh
echo "xxx" > data/benhvien.csv && ./he_thong_benh_vien
```
→ Cảnh báo "Khong doc duoc ... bat dau voi he thong rong", **file gốc được sao lưu thành `data/benhvien.csv.loi`** (không bị ghi đè mất).

Trong menu, chức năng `12` với file có dòng sai (ví dụ trùng mã) → báo **số dòng lỗi** và dữ liệu đang chạy được giữ nguyên.

### Chạy bằng dữ liệu nhập sẵn (dự phòng nếu gõ chậm)

```sh
make scripted
```
Tệp `tests/input_demo.txt` chứa cả những lần nhập sai để chứng minh chương trình hỏi lại; kết thúc bằng `0`.

## 3. Kiểm thử tự động & bộ nhớ (2 phút)

```sh
make test       # 65/65 đạt
make asan       # AddressSanitizer + UBSan: không lỗi
make valgrind   # "All heap blocks were freed -- no leaks are possible", 0 errors
```

| Nhóm | Mã | Số ca | Nội dung chính |
| --- | --- | --- | --- |
| Queue vòng | Q01–Q07 | 7 | rỗng, FIFO, đầy, **wrap-around**, xóa giữa, chỉ số biên |
| Priority Queue | P01–P06 | 6 | thứ tự ưu tiên, FIFO cùng mức, xóa đầu/giữa/cuối, đầy |
| Deque | D01–D04 | 4 | thêm/lấy hai đầu, rỗng rồi thêm lại |
| Nghiệp vụ | H01–H28 | 28 | hợp lệ, trùng mã, ngoài khoảng, tên sai, quá sức chứa, quá 10 phòng, thứ tự gọi, chuyển cấp cứu, hủy, lịch sử tràn 100, tham số rỗng |
| Lưu trữ | F01–F20 | 20 | lưu, nạp, STT, lịch sử, ổn định, 10 kiểu file hỏng, CRLF, giữ nguyên dữ liệu khi nạp lỗi |

## 4. Câu hỏi phản biện thường gặp

**Vì sao `HangDoi` dùng mảng vòng, `HangUuTien` dùng danh sách liên kết?**
Hàng thường chỉ thêm cuối/lấy đầu nên mảng vòng cho O(1) và không cấp phát động. Hàng ưu tiên cần chèn vào giữa theo thứ tự; danh sách liên kết chèn không phải dịch chuyển phần tử.

**Độ phức tạp?**
- `HangDoi`: thêm/lấy/xem đầu O(1); `layTheoMa` O(n).
- `HangUuTien`: lấy đầu O(1); **thêm O(n)** (chèn có thứ tự); `layTheoMa` O(n).
- `HangDoiHaiDau`: thêm/lấy hai đầu O(1); `xemTaiViTri` O(n).
- Tìm bệnh nhân theo mã: O(tổng số bệnh nhân). Gọi tự động: O(số phòng).

**Sao không dùng heap để thêm O(log n)?**
Heap không bảo đảm FIFO cho phần tử cùng mức nếu không thêm khóa phụ, và khó xóa theo mã/duyệt theo thứ tự để hiển thị bảng điện tử. Với tối đa 100 người mỗi hàng, O(n) vẫn nhỏ; ưu tiên tính đúng và dễ giải thích. Hướng mở rộng: heap + khóa (ưu tiên, STT).

**Quan hệ ưu tiên và FIFO được bảo đảm thế nào?**
`BenhNhan::operator<`: so `doUuTien` trước, bằng nhau thì so `soThuTu` (nhỏ hơn = đến trước = "lớn hơn"). Khi chèn, bệnh nhân mới đi qua mọi phần tử "không nhỏ hơn" nó nên đứng sau những người cùng mức đã đến trước.

**Điều gì xảy ra khi hàng đầy?**
`them` trả `false`, hệ thống không đổi trạng thái và báo lý do; không tràn mảng. Lịch sử gọi thì chủ động bỏ phần tử cũ nhất bằng `layDau` (Deque đóng vai trò hàng đệm vòng).

**Vì sao `HeThongBenhVien` xóa constructor sao chép?**
Chứa con trỏ sở hữu (danh sách liên kết); sao chép nông sẽ gây giải phóng hai lần. Xóa sao chép chặn lỗi ngay lúc biên dịch. Mỗi cấu trúc có destructor giải phóng toàn bộ nút (Valgrind: không rò rỉ).

**Vì sao `main` cấp phát hệ thống trên heap?**
Mỗi `HeThongBenhVien` chứa 10 phòng × mảng 100 `BenhNhan` (vài trăm KB). Để trên heap tránh tràn stack trên hệ thống stack nhỏ.

**File CSV bị cắt giữa chừng thì sao?**
Ghi ra `.tmp` rồi đổi tên nên file cũ không hỏng. Khi đọc, mọi dòng được kiểm tra (định dạng, số, khoảng giá trị, trùng mã/STT, số phòng, sức chứa) **trước khi** thay đổi hệ thống, nên nạp lỗi không để lại trạng thái nửa vời.

**Vì sao bắt buộc cấm dấu phẩy trong tên?**
Định dạng CSV tự viết không có cơ chế escape. Chặn từ lúc nhập thay vì để file hỏng sau này. Hướng mở rộng: bọc nháy kép theo RFC 4180.

**Hạn chế đã biết?**
- Chưa chạy đa luồng/nhiều tiến trình cùng ghi một file (không có khóa file).
- Thời gian chờ chỉ là ước tính 10 phút/lượt, không phải thời gian khám thực.
- Chữ xuất không dấu để hiển thị đúng trên mọi terminal; căn cột bảng có thể lệch nếu tên nhập có dấu UTF-8.
- Việc chuyển khoa cấp cứu lần 2 trả THẤT BẠI (đã ở cấp cứu) thay vì "thành công giả".
- Hàm nghiệp vụ trả `bool` + `loiCuoi()` thay vì ném ngoại lệ (tránh dùng ngoại lệ cho luồng bình thường); ngoại lệ dành cho I/O và dữ liệu hỏng.

## 5. Phân công & checklist "ai cũng hiểu code"

Mỗi thành viên nên tự giải thích được ít nhất một khối sau và chạy thử được `make test`:

| Khối | Câu hỏi tự kiểm tra |
| --- | --- |
| `HangDoi.h` | Vẽ trạng thái mảng vòng sau khi lấy 30 rồi thêm 30 (test Q04). |
| `HangUuTien.h` | Vì sao điều kiện chèn là `!(i->tiep->du < bn)`? |
| `HangDoiHaiDau.h` | Khi lấy phần tử cuối duy nhất, `dau` và `cuoi` đổi thế nào? |
| `HeThong_GoiSo.cpp` | Gọi tự động chọn BN thế nào khi có cấp cứu ở phòng khác? |
| `HeThong_ThongKe.cpp` | Vì sao BN thường phải cộng thêm số người ưu tiên đứng trước? |
| `HeThong_LuuTru.cpp` | Vì sao phải kiểm tra xong toàn bộ file rồi mới `datLai()`? |
| `main.cpp` | Luồng xử lý khi người dùng nhập chữ vào ô số? Khi gặp EOF? |

