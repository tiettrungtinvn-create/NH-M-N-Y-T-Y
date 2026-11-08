"""Tao slide demo co the sua bang PowerPoint. Can: pip install python-pptx."""

from pathlib import Path

from pptx import Presentation
from pptx.dml.color import RGBColor
from pptx.util import Inches, Pt


ROOT = Path(__file__).resolve().parent
prs = Presentation()
prs.slide_width = Inches(13.333)
prs.slide_height = Inches(7.5)
navy = RGBColor(19, 45, 76)
cyan = RGBColor(0, 139, 180)
grey = RGBColor(70, 84, 99)


def add_slide(title, lines, note):
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    heading = slide.shapes.add_textbox(Inches(0.8), Inches(0.55), Inches(11.8), Inches(0.85)).text_frame
    heading.word_wrap = True
    p = heading.paragraphs[0]
    p.text = title
    p.font.name = "Arial"
    p.font.size = Pt(31)
    p.font.bold = True
    p.font.color.rgb = navy
    body = slide.shapes.add_textbox(Inches(0.85), Inches(1.75), Inches(11.4), Inches(4.8)).text_frame
    body.clear()
    body.word_wrap = True
    for i, line in enumerate(lines):
        p = body.paragraphs[0] if i == 0 else body.add_paragraph()
        p.text = line
        p.font.name = "Arial"
        p.font.size = Pt(23)
        p.font.color.rgb = cyan if i == 0 else grey
        p.space_after = Pt(20)
    footer = slide.shapes.add_textbox(Inches(0.85), Inches(7.04), Inches(11.4), Inches(0.25)).text_frame.paragraphs[0]
    footer.text = f"CareQueue UTC2   /   {len(prs.slides):02d}"
    footer.font.name = "Arial"
    footer.font.size = Pt(11)
    footer.font.color.rgb = grey
    slide.notes_slide.notes_text_frame.text = note


add_slide("CareQueue UTC2", ["Hệ thống xếp hàng khám bệnh và cấp cứu", "Bài tập lớn Cấu trúc dữ liệu và Giải thuật", "Nhóm bổ sung tên thành viên và lớp trước khi trình bày"], "Giới thiệu đề tài ĐT02 và phạm vi bản C++ console. Không trình bày như một hệ thống y tế đưa vào vận hành thực.")
add_slide("Bài toán điều phối", ["Khám thường cần FIFO", "Ca ưu tiên được gọi trước theo mức độ khẩn cấp", "Cùng mức ưu tiên vẫn giữ thứ tự đến"], "Nêu ví dụ một người thường đến sớm và một người cấp cứu đến sau. Cần vừa công bằng vừa xử lý khẩn cấp.")
add_slide("Ba cấu trúc dữ liệu", ["Queue mảng vòng: hàng khám thường", "Priority Queue DSLK: hàng khẩn cấp", "Deque DSLK đôi: 100 lượt lịch sử gần nhất"], "Chỉ vào ba file header tương ứng. Giải thích mỗi cấu trúc có vai trò riêng, không dùng thay thế tùy tiện.")
add_slide("Quy tắc so sánh", ["Ưu tiên cao hơn được phục vụ trước", "Nếu bằng mức: STT nhỏ hơn được phục vụ trước", "operator< của BenhNhan giữ quy tắc nhất quán"], "Mở BenhNhan.h, trình bày 2 điều kiện trong operator<. Lấy ví dụ UT5/STT4 đứng trước UT3/STT2.")
add_slide("Tiếp nhận và phân luồng", ["Kiểm tra mã, tên, phòng và mức 1–5", "Mức 1 vào Queue; mức 2–5 vào Priority Queue", "Mã trùng hoặc hàng đầy được báo lỗi"], "Chạy menu tiếp nhận ba bệnh nhân khác mức, sau đó thử mã trùng. Chỉ rõ số thứ tự tự động tăng.")
add_slide("Gọi số và chuyển cấp cứu", ["Gọi toàn viện chọn đầu hàng có thứ tự cao nhất", "Chuyển cấp cứu đặt mức 999 và giữ STT cũ", "Người đã gọi được thêm vào Deque lịch sử"], "Chạy --demo, dừng ở đoạn chuyển bệnh nhân 103. Cho thấy bệnh nhân 103 được gọi trước.")
add_slide("Thời gian chờ dự kiến", ["10 phút nhân số người đứng trước trong cùng phòng", "Người thường tính cả hàng ưu tiên phía trước", "Đây là ước tính theo giả định 10 phút mỗi lượt"], "Lấy ví dụ mức 5 chờ 0 phút, mức 3 chờ 10 phút, thường chờ 20 phút. Không gọi đây là dự báo y tế chính xác.")
add_slide("Lưu và nạp dữ liệu", ["CSV giữ STT, phòng, hàng chờ và lịch sử", "Nạp kiểm tra toàn bộ file trước khi áp dụng", "File hỏng được báo lỗi, dữ liệu đang chạy được giữ"], "Cho xem mẫu ở data/mau_benhvien.csv. Demo lưu rồi chạy lại; nêu giới hạn không chạy đồng thời nhiều tiến trình.")
add_slide("Kiểm thử", ["65/65 ca đạt trên bản đóng gói", "Queue 7; Priority Queue 6; Deque 4", "Nghiệp vụ 28; CSV 20"], "Chạy run.sh test hoặc biên dịch tests/test_all.cpp. Không nhận là đã kiểm thử bộ nhớ trên mọi máy nếu chưa có kết quả.")
add_slide("Tổng kết và phân công", ["Ba cấu trúc phục vụ đúng ba vai trò", "Demo: tiếp nhận, chuyển cấp cứu, gọi, tra cứu, lưu/nạp", "Đối chiếu tiến độ thật trên GitHub và Jira"], "Mời từng thành viên trình bày phần đã làm. Thay thông tin nhóm và chèn link GitHub/Jira thật trước khi nộp.")

prs.save(ROOT / "SLIDE_DEMO.pptx")

