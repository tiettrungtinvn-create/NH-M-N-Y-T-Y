#ifndef HETHONGBENHVIEN_H
#define HETHONGBENHVIEN_H

#include "BenhNhan.h"
#include "HangDoi.h"
#include "HangDoiHaiDau.h"
#include "HangUuTien.h"

const int DO_UU_TIEN_CAP_CUU = 999;   // muc uu tien cua ca chuyen sang Khoa Cap Cuu
const int DO_DAI_TEN_TOI_DA = 50;

struct Phong {
	string ten;
	HangUuTien hangCapCuu;   // benh nhan uu tien (doUuTien >= 2) - Priority Queue
	HangDoi hangThuong;      // benh nhan thuong (doUuTien == 1) - Queue vong, FIFO
};

class HeThongBenhVien {
private:
	Phong phong[SO_PHONG_TOI_DA];
	int soPhong;
	int sucChua;
	int soLuong;
	int sttTuDong;
	HangDoiHaiDau lichSuGoi;     // Deque: lich su goi so
	string loiGanNhat;           // ly do that bai cua thao tac gan nhat

	int timPhong(string tenPhong) const;
	int taoPhong(string tenPhong);
	bool timBenhNhan(int ma, BenhNhan &bn, int &viTriPhong, bool &capCuu) const;
	static bool tenHopLe(const string &s);
	bool datLoi(const string &msg) { loiGanNhat = msg; return false; }

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
	int soPhongHienCo() const { return soPhong; }
	int sucChuaToiDa() const { return sucChua; }
	const string& loiCuoi() const { return loiGanNhat; }

	// --- Luu tru ben vung (CSV) ---
	// Ghi/doc toan bo trang thai (hang doi, STT, lich su). Loi -> nem std::runtime_error.
	void datLai();                                   // xoa sach he thong
	void luuCSV(const string &duongDan) const;       // ghi atomic (file tam + rename)
	void docCSV(const string &duongDan);             // kiem tra toan bo roi moi nap (khong nap do dang)
};

#endif
