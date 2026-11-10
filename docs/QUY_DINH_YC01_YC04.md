# Đối chiếu yêu cầu chung YC01–YC04

Tài liệu này phân biệt **nội dung có trong gói** với **minh chứng phải lấy từ các dịch vụ của nhóm**. Không điền tên người đóng góp hoặc tỷ lệ hoàn thành khi chưa có dữ liệu thực.

| Mã | Đã chuẩn bị trong gói | Minh chứng nhóm cần bổ sung trước khi nộp |
| --- | --- | --- |
| YC01 GitHub | Mã nguồn, `.gitignore`, README và hướng dẫn làm việc ở `docs/QUY_TRINH_GITHUB.md` | URL repository chính thức, lịch sử commit bằng tài khoản riêng của từng thành viên, nhánh/PR đã duyệt |
| YC02 Jira | Bảng WBS, tiêu chí nghiệm thu và mẫu theo dõi ở `docs/KE_HOACH_JIRA.md` | Board Jira thật, người phụ trách đã gán, trạng thái và tiến độ cập nhật hàng tuần, ảnh/chứng cứ xuất từ Jira |
| YC03 LaTeX | Nguồn báo cáo ở `bao_cao.tex`, cấu trúc đủ các phần yêu cầu và phụ lục mã | Tên nhóm, tên giảng viên, số liệu/ảnh thật, tài liệu tham khảo có nguồn, PDF đã biên dịch và kiểm tra độ dài 40–50 trang |
| YC04 C++ và demo | C++ console đầy đủ, dữ liệu mẫu, 65 ca kiểm thử, `docs/KICH_BAN_DEMO.md` | Video/ảnh demo nhóm, trình bày trực tiếp và xác nhận từng thành viên hiểu phần code được giao |

## Kiểm tra nhanh trước khi nộp

1. `./run.sh test` trên Linux/macOS hoặc biên dịch `tests/test_all.cpp` cùng `src/HeThong_*.cpp` trên Windows. Kết quả mong đợi: `65/65 dat`.
2. `./run.sh --demo` hoặc `run.bat --demo`. Kết quả mong đợi: tiếp nhận, chuyển cấp cứu, gọi số, lưu/nạp CSV và lịch sử đều hoàn tất.
3. Biên dịch `bao_cao.tex` bằng XeLaTeX hai lần trong Overleaf; đọc PDF và kiểm tra từng trang, đặc biệt trang bìa, mục lục, bảng, mã nguồn. Bổ sung/thắt gọn nội dung thật để đạt 40–50 trang.
4. Mở GitHub và Jira thật để lấy minh chứng. Không dùng bảng kế hoạch này thay cho lịch sử làm việc thực tế.

> Phạm vi: đây là **ứng dụng C++ chạy terminal**, đúng công cụ YC04 trong ảnh. Gói này không chứa website hoặc GUI; đó là hạng mục mở rộng riêng nếu giảng viên yêu cầu.

