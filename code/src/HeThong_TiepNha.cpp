#include "HeThongBenhVien.h"

HeThongBenhVien::HeThongBenhVien(int sc) {
    soPhong = 0;
    if (sc > 0 && sc <= MAX_HANG_DOI) {
        sucChua = sc;
    } else {
        sucChua = MAX_HANG_DOI;
    }
    soLuong = 0;
    sttTuDong = 0;
}

int HeThongBenhVien::timPhong(string tenPhong) const {
    for (int i = 0; i < soPhong; i++)
        if (phong[i].ten == tenPhong) return i;
    return -1;
}

int HeThongBenhVien::taoPhong(string tenPhong) {
    int viTri = timPhong(tenPhong);
    if (viTri >= 0) return viTri;
    if (tenPhong.empty() || soPhong == SO_PHONG_TOI_DA) return -1;
    phong[soPhong].ten = tenPhong;
    soPhong++;
    return soPhong - 1;
}

bool HeThongBenhVien::timBenhNhan(int ma, BenhNhan &bn, int &viTriPhong, bool &capCuu) const {
    for (int i = 0; i < soPhong; i++) {
        for (int j = 0; j < phong[i].hangCapCuu.kichThuoc(); j++) {
            phong[i].hangCapCuu.xemTaiViTri(j, bn);
            if (bn.maBenhNhan == ma) {
                viTriPhong = i;
                capCuu = true;
                return true;
            }
        }
        for (int j = 0; j < phong[i].hangThuong.kichThuoc(); j++) {
            phong[i].hangThuong.xemTaiViTri(j, bn);
            if (bn.maBenhNhan == ma) {
                viTriPhong = i;
                capCuu = false;
                return true;
            }
        }
    }
    return false;
}

bool HeThongBenhVien::tiepNhanVaPhanLuong(int ma, string ten, string tenPhong, int doUuTien) {
    if (ma <= 0 || ten.empty() || tenPhong.empty() || doUuTien < 1 || doUuTien > 5 || soLuong == sucChua)
        return false;
    BenhNhan daCo;
    int viTriCu;
    bool capCuuCu;
    if (timBenhNhan(ma, daCo, viTriCu, capCuuCu)) return false;
    int viTri = taoPhong(tenPhong);
    if (viTri < 0) return false;

    BenhNhan bn;
    bn.maBenhNhan = ma;
    bn.ten = ten;
    bn.phongKham = tenPhong;
    bn.doUuTien = doUuTien;
    bn.soThuTu = sttTuDong + 1;
    bool daThem;
if (doUuTien == 1) {
        daThem = phong[viTri].hangThuong.them(bn);
    } else {
        daThem = phong[viTri].hangCapCuu.them(bn);
    }
    if (!daThem) return false;
    sttTuDong++;
    soLuong++;
    return true;
}

