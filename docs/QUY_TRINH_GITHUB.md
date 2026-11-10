# Quy trình GitHub cho nhóm

## Thiết lập một lần

1. Trưởng nhóm tạo repository chính thức và mời từng thành viên bằng đúng tài khoản GitHub của họ.
2. Đặt nhánh mặc định `main`, bật yêu cầu pull request trước khi cập nhật `main` nếu quyền repository cho phép.
3. Giữ mã nguồn ở thư mục gốc repository: `README.md`, `.gitignore`, `include/`, `src/`, `tests/`, `data/`, `docs/`; không commit file `.exe`, `run_tests`, CSV dữ liệu phát sinh hoặc mật khẩu.
4. Trong README ở repository, thêm tên nhóm, liên kết Jira và bản PDF báo cáo khi các tài liệu đã được chốt.

## Mỗi thành viên làm một nhiệm vụ

```sh
git clone <URL-repository-cua-nhom>
cd <thu-muc-repository>
git switch -c feature/<ma-task>-<noi-dung-ngan>
# sửa mã và chạy kiểm thử
git add <cac-file-da-sua>
git commit -m "NNT-xx: mô tả thay đổi cụ thể"
git push -u origin feature/<ma-task>-<noi-dung-ngan>
```

Tạo pull request vào `main`, ghi mã Jira, mô tả cách kiểm tra và kết quả. Một bạn khác đọc diff và chạy thử rồi mới hợp nhất. Khi sửa lỗi, commit mới trên cùng nhánh; không chia sẻ tài khoản hoặc ghi tên người khác vào commit.

## Mẫu mô tả pull request

```text
Jira: NNT-xx
Thay đổi: ...
Kiểm tra: ./run.sh test (65/65); ./run.sh --demo
Rủi ro/giới hạn: ...
Người kiểm tra: ...
```

## Minh chứng YC01

- URL trang repository và ảnh cây mã nguồn đầy đủ.
- Trang Commits hiển thị commit thực tế của từng thành viên, thông điệp gắn nhiệm vụ.
- Danh sách pull request và review; liên kết từ Jira sang PR.
- README hướng dẫn cài đặt/biên dịch/chạy thử.

Không tạo commit giả chỉ để đủ số lượng hoặc đổi tác giả của người khác. Nếu nhiều bạn cùng lập trình tại một máy, mỗi người phải cấu hình `git user.name` và `git user.email` của chính mình trước khi commit.

