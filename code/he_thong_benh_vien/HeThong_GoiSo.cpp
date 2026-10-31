#include "HeThongBenhVien.h"

bool HeThongBenhVien::goiSoTuDong(string tenPhong) {
    if (laRong()) return false;
    int viTri = -1;
    bool capCuu = false;
    BenhNhan totNhat, ungVien;

    if (!tenPhong.empty()) {
        viTri = timPhong(tenPhong);
        if (viTri < 0) return false;
        capCuu = !phong[viTri].hangCapCuu.laRong();
        if (!capCuu && phong[viTri].hangThuong.laRong()) return false;
    } else {
        for (int i = 0; i < soPhong; i++) {
            if (phong[i].hangCapCuu.xemDau(ungVien) &&
                (viTri < 0 || !capCuu || totNhat < ungVien)) {
                viTri = i;
                capCuu = true;
                totNhat = ungVien;
            }
        }
        if (viTri < 0) {
            for (int i = 0; i < soPhong; i++) {
                if (phong[i].hangThuong.xemDau(ungVien) &&
                    (viTri < 0 || ungVien.soThuTu < totNhat.soThuTu)) {
                    viTri = i;
                    totNhat = ungVien;
                }
            }
        }
    }

    BenhNhan bn;
    if (capCuu) phong[viTri].hangCapCuu.lay(bn);
    else phong[viTri].hangThuong.lay(bn);
    soLuong--;
    if (lichSuGoi.kichThuoc() == MAX_HANG_DOI) {
        BenhNhan cu;
        lichSuGoi.layDau(cu);
    }
    lichSuGoi.themCuoi(bn);
    cout << "[LOA THONG BAO] Moi BN STT " << bn.soThuTu << " - " << bn.ten
         << " vao " << bn.phongKham << "!\n";
    return true;
}

void HeThongBenhVien::hienThiLichSuGoi() {
    cout << "--- LICH SU GOI ---\n";
    BenhNhan bn;
    for (int i = 0; i < lichSuGoi.kichThuoc(); i++) {
        lichSuGoi.xemTaiViTri(i, bn);
        cout << "STT " << bn.soThuTu << " | " << bn.ten << " | " << bn.phongKham << "\n";
    }
}
