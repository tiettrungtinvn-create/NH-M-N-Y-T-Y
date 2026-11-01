#include <limits>
#include <cstdlib>
#include "HeThongBenhVien.h"

static void kq(const string& viec, bool ok) {
	cout << "  -> " << viec << ": " << (ok ? "THANH CONG" : "THAT BAI") << "\n";
}

void chayDemo() {
	HeThongBenhVien ht(50);
	cout << "### KICH BAN DEMO ###\n\n-- Tiep nhan va phan luong --\n";
	kq("BN 101 (thuong, Noi)",      ht.tiepNhanVaPhanLuong(101, "Nguyen Van A", "Noi", 1));
	kq("BN 102 (uu tien 3, Noi)",   ht.tiepNhanVaPhanLuong(102, "Tran Thi B",   "Noi", 3));
	kq("BN 103 (thuong, Ngoai)",    ht.tiepNhanVaPhanLuong(103, "Le Van C",     "Ngoai", 1));
	kq("BN 104 (uu tien 5, Noi)",   ht.tiepNhanVaPhanLuong(104, "Pham Thi D",   "Noi", 5));
	kq("BN 105 (uu tien 2, Nhi)",   ht.tiepNhanVaPhanLuong(105, "Hoang Van E",  "Nhi", 2));
	kq("BN 106 (thuong, Noi)",      ht.tiepNhanVaPhanLuong(106, "Do Thi G",     "Noi", 1));

	cout << "\n-- Cac truong hop loi --\n";
	kq("Trung ma 101",              ht.tiepNhanVaPhanLuong(101, "Trung Ma",     "Noi", 1));
	kq("Do uu tien 9 khong hop le", ht.tiepNhanVaPhanLuong(107, "Sai Muc",      "Noi", 9));
	kq("Ten rong",                  ht.tiepNhanVaPhanLuong(108, "",             "Noi", 1));

	cout << "\n";
	ht.hienThiBangDienTu();

	cout << "\n-- Uoc tinh thoi gian cho --\n";
	ht.uocTinhThoiGianCho(104);   
	ht.uocTinhThoiGianCho(102);   
	ht.uocTinhThoiGianCho(101);   
	ht.uocTinhThoiGianCho(106);   
	ht.uocTinhThoiGianCho(999);   

	cout << "\n-- Chuyen khoa cap cuu uu tien --\n";
	kq("Chuyen BN 103 (Ngoai)",     ht.chuyenKhoaCapCuuUuTien(103));
	kq("Chuyen BN 103 lan nua",     ht.chuyenKhoaCapCuuUuTien(103));
	kq("Chuyen BN 999 (khong co)",  ht.chuyenKhoaCapCuuUuTien(999));
	ht.thongKePhongKham("Khoa Cap Cuu");
	ht.thongKePhongKham("Ngoai");

	cout << "\n-- Goi so tu dong toan he thong (3 luot) --\n";
	for (int i = 0; i < 3; i++) ht.goiSoTuDong();

	cout << "\n-- Goi so rieng phong Nhi --\n";
	ht.goiSoTuDong("Nhi");
	kq("Goi phong Nhi khi da het BN", ht.goiSoTuDong("Nhi"));
	kq("Goi phong khong ton tai",     ht.goiSoTuDong("Mat"));

	cout << "\n";
	ht.hienThiBangDienTu();
	cout << "\n";
	ht.hienThiLichSuGoi();
	cout << "\nCon lai trong he thong: " << ht.kichThuoc() << " BN\n";
}

static int readInt(const string& prompt, int lo, int hi) {
	int v;
	while (true) {
		cout << prompt;
		if (cin >> v && v >= lo && v <= hi) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return v;
		}
		if (cin.eof()) exit(0);
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "  Gia tri khong hop le (" << lo << ".." << hi << ").\n";
	}
}

static string readLine(const string& prompt) {
	string s;
	cout << prompt;
	getline(cin, s);
	if (cin.eof()) exit(0);
	return s;
}

