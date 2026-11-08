#include <iomanip>
#include "HeThongBenhVien.h"
int HeThongBenhVien::lietKeThuTuCho(BenhNhan danhSach[], int sucChuaMang) const {
    if (danhSach == NULL || sucChuaMang <= 0) return 0;
    BenhNhan tatCa[MAX_HANG_DOI];
    int dem = 0;
    for (int i = 0; i < soPhong; i++) {
        for (int j = 0; j < phong[i].hangCapCuu.kichThuoc(); j++)
            phong[i].hangCapCuu.xemTaiViTri(j, tatCa[dem++]);
        for (int j = 0; j < phong[i].hangThuong.kichThuoc(); j++)
            phong[i].hangThuong.xemTaiViTri(j, tatCa[dem++]);
    }
    for (int i = 0; i < dem; i++) {
        int totNhat = i;
        for (int j = i + 1; j < dem; j++)
            if (tatCa[totNhat] < tatCa[j]) totNhat = j;
        BenhNhan tam = tatCa[i];
        tatCa[i] = tatCa[totNhat];
        tatCa[totNhat] = tam;
    }
    if (dem > sucChuaMang) dem = sucChuaMang;
    for (int i = 0; i < dem; i++) danhSach[i] = tatCa[i];
    return dem;
}
int HeThongBenhVien::thoiGianChoDuKien(int maBN) const {
    BenhNhan bn, dangXet;
    int viTri;
    bool capCuu;
    if (!timBenhNhan(maBN, bn, viTri, capCuu)) return -1;
    int truoc;
    if (capCuu) {
        truoc = 0;
    } else {
        truoc = phong[viTri].hangCapCuu.kichThuoc();
    }
    if (capCuu) {
        for (int i = 0; i < phong[viTri].hangCapCuu.kichThuoc(); i++) {
            phong[viTri].hangCapCuu.xemTaiViTri(i, dangXet);
            if (dangXet.maBenhNhan == maBN) break;
            truoc++;
        }
    } else {
        for (int i = 0; i < phong[viTri].hangThuong.kichThuoc(); i++) {
            phong[viTri].hangThuong.xemTaiViTri(i, dangXet);
            if (dangXet.maBenhNhan == maBN) break;
            truoc++;
        }
    }
    return truoc * PHUT_MOI_LUOT;
}

void HeThongBenhVien::uocTinhThoiGianCho(int maBN) {
    int phut = thoiGianChoDuKien(maBN);
    if (phut < 0) cout << "[LOI] Khong tim thay BN " << maBN << " trong hang cho.\n";
    else cout << "[TRA CUU] BN " << maBN << " can cho khoang " << phut << " phut.\n";
}

void HeThongBenhVien::hienThiBangDienTu() {
    if (laRong()) {
        cout << "--- BANG DIEN TU: TRONG ---\n";
        return;
    }
    cout << "--- BANG DIEN TU (" << soLuong << " benh nhan dang cho) ---\n";
    BenhNhan bn;
    for (int i = 0; i < soPhong; i++) {
        if (phong[i].hangCapCuu.laRong() && phong[i].hangThuong.laRong()) continue;
        cout << "Phong: " << phong[i].ten << "\n";
        cout << "  " << left << setw(10) << "Loai" << setw(6) << "STT" << setw(9) << "Ma"
             << setw(26) << "Ho ten" << "Cho(phut)\n";
        int viTri = 0;
        for (int j = 0; j < phong[i].hangCapCuu.kichThuoc(); j++, viTri++) {
            phong[i].hangCapCuu.xemTaiViTri(j, bn);
            string loai = "UT " + to_string(bn.doUuTien);
            cout << "  " << left << setw(10) << loai << setw(6) << bn.soThuTu << setw(9) << bn.maBenhNhan
                 << setw(26) << bn.ten << viTri * PHUT_MOI_LUOT << "\n";
        }
        for (int j = 0; j < phong[i].hangThuong.kichThuoc(); j++, viTri++) {
            phong[i].hangThuong.xemTaiViTri(j, bn);
            cout << "  " << left << setw(10) << "THUONG" << setw(6) << bn.soThuTu << setw(9) << bn.maBenhNhan
                 << setw(26) << bn.ten << viTri * PHUT_MOI_LUOT << "\n";
        }
    }
}
void HeThongBenhVien::thongKePhongKham(string tenPhong) {
    int viTri = timPhong(tenPhong);
    int dem;
    if (viTri < 0) {
        dem = 0;
    } else {
        dem = phong[viTri].hangCapCuu.kichThuoc() + phong[viTri].hangThuong.kichThuoc();
    }
    if (viTri < 0) {
        cout << "[THONG KE] Phong \"" << tenPhong << "\" khong ton tai (0 BN).\n";
        return;
    }
    cout << "[THONG KE] Phong " << tenPhong << ": " << dem << " BN xep hang ("
         << phong[viTri].hangCapCuu.kichThuoc() << " uu tien, "
         << phong[viTri].hangThuong.kichThuoc() << " thuong).\n";
}

bool HeThongBenhVien::laRong() { return soLuong == 0; }
int HeThongBenhVien::kichThuoc() { return soLuong; }

