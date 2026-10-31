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

int main(int argc, char** argv) {
	if (argc > 1 && string(argv[1]) == "--demo") {
		chayDemo();
		return 0;
	}
	HeThongBenhVien ht(50);
	while (true) {
		cout << "\n===== HE THONG XEP HANG KHAM BENH & CAP CUU =====\n"
		     << " 1. Tiep nhan va phan luong benh nhan\n"
		     << " 2. Chuyen khoa cap cuu uu tien\n"
		     << " 3. Goi so tu dong (toan he thong)\n"
		     << " 4. Goi so theo phong\n"
		     << " 5. Uoc tinh thoi gian cho\n"
		     << " 6. Bang dien tu\n"
		     << " 7. Thong ke phong kham\n"
		     << " 8. Lich su goi so\n"
		     << " 0. Thoat\n";
		int c = readInt("Chon: ", 0, 8);
		if (c == 0) break;
		if (c == 1) {
			int ma = readInt("  Ma benh nhan: ", 1, 1000000);
			string ten = readLine("  Ho ten: ");
			string phong = readLine("  Phong kham: ");
			int dut = readInt("  Do uu tien (1 = thuong ... 5 = khan nhat): ", 1, 5);
			kq("Tiep nhan", ht.tiepNhanVaPhanLuong(ma, ten, phong, dut));
		} else if (c == 2) {
			kq("Chuyen cap cuu", ht.chuyenKhoaCapCuuUuTien(readInt("  Ma benh nhan: ", 1, 1000000)));
		} else if (c == 3) {
			if (!ht.goiSoTuDong()) cout << "  Khong co benh nhan nao dang cho.\n";
		} else if (c == 4) {
			if (!ht.goiSoTuDong(readLine("  Ten phong: "))) cout << "  Khong goi duoc (phong khong co BN hoac khong ton tai).\n";
		} else if (c == 5) {
			ht.uocTinhThoiGianCho(readInt("  Ma benh nhan: ", 1, 1000000));
		} else if (c == 6) {
			ht.hienThiBangDienTu();
		} else if (c == 7) {
			ht.thongKePhongKham(readLine("  Ten phong: "));
		} else if (c == 8) {
			ht.hienThiLichSuGoi();
		}
	}
	cout << "Tam biet!\n";
	return 0;
}
