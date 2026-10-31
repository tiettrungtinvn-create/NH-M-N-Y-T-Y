// ============================================================================
// PHẦN 9/10 - HeThong_ThongKe.cpp : THỜI GIAN CHỜ DỰ KIẾN, BẢNG ĐIỆN TỬ, THỐNG KÊ
// ----------------------------------------------------------------------------
// Nhiệm vụ:
//   - thoiGianChoDuKien(maBN): số người đứng trước * PHUT_MOI_LUOT. Bệnh nhân thường
//     phải chờ cả hàng ưu tiên của phòng; bệnh nhân ưu tiên chỉ chờ người ưu tiên trước mình.
//   - uocTinhThoiGianCho(maBN): in kết quả tra cứu hoặc báo không tìm thấy.
//   - hienThiBangDienTu(): in từng phòng, ưu tiên trước rồi đến hàng thường.
//   - thongKePhongKham(tenPhong): đếm số bệnh nhân đang xếp hàng của phòng.
//   - laRong() / kichThuoc(): trạng thái chung của hệ thống.
// Phụ thuộc: HeThongBenhVien.h
// ============================================================================
#include "HeThongBenhVien.h"

int HeThongBenhVien::thoiGianChoDuKien(int maBN) {
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
        cout << "--- BANG DIEN TU TRONG ---\n";
        return;
    }
    cout << "--- DANH SACH CHO KHAM ---\n";
    BenhNhan bn;
    for (int i = 0; i < soPhong; i++) {
        if (phong[i].hangCapCuu.laRong() && phong[i].hangThuong.laRong()) continue;
        cout << "Phong: " << phong[i].ten << "\n";
        for (int j = 0; j < phong[i].hangCapCuu.kichThuoc(); j++) {
            phong[i].hangCapCuu.xemTaiViTri(j, bn);
            cout << "  [UU TIEN " << bn.doUuTien << "] STT " << bn.soThuTu
                 << " | Ma " << bn.maBenhNhan << " | " << bn.ten << "\n";
        }
        for (int j = 0; j < phong[i].hangThuong.kichThuoc(); j++) {
            phong[i].hangThuong.xemTaiViTri(j, bn);
            cout << "  [THUONG] STT " << bn.soThuTu << " | Ma " << bn.maBenhNhan
                 << " | " << bn.ten << "\n";
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
    cout << "[THONG KE] Phong " << tenPhong << " dang co " << dem << " BN xep hang.\n";
}

bool HeThongBenhVien::laRong() { return soLuong == 0; }
int HeThongBenhVien::kichThuoc() { return soLuong; }

