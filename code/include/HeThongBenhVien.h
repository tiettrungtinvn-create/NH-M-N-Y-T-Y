#ifndef HE_THONG_BENH_VIEN_H
#define HE_THONG_BENH_VIEN_H
#include <iostream>
#include <string>
using namespace std;
struct BenhNhan {
    int maBenhNhan;
    string ten;
    string phongKham;
    int doUuTien;
    int soThuTu;
};
class HeThongBenhVien {
private:
    BenhNhan* danhSach;
    int sucChua;
    int soLuong;
    int sttTuDong;
    void sapXepUuTien();
public:
    HeThongBenhVien(int sc);
    ~HeThongBenhVien();
    void tiepNhanVaPhanLuong(int ma, string ten, string phong, int doUuTien);
    void chuyenKhoaCapCuuUuTien(int maBN);
    void goiSoTuDong();
    void uocTinhThoiGianCho(int maBN);
    void hienThiBangDienTu();
    void thongKePhongKham(string tenPhong);
    bool laRong();
};
#endif
