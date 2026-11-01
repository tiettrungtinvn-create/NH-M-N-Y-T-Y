// ============================================================================
// PHẦN 5/10 - HeThongBenhVien.h : KHAI BÁO LỚP HỆ THỐNG BỆNH VIỆN
// ----------------------------------------------------------------------------
// Nhiệm vụ:
//   - Khai báo struct Phong: tên phòng + hangCapCuu (HangUuTien) + hangThuong (HangDoi).
//   - Khai báo lớp HeThongBenhVien: dữ liệu (phong[], soPhong, sucChua, soLuong,
//     sttTuDong, lichSuGoi) và chữ ký các hàm. Các hàm được cài trong 4 file .cpp:
//        HeThong_TiepNhan.cpp, HeThong_ChuyenKhoa.cpp, HeThong_GoiSo.cpp, HeThong_ThongKe.cpp
//   - Cấm sao chép đối tượng (các hàng đợi quản lý bộ nhớ động).
// Phụ thuộc: BenhNhan.h, HangDoi.h, HangDoiHaiDau.h, HangUuTien.h
// ============================================================================
#ifndef HETHONGBENHVIEN_H
#define HETHONGBENHVIEN_H

#include "BenhNhan.h"
#include "HangDoi.h"
#include "HangDoiHaiDau.h"
#include "HangUuTien.h"

struct Phong {
	string ten;
	HangUuTien hangCapCuu;   // bệnh nhân ưu tiên (doUuTien >= 2)
	HangDoi hangThuong;      // bệnh nhân thường (doUuTien == 1), FIFO
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
	void luuLichSuGoi(const BenhNhan &bn);

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
	void uocTinhThoiGianCho(int maBN);
	void hienThiBangDienTu();
	void thongKePhongKham(string tenPhong);
	void hienThiLichSuGoi();
	bool laRong();
	int kichThuoc();
};

#endif
