# Kế hoạch Jira và WBS CareQueue

**Dự án hiện có:** NNT tại `nguyentienthanh11207.atlassian.net`. Năm Epic đã tồn tại là NNT-1 đến NNT-5. Bảng dưới là WBS cho **bản console YC01–YC04**, cần đối chiếu và cập nhật các Task hiện có, không tạo trùng. Ngày bắt đầu/hạn chót do nhóm điền theo lịch môn học.

**Trạng thái:** To do → In Progress → Review → Done. Chỉ chuyển Done khi có liên kết commit/PR và kiểm thử hoặc sản phẩm tài liệu tương ứng.

| Epic | Người phụ trách | Mã WBS | Task cụ thể | Sản phẩm / tiêu chí nghiệm thu | Phụ thuộc |
| --- | --- | --- | --- | --- | --- |
| TV1 — Nền tảng | Nguyễn Tiến Thành | 1.1 | Thiết lập GitHub, README và quy tắc nhánh | Repo chung, README chạy được, mỗi người có quyền truy cập | — |
| TV1 — Nền tảng | Nguyễn Tiến Thành | 1.2 | Cài đặt Queue mảng vòng | `them`, `lay`, `xemDau`, `layTheoMa`; Q01–Q07 đạt | 1.1 |
| TV1 — Nền tảng | Nguyễn Tiến Thành | 1.3 | Tích hợp khung C++ và Makefile | Build console + test thành công trên máy nhóm | 1.2 |
| TV2 — CTDL nâng cao | Hồ Quốc Thịnh | 2.1 | Cài đặt Priority Queue DSLK | Ưu tiên giảm dần, FIFO cùng mức; P01–P06 đạt | 1.3 |
| TV2 — CTDL nâng cao | Hồ Quốc Thịnh | 2.2 | Cài đặt Deque DSLK đôi | Thêm/lấy hai đầu, giải phóng nút; D01–D04 đạt | 1.3 |
| TV2 — CTDL nâng cao | Hồ Quốc Thịnh | 2.3 | Nạp chồng toán tử và điều phối | `<`, `>`, `==`; H11, H14, H25 đạt | 2.1, 2.2 |
| TV3 — Tiếp nhận | Nguyễn Vinh Quang | 3.1 | Kiểm tra đầu vào và tiếp nhận | Mã/tên/phòng/ưu tiên hợp lệ, chặn trùng; H01–H10 đạt | 1.3, 2.1 |
| TV3 — Tiếp nhận | Nguyễn Vinh Quang | 3.2 | Phân luồng phòng khám | Tạo/tìm phòng, giới hạn 10 phòng; H10 đạt | 3.1 |
| TV3 — Tiếp nhận | Nguyễn Vinh Quang | 3.3 | Thời gian chờ và bảng điện tử | 10 phút/lượt theo thứ tự phòng; H17–H19, H27 đạt | 2.3, 3.2 |
| TV4 — Điều phối | Tiết Trung Tín | 4.1 | Gọi số tự động và theo phòng | Ưu tiên trước, FIFO cùng mức; H11–H15 đạt | 2.3, 3.2 |
| TV4 — Điều phối | Tiết Trung Tín | 4.2 | Chuyển khoa cấp cứu | Chuyển sang mức 999, không trùng/hỏng hàng; H14, H16 đạt | 4.1 |
| TV4 — Điều phối | Tiết Trung Tín | 4.3 | Kiểm tra bộ nhớ và lưu/nạp | Chạy test, AddressSanitizer/Valgrind nếu máy hỗ trợ; F01–F20 đạt | 4.1, 4.2 |
| TV5 — Báo cáo | Sum Le | 5.1 | Hoàn thiện báo cáo LaTeX | PDF 40–50 trang, đủ chương, dẫn nguồn thật | 1–4 |
| TV5 — Báo cáo | Sum Le | 5.2 | Làm slide và kịch bản demo | Slide thể hiện CTDL, luồng nghiệp vụ, test; cả nhóm demo được | 1–4 |
| TV5 — Báo cáo | Sum Le | 5.3 | Tổng hợp README và minh chứng | README, liên kết GitHub/Jira, ảnh board, số liệu tiến độ thật | 1–4 |

## Mẫu mô tả cho mỗi Task

```text
Mục tiêu: [một đầu ra kiểm tra được]
Phạm vi file: [đường dẫn]
Tiêu chí Done: [lệnh kiểm thử + kết quả hoặc link tài liệu]
Minh chứng: [URL commit/PR, ảnh hoặc PDF]
Phụ thuộc: [mã issue]
```

## Cập nhật tiến độ

Mỗi tuần, từng người ghi: việc đã xong, việc đang làm, vướng mắc, dự kiến hoàn thành. Trưởng nhóm cập nhật board và chụp/xuất báo cáo khi nộp. Tỷ lệ đóng góp nên tính bằng các Task đã nghiệm thu có trọng số/ước lượng công sức đã thống nhất trước; **không dùng số commit đơn lẻ** để đại diện công sức.

| Thành viên | Task được giao | Task Done | Tỷ lệ hoàn thành | Minh chứng |
| --- | ---: | ---: | ---: | --- |
| TV1 Nguyễn Tiến Thành | 3 | Điền từ Jira | Điền từ Jira | Liên kết issue/PR |
| TV2 Hồ Quốc Thịnh | 3 | Điền từ Jira | Điền từ Jira | Liên kết issue/PR |
| TV3 Nguyễn Vinh Quang | 3 | Điền từ Jira | Điền từ Jira | Liên kết issue/PR |
| TV4 Tiết Trung Tín | 3 | Điền từ Jira | Điền từ Jira | Liên kết issue/PR |
| TV5 Sum Le | 3 | Điền từ Jira | Điền từ Jira | Liên kết issue/PR |

> Jira NNT đã có board và Task cho phiên bản web/SQLite. Giữ nguyên lịch sử đó; cập nhật mô tả hoặc tạo Task **mới chỉ khi thực sự không có Task tương đương** cho bản console. Trạng thái Done cũ không tự chứng minh bản console đã được nghiệm thu.
