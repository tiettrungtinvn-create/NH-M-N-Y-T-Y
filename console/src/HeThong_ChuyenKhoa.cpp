#include "HeThongBenhVien.h"

bool HeThongBenhVien::chuyenKhoaCapCuuUuTien(int maBN, string tenKhoa) {
    loiGanNhat.clear();
    BenhNhan bn;
    int viTriCu;
    bool capCuuCu;
    if (!timBenhNhan(maBN, bn, viTriCu, capCuuCu))
        return datLoi("Khong tim thay benh nhan " + to_string(maBN) + " trong hang cho.");
    if (bn.phongKham == tenKhoa && bn.doUuTien == DO_UU_TIEN_CAP_CUU)
        return datLoi("Benh nhan da o " + tenKhoa + " voi uu tien cao nhat.");
    int viTriMoi = taoPhong(tenKhoa);
    if (viTriMoi < 0) return datLoi("Khong tao duoc khoa dich (ten sai hoac da du so phong).");
    if (phong[viTriMoi].hangCapCuu.kichThuoc() == MAX_HANG_DOI &&
        !(viTriCu == viTriMoi && capCuuCu))
        return datLoi("Hang cap cuu cua " + tenKhoa + " da day.");

    if (capCuuCu) phong[viTriCu].hangCapCuu.layTheoMa(maBN, bn);
    else phong[viTriCu].hangThuong.layTheoMa(maBN, bn);
    bn.phongKham = tenKhoa;
    bn.doUuTien = DO_UU_TIEN_CAP_CUU;
    phong[viTriMoi].hangCapCuu.them(bn);
    cout << "[THONG BAO KHAN] Da chuyen BN " << maBN << " sang " << tenKhoa << "!\n";
    return true;
}
