#ifndef HETHONGBENHVIEN_H
#define HETHONGBENHVIEN_H

#include "BenhNhan.h"
#include "HangDoi.h"
#include "HangDoiHaiDau.h"
#include "HangUuTien.h"

struct Phong {
	string ten;
	HangUuTien hangCapCuu;   
	HangDoi hangThuong;      
};

class HeThongBenhVien {
private:
	Phong phong[SO_PHONG_TOI_DA];
	int soPhong;
	int sucChua;
	int soLuong;
	int sttTuDong;
	HangDoiHaiDau lichSuGoi;

	int timPhong(string tenPhong) const;
	int taoPhong(string tenPhong);
	bool timBenhNhan(int ma, BenhNhan &bn, int &viTriPhong, bool &capCuu) const;
	

public:
	HeThongBenhVien(int sc = MAX_HANG_DOI);
	HeThongBenhVien(const HeThongBenhVien&) = delete;
	HeThongBenhVien& operator=(const HeThongBenhVien&) = delete;

	bool tiepNhanVaPhanLuong(int ma, string ten, string tenPhong, int doUuTien);
	bool chuyenKhoaCapCuuUuTien(int maBN, string tenKhoa = "Khoa Cap Cuu");
	bool goiSoTuDong(string tenPhong = "");
	bool layBenhNhanTiepTheo(string tenPhong, BenhNhan &bn);
	bool goiTheoMa(int maBN, BenhNhan &bn);
	bool huyBenhNhan(int maBN);
	int thoiGianChoDuKien(int maBN) const;
	int lietKeThuTuCho(BenhNhan danhSach[], int sucChuaMang) const;
	void luuLichSuGoi(const BenhNhan &bn);
	int lietKeLichSuGoi(BenhNhan danhSach[], int sucChuaMang) const;
	void uocTinhThoiGianCho(int maBN);
	void hienThiBangDienTu();
	void thongKePhongKham(string tenPhong);
	void hienThiLichSuGoi();
	bool laRong();
	int kichThuoc();
};

#endif
