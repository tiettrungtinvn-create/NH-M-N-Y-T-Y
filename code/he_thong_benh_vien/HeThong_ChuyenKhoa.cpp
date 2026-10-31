// ============================================================================
// PHẦN 7/10 - HeThong_ChuyenKhoa.cpp : CHUYỂN KHOA CẤP CỨU ƯU TIÊN
// ----------------------------------------------------------------------------
// Nhiệm vụ: cài chuyenKhoaCapCuuUuTien(maBN)
//   - Tìm bệnh nhân; nếu đã ở "Khoa Cap Cuu" với độ ưu tiên 999 thì coi như xong.
//   - Tạo (nếu chưa có) phòng "Khoa Cap Cuu", kiểm tra còn chỗ rồi gỡ bệnh nhân khỏi
//     hàng cũ (layTheoMa), đổi phòng và độ ưu tiên thành 999, thêm vào hangCapCuu.
//   - In thông báo khẩn. Số lượng bệnh nhân trong hệ thống không đổi.
// Kiểm thử: chuyển người ở hàng thường / hàng ưu tiên; chuyển hai lần; mã không tồn tại.
// Phụ thuộc: HeThongBenhVien.h
// ============================================================================
#include "HeThongBenhVien.h"

bool HeThongBenhVien::chuyenKhoaCapCuuUuTien(int maBN) {
    BenhNhan bn;
    int viTriCu;
    bool capCuuCu;
    if (!timBenhNhan(maBN, bn, viTriCu, capCuuCu)) return false;
    if (bn.phongKham == "Khoa Cap Cuu" && bn.doUuTien == 999) return true;
    int viTriMoi = taoPhong("Khoa Cap Cuu");
    if (viTriMoi < 0) return false;
    if (phong[viTriMoi].hangCapCuu.kichThuoc() == MAX_HANG_DOI &&
        !(viTriCu == viTriMoi && capCuuCu)) return false;

    if (capCuuCu) phong[viTriCu].hangCapCuu.layTheoMa(maBN, bn);
    else phong[viTriCu].hangThuong.layTheoMa(maBN, bn);
    bn.phongKham = "Khoa Cap Cuu";
    bn.doUuTien = 999;
    phong[viTriMoi].hangCapCuu.them(bn);
    cout << "[THONG BAO KHAN] Da chuyen BN " << maBN << " sang Khoa Cap Cuu!\n";
    return true;
}
