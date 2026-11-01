#include "HeThongBenhVien.h"

bool HeThongBenhVien::chuyenKhoaCapCuuUuTien(int maBN, string tenKhoa) {
    BenhNhan bn;
    int viTriCu;
    bool capCuuCu;
    if (!timBenhNhan(maBN, bn, viTriCu, capCuuCu)) return false;
    if (bn.phongKham == tenKhoa && bn.doUuTien == 999) return true;
    int viTriMoi = taoPhong(tenKhoa);
    if (viTriMoi < 0) return false;
    if (phong[viTriMoi].hangCapCuu.kichThuoc() == MAX_HANG_DOI &&
        !(viTriCu == viTriMoi && capCuuCu)) return false;

    if (capCuuCu) phong[viTriCu].hangCapCuu.layTheoMa(maBN, bn);
    else phong[viTriCu].hangThuong.layTheoMa(maBN, bn);
    bn.phongKham = tenKhoa;
    bn.doUuTien = 999;
    phong[viTriMoi].hangCapCuu.them(bn);
    cout << "[THONG BAO KHAN] Da chuyen BN " << maBN << " sang Khoa Cap Cuu!\n";
    return true;
}
