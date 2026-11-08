// CareQueue UTC2 - He thong xep hang kham benh & cap cuu (Console)
// Cau truc du lieu: Queue vong (HangDoi), Priority Queue lien ket (HangUuTien),
//                   Deque lien ket doi (HangDoiHaiDau).
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include "HeThongBenhVien.h"

namespace {

const char *FILE_MAC_DINH = "data/benhvien.csv";

// Nem khi het du lieu nhap (Ctrl+D / Ctrl+Z, hoac stdin dong) -> thoat sach se.
struct HetDuLieuNhap {};

void kq(const string &viec, bool ok, const HeThongBenhVien *ht = nullptr) {
	cout << "  -> " << viec << ": " << (ok ? "THANH CONG" : "THAT BAI");
	if (!ok && ht != nullptr && !ht->loiCuoi().empty()) cout << "  (" << ht->loiCuoi() << ")";
	cout << "\n";
}

string catKhoangTrang(const string &s) {
	size_t a = s.find_first_not_of(" \t\r\n");
	if (a == string::npos) return "";
	size_t b = s.find_last_not_of(" \t\r\n");
	return s.substr(a, b - a + 1);
}

string docDong(const string &loiNhac) {
	string s;
	cout << loiNhac;
	if (!getline(cin, s)) throw HetDuLieuNhap();
	return catKhoangTrang(s);
}

// Doc so nguyen trong [lo, hi]; nhap sai thi bao loi va hoi lai.
int docSo(const string &loiNhac, int lo, int hi) {
	while (true) {
		string s = docDong(loiNhac);
		size_t pos = 0;
		bool ok = !s.empty();
		long long v = 0;
		if (ok) {
			try { v = stoll(s, &pos); }
			catch (const exception &) { ok = false; }
		}
		if (ok && pos == s.size() && v >= lo && v <= hi) return (int)v;
		cout << "  ! Gia tri khong hop le - nhap mot so nguyen trong khoang " << lo << ".." << hi << ".\n";
	}
}

string docChuoiKhongRong(const string &loiNhac) {
	while (true) {
		string s = docDong(loiNhac);
		if (!s.empty()) return s;
		cout << "  ! Khong duoc de trong.\n";
	}
}

void luuTuDong(const HeThongBenhVien &ht, const string &file) {
	try {
		ht.luuCSV(file);
	} catch (const exception &e) {
		cout << "  [CANH BAO] Khong luu duoc du lieu: " << e.what() << "\n";
	}
}

// ---------------------------------------------------------------- demo
void chayDemo() {
	HeThongBenhVien ht(50);
	cout << "### KICH BAN DEMO ###\n\n-- 1. Tiep nhan va phan luong --\n";
	kq("BN 101 (thuong, Noi)",      ht.tiepNhanVaPhanLuong(101, "Nguyen Van A", "Noi", 1), &ht);
	kq("BN 102 (uu tien 3, Noi)",   ht.tiepNhanVaPhanLuong(102, "Tran Thi B",   "Noi", 3), &ht);
	kq("BN 103 (thuong, Ngoai)",    ht.tiepNhanVaPhanLuong(103, "Le Van C",     "Ngoai", 1), &ht);
	kq("BN 104 (uu tien 5, Noi)",   ht.tiepNhanVaPhanLuong(104, "Pham Thi D",   "Noi", 5), &ht);
	kq("BN 105 (uu tien 2, Nhi)",   ht.tiepNhanVaPhanLuong(105, "Hoang Van E",  "Nhi", 2), &ht);
	kq("BN 106 (thuong, Noi)",      ht.tiepNhanVaPhanLuong(106, "Do Thi G",     "Noi", 1), &ht);

	cout << "\n-- 2. Cac truong hop loi --\n";
	kq("Trung ma 101",              ht.tiepNhanVaPhanLuong(101, "Trung Ma",     "Noi", 1), &ht);
	kq("Do uu tien 9 khong hop le", ht.tiepNhanVaPhanLuong(107, "Sai Muc",      "Noi", 9), &ht);
	kq("Ten rong",                  ht.tiepNhanVaPhanLuong(108, "",             "Noi", 1), &ht);
	kq("Ten co dau phay",           ht.tiepNhanVaPhanLuong(109, "A,B",          "Noi", 1), &ht);

	cout << "\n";
	ht.hienThiBangDienTu();

	cout << "\n-- 3. Uoc tinh thoi gian cho --\n";
	ht.uocTinhThoiGianCho(104);
	ht.uocTinhThoiGianCho(102);
	ht.uocTinhThoiGianCho(101);
	ht.uocTinhThoiGianCho(106);
	ht.uocTinhThoiGianCho(999);

	cout << "\n-- 4. Chuyen khoa cap cuu uu tien --\n";
	kq("Chuyen BN 103 (Ngoai)",     ht.chuyenKhoaCapCuuUuTien(103), &ht);
	kq("Chuyen BN 103 lan nua",     ht.chuyenKhoaCapCuuUuTien(103), &ht);
	kq("Chuyen BN 999 (khong co)",  ht.chuyenKhoaCapCuuUuTien(999), &ht);
	ht.thongKePhongKham("Khoa Cap Cuu");
	ht.thongKePhongKham("Ngoai");

	cout << "\n-- 5. Luu file / doc lai (tinh ben vung) --\n";
	const string tam = "data/demo_tam.csv";
	try {
		ht.luuCSV(tam);
		cout << "  Da luu " << ht.kichThuoc() << " BN vao " << tam << "\n";
		HeThongBenhVien ht2(50);
		ht2.docCSV(tam);
		cout << "  He thong MOI doc lai file: con " << ht2.kichThuoc() << " BN\n";
		ht2.hienThiBangDienTu();
		remove(tam.c_str());
	} catch (const exception &e) {
		cout << "  [LOI] " << e.what() << "\n";
	}

	cout << "\n-- 6. Goi so tu dong toan he thong (3 luot) --\n";
	for (int i = 0; i < 3; i++) ht.goiSoTuDong();

	cout << "\n-- 7. Goi so rieng phong Nhi --\n";
	ht.goiSoTuDong("Nhi");
	kq("Goi phong Nhi khi da het BN", ht.goiSoTuDong("Nhi"), &ht);
	kq("Goi phong khong ton tai",     ht.goiSoTuDong("Mat"), &ht);

	cout << "\n-- 8. Huy / goi theo ma --\n";
	BenhNhan bn;
	kq("Huy BN 106",                  ht.huyBenhNhan(106), &ht);
	kq("Huy BN 106 lan nua",          ht.huyBenhNhan(106), &ht);
	kq("Goi theo ma 101",             ht.goiTheoMa(101, bn), &ht);

	cout << "\n";
	ht.hienThiBangDienTu();
	cout << "\n";
	ht.hienThiLichSuGoi();
	cout << "\nCon lai trong he thong: " << ht.kichThuoc() << " BN\n";
}

// ---------------------------------------------------------------- menu
void inMenu() {
	cout << "\n==================== CAREQUEUE UTC2 ====================\n"
	     << "  XEP HANG KHAM BENH & CAP CUU\n"
	     << "---------------------------------------------------------\n"
	     << "  1. Tiep nhan va phan luong benh nhan\n"
	     << "  2. Chuyen khoa cap cuu uu tien\n"
	     << "  3. Goi so tu dong (toan he thong)\n"
	     << "  4. Goi so theo phong\n"
	     << "  5. Goi so theo ma benh nhan\n"
	     << "  6. Huy phieu cua benh nhan\n"
	     << "  7. Uoc tinh thoi gian cho\n"
	     << "  8. Bang dien tu (danh sach cho)\n"
	     << "  9. Thong ke phong kham\n"
	     << " 10. Lich su goi so\n"
	     << " 11. Luu du lieu ra file\n"
	     << " 12. Doc du lieu tu file\n"
	     << " 13. Xoa toan bo du lieu\n"
	     << "  0. Thoat\n"
	     << "=========================================================\n";
}

void chayMenu(HeThongBenhVien &ht, const string &file) {
	while (true) {
		inMenu();
		int c = docSo("Chon chuc nang [0-13]: ", 0, 13);
		cout << "\n";
		if (c == 0) break;
		switch (c) {
		case 1: {
			int ma = docSo("  Ma benh nhan (so duong): ", 1, 999999999);
			string ten = docChuoiKhongRong("  Ho ten: ");
			string phong = docChuoiKhongRong("  Phong kham (vd: Noi, Ngoai, Nhi): ");
			int dut = docSo("  Do uu tien (1 = thuong ... 5 = khan nhat): ", 1, 5);
			bool ok = ht.tiepNhanVaPhanLuong(ma, ten, phong, dut);
			kq("Tiep nhan", ok, &ht);
			if (ok) luuTuDong(ht, file);
			break;
		}
		case 2: {
			int ma = docSo("  Ma benh nhan can chuyen: ", 1, 999999999);
			bool ok = ht.chuyenKhoaCapCuuUuTien(ma);
			kq("Chuyen cap cuu", ok, &ht);
			if (ok) luuTuDong(ht, file);
			break;
		}
		case 3: {
			if (ht.goiSoTuDong()) luuTuDong(ht, file);
			else cout << "  " << ht.loiCuoi() << "\n";
			break;
		}
		case 4: {
			string p = docChuoiKhongRong("  Ten phong: ");
			if (ht.goiSoTuDong(p)) luuTuDong(ht, file);
			else cout << "  " << ht.loiCuoi() << "\n";
			break;
		}
		case 5: {
			int ma = docSo("  Ma benh nhan can goi: ", 1, 999999999);
			BenhNhan bn;
			if (ht.goiTheoMa(ma, bn)) {
				cout << "[LOA THONG BAO] Moi BN STT " << bn.soThuTu << " - " << bn.ten
				     << " vao " << bn.phongKham << "!\n";
				luuTuDong(ht, file);
			} else {
				cout << "  " << ht.loiCuoi() << "\n";
			}
			break;
		}
		case 6: {
			int ma = docSo("  Ma benh nhan can huy: ", 1, 999999999);
			bool ok = ht.huyBenhNhan(ma);
			kq("Huy phieu", ok, &ht);
			if (ok) luuTuDong(ht, file);
			break;
		}
		case 7:
			ht.uocTinhThoiGianCho(docSo("  Ma benh nhan: ", 1, 999999999));
			break;
		case 8:
			ht.hienThiBangDienTu();
			break;
		case 9:
			ht.thongKePhongKham(docChuoiKhongRong("  Ten phong: "));
			break;
		case 10:
			ht.hienThiLichSuGoi();
			break;
		case 11:
			try {
				string f = docDong("  File luu [Enter = " + file + "]: ");
				if (f.empty()) f = file;
				ht.luuCSV(f);
				cout << "  Da luu " << ht.kichThuoc() << " BN vao " << f << "\n";
			} catch (const exception &e) {
				cout << "  [LOI] " << e.what() << "\n";
			}
			break;
		case 12:
			try {
				string f = docDong("  File doc [Enter = " + file + "]: ");
				if (f.empty()) f = file;
				ht.docCSV(f);
				cout << "  Da doc " << ht.kichThuoc() << " BN tu " << f << "\n";
				luuTuDong(ht, file);
			} catch (const exception &e) {
				cout << "  [LOI] " << e.what() << " (du lieu hien tai giu nguyen)\n";
			}
			break;
		case 13: {
			string x = docDong("  Xoa het du lieu? Go \"yes\" de xac nhan: ");
			if (x == "yes") {
				ht.datLai();
				luuTuDong(ht, file);
				cout << "  Da xoa toan bo du lieu.\n";
			} else {
				cout << "  Da huy.\n";
			}
			break;
		}
		}
	}
}

void inTroGiup(const char *ten) {
	cout << "Cach dung: " << ten << " [--file <duong-dan.csv>] [--demo] [--help]\n"
	     << "  (khong tham so)   Chay menu tuong tac, tu dong nap/luu " << FILE_MAC_DINH << "\n"
	     << "  --file <path>     Dung file CSV khac de luu tru\n"
	     << "  --demo            Chay kich ban demo tu dong (khong dung file chinh)\n"
	     << "  --help            Hien thi tro giup\n";
}

}  // namespace

int main(int argc, char **argv) {
	string file = FILE_MAC_DINH;
	bool demo = false;
	for (int i = 1; i < argc; i++) {
		string a = argv[i];
		if (a == "--demo") demo = true;
		else if (a == "--help" || a == "-h") { inTroGiup(argv[0]); return 0; }
		else if (a == "--file" && i + 1 < argc) file = argv[++i];
		else { cerr << "Tham so khong hop le: " << a << "\n"; inTroGiup(argv[0]); return 2; }
	}

	try {
		if (demo) {
			chayDemo();
			return 0;
		}

		// HeThongBenhVien lon (~vai tram KB) nen cap phat tren heap, tranh tran stack.
		HeThongBenhVien *ht = new HeThongBenhVien(50);
		try {
			error_code ec;
			if (filesystem::exists(file, ec)) {
				try {
					ht->docCSV(file);
					cout << "Da nap " << ht->kichThuoc() << " benh nhan tu " << file << "\n";
				} catch (const exception &e) {
					string hong = file + ".loi";
					error_code ec2;
					filesystem::copy_file(file, hong, filesystem::copy_options::overwrite_existing, ec2);
					cout << "[CANH BAO] Khong doc duoc " << file << ": " << e.what() << "\n"
					     << "           Bat dau voi he thong rong; ban sao file loi: " << hong << "\n";
				}
			} else {
				cout << "Chua co file du lieu (" << file << ") - bat dau he thong moi.\n";
			}
			chayMenu(*ht, file);
			luuTuDong(*ht, file);
			cout << "Tam biet!\n";
		} catch (const HetDuLieuNhap &) {
			luuTuDong(*ht, file);
			cout << "\n(Het du lieu nhap) Da luu va thoat.\n";
		} catch (...) {
			delete ht;
			throw;
		}
		delete ht;
	} catch (const HetDuLieuNhap &) {
		cout << "\n(Het du lieu nhap)\n";
	} catch (const exception &e) {
		cerr << "Loi nghiem trong: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
