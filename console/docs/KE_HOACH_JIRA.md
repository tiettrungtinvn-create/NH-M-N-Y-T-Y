# Kế hoạch Jira và WBS CareQueue

**Dự án hiện có:** NNT tại `nguyentienthanh11207.atlassian.net`. Năm Epic đã tồn tại là NNT-1 đến NNT-5. Bảng dưới là 15 Task mới cho **bản console YC01–YC04**; các Task web/SQLite cũ được giữ nguyên. Ngày bắt đầu/hạn chót do nhóm điền theo lịch môn học.

**Trạng thái:** To do → In Progress → Review → Done. Chỉ chuyển Done khi có liên kết commit/PR và kiểm thử hoặc sản phẩm tài liệu tương ứng.

**Task chung của bản console đã tạo ngày 08/11/2026:** [YC01 NNT-58](https://nguyentienthanh11207.atlassian.net/browse/NNT-58), [YC02 NNT-59](https://nguyentienthanh11207.atlassian.net/browse/NNT-59), [YC03 NNT-60](https://nguyentienthanh11207.atlassian.net/browse/NNT-60), [YC04 NNT-61](https://nguyentienthanh11207.atlassian.net/browse/NNT-61). Cả 15 Task WBS bên dưới cũng đã tạo dưới 5 Epic cũ và đang To Do để nhóm bổ sung minh chứng thật.

| Epic | Người phụ trách | Mã WBS | Jira | Task cụ thể | Sản phẩm / tiêu chí nghiệm thu | Phụ thuộc |
| --- | --- | --- | --- | --- | --- | --- |
| TV1 — Nền tảng | Nguyễn Tiến Thành | 1.1 | [NNT-62](https://nguyentienthanh11207.atlassian.net/browse/NNT-62) | Thiết lập GitHub, README và quy tắc nhánh | Repo chung, README chạy được, mỗi người có quyền truy cập | — |
| TV1 — Nền tảng | Nguyễn Tiến Thành | 1.2 | [NNT-63](https://nguyentienthanh11207.atlassian.net/browse/NNT-63) | Cài đặt Queue mảng vòng | `them`, `lay`, `xemDau`, `layTheoMa`; Q01–Q07 đạt | 1.1 |
| TV1 — Nền tảng | Nguyễn Tiến Thành | 1.3 | [NNT-64](https://nguyentienthanh11207.atlassian.net/browse/NNT-64) | Tích hợp khung C++ và Makefile | Build console + test thành công trên máy nhóm | 1.2 |
| TV2 — CTDL nâng cao | Hồ Quốc Thịnh | 2.1 | [NNT-65](https://nguyentienthanh11207.atlassian.net/browse/NNT-65) | Cài đặt Priority Queue DSLK | Ưu tiên giảm dần, FIFO cùng mức; P01–P06 đạt | 1.3 |
| TV2 — CTDL nâng cao | Hồ Quốc Thịnh | 2.2 | [NNT-66](https://nguyentienthanh11207.atlassian.net/browse/NNT-66) | Cài đặt Deque DSLK đôi | Thêm/lấy hai đầu, giải phóng nút; D01–D04 đạt | 1.3 |
| TV2 — CTDL nâng cao | Hồ Quốc Thịnh | 2.3 | [NNT-67](https://nguyentienthanh11207.atlassian.net/browse/NNT-67) | Nạp chồng toán tử và điều phối | `<`, `>`, `==`; H11, H14, H25 đạt | 2.1, 2.2 |
| TV3 — Tiếp nhận | Nguyễn Vinh Quang | 3.1 | [NNT-68](https://nguyentienthanh11207.atlassian.net/browse/NNT-68) | Kiểm tra đầu vào và tiếp nhận | Mã/tên/phòng/ưu tiên hợp lệ, chặn trùng; H01–H10 đạt | 1.3, 2.1 |
| TV3 — Tiếp nhận | Nguyễn Vinh Quang | 3.2 | [NNT-69](https://nguyentienthanh11207.atlassian.net/browse/NNT-69) | Phân luồng phòng khám | Tạo/tìm phòng, giới hạn 10 phòng; H10 đạt | 3.1 |
| TV3 — Tiếp nhận | Nguyễn Vinh Quang | 3.3 | [NNT-70](https://nguyentienthanh11207.atlassian.net/browse/NNT-70) | Thời gian chờ và bảng điện tử | 10 phút/lượt theo thứ tự phòng; H17–H19, H27 đạt | 2.3, 3.2 |
| TV4 — Điều phối | Tiết Trung Tín | 4.1 | [NNT-71](https://nguyentienthanh11207.atlassian.net/browse/NNT-71) | Gọi số tự động và theo phòng | Ưu tiên trước, FIFO cùng mức; H11–H15 đạt | 2.3, 3.2 |
| TV4 — Điều phối | Tiết Trung Tín | 4.2 | [NNT-72](https://nguyentienthanh11207.atlassian.net/browse/NNT-72) | Chuyển khoa cấp cứu | Chuyển sang mức 999, không trùng/hỏng hàng; H14, H16 đạt | 4.1 |
| TV4 — Điều phối | Tiết Trung Tín | 4.3 | [NNT-73](https://nguyentienthanh11207.atlassian.net/browse/NNT-73) | Kiểm tra bộ nhớ và lưu/nạp | Chạy test, AddressSanitizer/Valgrind nếu máy hỗ trợ; F01–F20 đạt | 4.1, 4.2 |
| TV5 — Báo cáo | Sum Le | 5.1 | [NNT-74](https://nguyentienthanh11207.atlassian.net/browse/NNT-74) | Hoàn thiện báo cáo LaTeX | PDF 40–50 trang, đủ chương, dẫn nguồn thật | 1–4 |
| TV5 — Báo cáo | Sum Le | 5.2 | [NNT-75](https://nguyentienthanh11207.atlassian.net/browse/NNT-75) | Làm slide và kịch bản demo | Slide thể hiện CTDL, luồng nghiệp vụ, test; cả nhóm demo được | 1–4 |
| TV5 — Báo cáo | Sum Le | 5.3 | [NNT-76](https://nguyentienthanh11207.atlassian.net/browse/NNT-76) | Tổng hợp README và minh chứng | README, liên kết GitHub/Jira, ảnh board, số liệu tiến độ thật | 1–4 |

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

> Jira NNT đã có board và Task cho phiên bản web/SQLite. Lịch sử đó được giữ nguyên. 19 Task mới của bản console đang To Do; trạng thái Done cũ không tự chứng minh bản console đã được nghiệm thu.
