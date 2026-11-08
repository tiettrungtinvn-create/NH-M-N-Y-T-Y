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

// Ten hop le: khong rong, khong chi gom khoang trang, <= 50 ky tu,
// khong chua dau phay / xuong dong (de luu CSV an toan).
bool HeThongBenhVien::tenHopLe(const string &s) {
    if (s.empty() || s.size() > (size_t)DO_DAI_TEN_TOI_DA) return false;
    bool coChu = false;
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == ',' || c == '\n' || c == '\r') return false;
        if (c != ' ' && c != '\t') coChu = true;
    }
    return coChu;
}

int HeThongBenhVien::timPhong(string tenPhong) const {
    for (int i = 0; i < soPhong; i++)
        if (phong[i].ten == tenPhong) return i;
    return -1;
}

int HeThongBenhVien::taoPhong(string tenPhong) {
    int viTri = timPhong(tenPhong);
    if (viTri >= 0) return viTri;
    if (!tenHopLe(tenPhong) || soPhong == SO_PHONG_TOI_DA) return -1;
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
    loiGanNhat.clear();
    if (ma <= 0) return datLoi("Ma benh nhan phai la so duong.");
    if (!tenHopLe(ten)) return datLoi("Ho ten khong hop le (rong, qua 50 ky tu hoac chua dau phay).");
    if (!tenHopLe(tenPhong)) return datLoi("Ten phong khong hop le (rong, qua 50 ky tu hoac chua dau phay).");
    if (doUuTien < 1 || doUuTien > 5) return datLoi("Do uu tien phai nam trong khoang 1..5.");
    if (soLuong == sucChua) return datLoi("He thong da day (" + to_string(sucChua) + " benh nhan).");
    BenhNhan daCo;
    int viTriCu;
    bool capCuuCu;
    if (timBenhNhan(ma, daCo, viTriCu, capCuuCu)) return datLoi("Ma benh nhan " + to_string(ma) + " da ton tai.");
    int viTri = taoPhong(tenPhong);
    if (viTri < 0) return datLoi("Da dat so phong toi da (" + to_string(SO_PHONG_TOI_DA) + ").");

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
    if (!daThem) return datLoi("Hang doi cua phong " + tenPhong + " da day.");
    sttTuDong++;
    soLuong++;
    return true;
}
