# Kết quả kiểm tra gói phát hành

Ngày kiểm tra: 08/11/2026. Môi trường: Windows, `C:\msys64\ucrt64\bin\g++.exe`, chuẩn C++17.

| Mục | Lệnh / cách làm | Kết quả |
| --- | --- | --- |
| Biên dịch bộ test | `g++ -std=c++17 -O2 -Wall -Wextra -Iinclude tests/test_all.cpp src/HeThong_*.cpp -o run_tests.exe` | Thành công, không có cảnh báo biên dịch |
| Chạy bộ test | `run_tests.exe` | `KET QUA: 65/65 dat` |
| Biên dịch ứng dụng | `g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/main.cpp src/HeThong_*.cpp -o he_thong_benh_vien.exe` | Thành công, không có cảnh báo biên dịch |
| Demo tự động | `he_thong_benh_vien.exe --demo` | Chạy xong; bảng điện tử rỗng sau khi gọi hết; lịch sử có các lượt gọi |
| Slide | Mở bằng `python-pptx` | 10 slide có ghi chú |
| LaTeX | Kiểm tra tĩnh 12 đường dẫn `\lstinputlisting` | Các file nguồn đều tồn tại; chưa biên dịch PDF vì máy không có XeLaTeX |

Kết quả này chỉ chứng minh bản mã trong gói tại thời điểm kiểm tra. Nhóm cần chạy lại trên máy trình bày, kiểm tra PDF và lưu ảnh/biên bản thật vào Jira trước khi nộp.
