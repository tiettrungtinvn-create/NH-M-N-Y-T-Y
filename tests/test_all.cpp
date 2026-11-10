// Bo kiem thu tu dong (khong can thu vien ngoai). Chay: make test
#include <cstdio>
#include <fstream>
#include <sstream>
#include "HeThongBenhVien.h"

static int tong = 0, dat = 0;

#define CHECK(id, moTa, cond) do { tong++; if (cond) { dat++; printf("  [PASS] %s %s\n", id, moTa); } \
    else printf("  [FAIL] %s %s   (dong %d)\n", id, moTa, __LINE__); } while (0)

static BenhNhan mk(int ma, int ut, int stt) {
	BenhNhan b; b.maBenhNhan = ma; b.doUuTien = ut; b.soThuTu = stt; b.ten = "BN"; b.phongKham = "P";
	return b;
}

// Chay f() trong khi tat cout (cac ham he thong in thong bao ra man hinh)
template <class F> static void im(F f) {
	ostringstream sink;
	streambuf *cu = cout.rdbuf(sink.rdbuf());
	f();
	cout.rdbuf(cu);
}
static string layCout(void (*f)(HeThongBenhVien&), HeThongBenhVien &h) {
	ostringstream sink;
	streambuf *cu = cout.rdbuf(sink.rdbuf());
	f(h);
	cout.rdbuf(cu);
	return sink.str();
}

static void testHangDoi() {
	puts("\n== HangDoi (Queue mang vong) ==");
	HangDoi q; BenhNhan b;
	CHECK("Q01", "hang moi rong, lay that bai", q.laRong() && !q.lay(b) && !q.xemDau(b));
	for (int i = 1; i <= 3; i++) q.them(mk(i, 1, i));
	q.lay(b);
	CHECK("Q02", "FIFO: lay phan tu vao truoc", b.maBenhNhan == 1 && q.kichThuoc() == 2);
	HangDoi day;
	bool tatCaThem = true;
	for (int i = 1; i <= MAX_HANG_DOI; i++) tatCaThem = tatCaThem && day.them(mk(i, 1, i));
	CHECK("Q03", "them du MAX phan tu, them nua bi tu choi", tatCaThem && day.laDay() && !day.them(mk(999, 1, 999)));
	// vong: lay 30 roi them 30 -> con tro dau quay vong
	for (int i = 0; i < 30; i++) day.lay(b);
	bool vong = true;
	for (int i = 0; i < 30; i++) vong = vong && day.them(mk(1000 + i, 1, 1000 + i));
	day.xemTaiViTri(MAX_HANG_DOI - 1, b);
	CHECK("Q04", "quay vong con tro (wrap-around) dung thu tu", vong && b.maBenhNhan == 1029 && day.kichThuoc() == MAX_HANG_DOI);
	HangDoi t;
	for (int i = 1; i <= 5; i++) t.them(mk(i, 1, i));
	CHECK("Q05", "layTheoMa o giua giu nguyen thu tu con lai", t.layTheoMa(3, b) && t.kichThuoc() == 4);
	t.xemTaiViTri(2, b);
	CHECK("Q06", "sau khi xoa 3, vi tri 2 la ma 4", b.maBenhNhan == 4);
	CHECK("Q07", "layTheoMa ma khong ton tai -> false", !t.layTheoMa(77, b) && !t.xemTaiViTri(9, b) && !t.xemTaiViTri(-1, b));
}

static void testHangUuTien() {
	puts("\n== HangUuTien (Priority Queue danh sach lien ket) ==");
	HangUuTien pq; BenhNhan b;
	CHECK("P01", "hang moi rong", pq.laRong() && !pq.lay(b) && !pq.xemDau(b));
	pq.them(mk(1, 2, 1)); pq.them(mk(2, 5, 2)); pq.them(mk(3, 3, 3)); pq.them(mk(4, 5, 4));
	int th[4]; for (int i = 0; i < 4; i++) { pq.lay(b); th[i] = b.maBenhNhan; }
	CHECK("P02", "lay theo uu tien giam dan, cung muc theo FIFO (2,4,3,1)", th[0] == 2 && th[1] == 4 && th[2] == 3 && th[3] == 1);
	pq.them(mk(10, 3, 1)); pq.them(mk(11, 3, 2)); pq.them(mk(12, 4, 3));
	CHECK("P03", "layTheoMa phan tu giua / dau / cuoi", pq.layTheoMa(10, b) && pq.layTheoMa(12, b) && pq.layTheoMa(11, b) && pq.laRong());
	CHECK("P04", "layTheoMa tren hang rong -> false", !pq.layTheoMa(10, b));
	HangUuTien day; bool ok = true;
	for (int i = 1; i <= MAX_HANG_DOI; i++) ok = ok && day.them(mk(i, 2, i));
	CHECK("P05", "day MAX phan tu thi tu choi them", ok && !day.them(mk(999, 5, 999)) && day.kichThuoc() == MAX_HANG_DOI);
	CHECK("P06", "xemTaiViTri hop le / bien", day.xemTaiViTri(0, b) && b.maBenhNhan == 1 && !day.xemTaiViTri(MAX_HANG_DOI, b));
}

static void testDeque() {
	puts("\n== HangDoiHaiDau (Deque danh sach lien ket doi) ==");
	HangDoiHaiDau d; BenhNhan b;
	CHECK("D01", "deque rong", d.laRong() && !d.layDau(b) && !d.layCuoi(b));
	d.themCuoi(mk(2, 1, 2)); d.themCuoi(mk(3, 1, 3)); d.themDau(mk(1, 1, 1));
	d.xemTaiViTri(0, b); int a0 = b.maBenhNhan; d.xemTaiViTri(2, b); int a2 = b.maBenhNhan;
	CHECK("D02", "themDau/themCuoi cho thu tu 1,2,3", a0 == 1 && a2 == 3 && d.kichThuoc() == 3);
	d.layCuoi(b); int c = b.maBenhNhan; d.layDau(b);
	CHECK("D03", "layCuoi=3, layDau=1", c == 3 && b.maBenhNhan == 1 && d.kichThuoc() == 1);
	d.layCuoi(b); d.themDau(mk(7, 1, 7));
	CHECK("D04", "lay het roi them lai van hoat dong (con tro dau/cuoi)", d.kichThuoc() == 1 && d.layDau(b) && b.maBenhNhan == 7 && d.laRong());
}

static void testHeThong() {
	puts("\n== HeThongBenhVien (nghiep vu) ==");
	{
		HeThongBenhVien h(50);
		CHECK("H01", "tiep nhan hop le", h.tiepNhanVaPhanLuong(1, "A", "Noi", 1));
		CHECK("H02", "trung ma bi tu choi", !h.tiepNhanVaPhanLuong(1, "B", "Noi", 1));
		CHECK("H03", "ma <= 0 bi tu choi", !h.tiepNhanVaPhanLuong(0, "B", "Noi", 1) && !h.tiepNhanVaPhanLuong(-5, "B", "Noi", 1));
		CHECK("H04", "uu tien ngoai 1..5 bi tu choi", !h.tiepNhanVaPhanLuong(2, "B", "Noi", 0) && !h.tiepNhanVaPhanLuong(2, "B", "Noi", 6));
		CHECK("H05", "ten / phong rong hoac toan khoang trang bi tu choi",
		      !h.tiepNhanVaPhanLuong(2, "", "Noi", 1) && !h.tiepNhanVaPhanLuong(2, "B", "", 1) && !h.tiepNhanVaPhanLuong(2, "   ", "Noi", 1));
		CHECK("H06", "ten chua dau phay / xuong dong bi tu choi", !h.tiepNhanVaPhanLuong(2, "A,B", "Noi", 1) && !h.tiepNhanVaPhanLuong(2, "A\nB", "Noi", 1));
		CHECK("H07", "ten qua 50 ky tu bi tu choi", !h.tiepNhanVaPhanLuong(2, string(51, 'x'), "Noi", 1) && h.tiepNhanVaPhanLuong(2, string(50, 'x'), "Noi", 1));
		CHECK("H08", "so luong khong doi khi tiep nhan that bai", h.kichThuoc() == 2);
	}
	{
		HeThongBenhVien h(3);
		h.tiepNhanVaPhanLuong(1, "A", "Noi", 1); h.tiepNhanVaPhanLuong(2, "B", "Noi", 2); h.tiepNhanVaPhanLuong(3, "C", "Noi", 3);
		CHECK("H09", "vuot suc chua he thong bi tu choi", !h.tiepNhanVaPhanLuong(4, "D", "Noi", 1) && h.kichThuoc() == 3 && !h.loiCuoi().empty());
	}
	{
		HeThongBenhVien h(100);
		bool ok = true;
		for (int i = 0; i < SO_PHONG_TOI_DA; i++) ok = ok && h.tiepNhanVaPhanLuong(i + 1, "BN", "P" + to_string(i), 1);
		CHECK("H10", "toi da 10 phong, phong thu 11 bi tu choi", ok && !h.tiepNhanVaPhanLuong(99, "BN", "PX", 1));
	}
	{
		HeThongBenhVien h(50); BenhNhan b;
		h.tiepNhanVaPhanLuong(101, "A", "Noi", 1);
		h.tiepNhanVaPhanLuong(102, "B", "Noi", 3);
		h.tiepNhanVaPhanLuong(103, "C", "Ngoai", 1);
		h.tiepNhanVaPhanLuong(104, "D", "Noi", 5);
		h.tiepNhanVaPhanLuong(105, "E", "Noi", 3);
		h.tiepNhanVaPhanLuong(106, "F", "Noi", 1);
		h.layBenhNhanTiepTheo("Noi", b); int g1 = b.maBenhNhan;
		h.layBenhNhanTiepTheo("Noi", b); int g2 = b.maBenhNhan;
		h.layBenhNhanTiepTheo("Noi", b); int g3 = b.maBenhNhan;
		h.layBenhNhanTiepTheo("Noi", b); int g4 = b.maBenhNhan;
		CHECK("H11", "goi theo phong: UT5, UT3 (FIFO 102 truoc 105), roi thuong FIFO", g1 == 104 && g2 == 102 && g3 == 105 && g4 == 101);
		CHECK("H12", "goi het phong -> that bai co ly do", h.layBenhNhanTiepTheo("Noi", b) && h.layBenhNhanTiepTheo("Noi", b) == false && !h.loiCuoi().empty());
		CHECK("H13", "goi phong khong ton tai -> that bai", !h.layBenhNhanTiepTheo("Mat", b));
	}
	{
		HeThongBenhVien h(50); BenhNhan b;
		h.tiepNhanVaPhanLuong(1, "Thuong", "Noi", 1);   // stt 1
		h.tiepNhanVaPhanLuong(2, "Ngoai", "Ngoai", 1);  // stt 2
		h.tiepNhanVaPhanLuong(3, "Nhi", "Nhi", 2);      // stt 3
		h.tiepNhanVaPhanLuong(4, "Khan", "Noi", 4);     // stt 4
		im([&] { h.chuyenKhoaCapCuuUuTien(2); });
		h.layBenhNhanTiepTheo("", b); int a = b.maBenhNhan;
		h.layBenhNhanTiepTheo("", b); int c = b.maBenhNhan;
		h.layBenhNhanTiepTheo("", b); int d = b.maBenhNhan;
		h.layBenhNhanTiepTheo("", b); int e = b.maBenhNhan;
		CHECK("H14", "goi tu dong toan he: cap cuu(2) -> UT4 -> UT2 -> thuong", a == 2 && c == 4 && d == 3 && e == 1);
		CHECK("H15", "het BN -> goi tu dong that bai", !h.goiSoTuDong() && h.laRong() && h.kichThuoc() == 0);
	}
	{
		HeThongBenhVien h(50);
		h.tiepNhanVaPhanLuong(1, "A", "Noi", 1);
		bool ok1, ok2, ok3, ok4;
		im([&] {
			ok1 = h.chuyenKhoaCapCuuUuTien(1);
			ok2 = h.chuyenKhoaCapCuuUuTien(1);
			ok3 = h.chuyenKhoaCapCuuUuTien(77);
		});
		CHECK("H16", "chuyen khoa cap cuu: lan 1 ok, lan 2 va ma la that bai", ok1 && !ok2 && !ok3);
		ok4 = h.thoiGianChoDuKien(1) == 0;
		CHECK("H17", "BN cap cuu o dau hang -> cho 0 phut", ok4);
	}
	{
		HeThongBenhVien h(50); BenhNhan b;
		h.tiepNhanVaPhanLuong(1, "A", "Noi", 1);
		h.tiepNhanVaPhanLuong(2, "B", "Noi", 1);
		h.tiepNhanVaPhanLuong(3, "C", "Noi", 3);
		CHECK("H18", "thoi gian cho: UT truoc, thuong sau (0,10,20 phut)",
		      h.thoiGianChoDuKien(3) == 0 && h.thoiGianChoDuKien(1) == 10 && h.thoiGianChoDuKien(2) == 20);
		CHECK("H19", "thoi gian cho ma khong ton tai = -1", h.thoiGianChoDuKien(404) == -1);
		CHECK("H20", "huy BN: ok 1 lan, lan sau that bai", h.huyBenhNhan(2) && !h.huyBenhNhan(2) && h.kichThuoc() == 2);
		CHECK("H21", "huy khong ghi vao lich su, goi theo ma thi co",
		      h.goiTheoMa(1, b) && b.maBenhNhan == 1 && !h.goiTheoMa(1, b));
		BenhNhan ls[10];
		CHECK("H22", "lich su goi chi co 1 BN (ma 1)", h.lietKeLichSuGoi(ls, 10) == 1 && ls[0].maBenhNhan == 1);
		CHECK("H28", "lich su voi mang NULL / suc chua khong hop le -> 0", h.lietKeLichSuGoi(NULL, 5) == 0 && h.lietKeLichSuGoi(ls, 0) == 0);
	}
	{
		HeThongBenhVien h(100); BenhNhan b;
		for (int i = 1; i <= 100; i++) h.tiepNhanVaPhanLuong(i, "BN", "Noi", 1);
		for (int i = 1; i <= 100; i++) h.layBenhNhanTiepTheo("Noi", b);
		BenhNhan ls[100];
		int n = h.lietKeLichSuGoi(ls, 100);
		CHECK("H23", "lich su day 100: giu du 100, BN cu nhat con la 1", n == 100 && ls[0].maBenhNhan == 1 && ls[99].maBenhNhan == 100);
		h.tiepNhanVaPhanLuong(101, "BN", "Noi", 1);
		h.layBenhNhanTiepTheo("Noi", b);
		n = h.lietKeLichSuGoi(ls, 100);
		CHECK("H24", "goi them 1 -> lich su van 100, BN 1 bi day ra (Deque layDau)", n == 100 && ls[0].maBenhNhan == 2 && ls[99].maBenhNhan == 101);
	}
	{
		HeThongBenhVien h(50);
		h.tiepNhanVaPhanLuong(1, "A", "Noi", 1);
		h.tiepNhanVaPhanLuong(2, "B", "Noi", 4);
		h.tiepNhanVaPhanLuong(3, "C", "Nhi", 2);
		BenhNhan ds[10];
		int n = h.lietKeThuTuCho(ds, 10);
		CHECK("H25", "lietKeThuTuCho sap xep toan he theo uu tien", n == 3 && ds[0].maBenhNhan == 2 && ds[1].maBenhNhan == 3 && ds[2].maBenhNhan == 1);
		CHECK("H26", "lietKeThuTuCho tham so xau (NULL / 0) an toan", h.lietKeThuTuCho(NULL, 5) == 0 && h.lietKeThuTuCho(ds, 0) == 0 && h.lietKeThuTuCho(ds, 2) == 2);
	}
	{
		HeThongBenhVien h(50);
		h.tiepNhanVaPhanLuong(1, "A", "Noi", 1);
		string s = layCout([](HeThongBenhVien &x) { x.hienThiBangDienTu(); }, h);
		string s2 = layCout([](HeThongBenhVien &x) { x.thongKePhongKham("Khong co"); }, h);
		CHECK("H27", "bang dien tu / thong ke phong la khong crash, co noi dung", s.find("Nguyen") == string::npos && s.find("A") != string::npos && s2.find("khong ton tai") != string::npos);
	}
}

static string docFile(const string &p) {
	ifstream f(p.c_str()); ostringstream s; s << f.rdbuf(); return s.str();
}
static void ghiFile(const string &p, const string &c) {
	ofstream f(p.c_str(), ios::trunc); f << c;
}
static bool nemLoi(HeThongBenhVien &h, const string &p) {
	try { h.docCSV(p); } catch (const runtime_error &) { return true; } catch (...) { return false; }
	return false;
}

static void testLuuTru() {
	puts("\n== Luu / doc CSV (tinh ben vung) ==");
	const string f = "data/test_tmp.csv";
	HeThongBenhVien a(50);
	a.tiepNhanVaPhanLuong(101, "Nguyen Van A", "Noi", 1);
	a.tiepNhanVaPhanLuong(102, "Tran Thi B", "Noi", 3);
	a.tiepNhanVaPhanLuong(103, "Le Van C", "Ngoai", 1);
	a.tiepNhanVaPhanLuong(104, "Pham Thi D", "Noi", 5);
	a.tiepNhanVaPhanLuong(105, "Hoang Van E", "Nhi", 2);
	a.tiepNhanVaPhanLuong(106, "Do Thi G", "Noi", 1);
	im([&] { a.chuyenKhoaCapCuuUuTien(103); a.goiSoTuDong(); a.goiSoTuDong("Nhi"); });
	bool luuOk = true;
	try { a.luuCSV(f); } catch (...) { luuOk = false; }
	CHECK("F01", "luu file thanh cong, khong de lai file .tmp", luuOk && !ifstream((f + ".tmp").c_str()));

	HeThongBenhVien b(50);
	bool docOk = true;
	try { b.docCSV(f); } catch (...) { docOk = false; }
	CHECK("F02", "doc file vao he thong moi thanh cong", docOk && b.kichThuoc() == a.kichThuoc());
	string sa = layCout([](HeThongBenhVien &x) { x.hienThiBangDienTu(); x.hienThiLichSuGoi(); }, a);
	string sb = layCout([](HeThongBenhVien &x) { x.hienThiBangDienTu(); x.hienThiLichSuGoi(); }, b);
	// thu tu phong co the khac vi cap cuu chuyen -> so sanh theo goi so lan luot
	BenhNhan x, y; bool giong = true;
	for (int i = 0; i < 6; i++) {
		bool u, v;
		im([&] { u = a.layBenhNhanTiepTheo("", x); v = b.layBenhNhanTiepTheo("", y); });
		if (u != v || (u && (x.maBenhNhan != y.maBenhNhan || x.soThuTu != y.soThuTu))) giong = false;
	}
	CHECK("F03", "thu tu goi sau khi nap giong het ban goc", giong);
	(void)sa; (void)sb;

	// STT tiep tuc dung sau khi nap
	HeThongBenhVien c(50);
	c.docCSV(f);
	c.tiepNhanVaPhanLuong(200, "Moi", "Noi", 1);
	BenhNhan ds[10];
	int n = c.lietKeThuTuCho(ds, 10), maxStt = 0;
	for (int i = 0; i < n; i++) if (ds[i].soThuTu > maxStt) maxStt = ds[i].soThuTu;
	CHECK("F04", "sau khi nap, STT tiep tuc (BN moi nhan STT 7)", maxStt == 7);
	BenhNhan ls[100];
	CHECK("F05", "lich su goi duoc khoi phuc (2 luot)", c.lietKeLichSuGoi(ls, 100) == 2);

	// roundtrip lap lai khong doi noi dung
	c.luuCSV(f + "2"); HeThongBenhVien d(50); d.docCSV(f + "2"); d.luuCSV(f + "3");
	CHECK("F06", "luu -> doc -> luu cho file giong nhau (on dinh)", docFile(f + "2") == docFile(f + "3"));

	HeThongBenhVien e(50);
	CHECK("F07", "file khong ton tai -> nem runtime_error", nemLoi(e, "data/khong_co.csv"));
	ghiFile(f, "");
	CHECK("F08", "file rong -> nem loi", nemLoi(e, f));
	ghiFile(f, "NOT A CAREQUEUE FILE\n");
	CHECK("F09", "sai tieu de -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nSTT,5\nQ,abc,A,Noi,1,1\n");
	CHECK("F10", "ma khong phai so -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nSTT,5\nQ,1,A,Noi,9,1\n");
	CHECK("F11", "uu tien ngoai khoang -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nSTT,5\nQ,1,A,Noi,1,1\nQ,1,B,Noi,1,2\n");
	CHECK("F12", "trung ma benh nhan -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nSTT,5\nQ,1,A,Noi,1\n");
	CHECK("F13", "thieu truong -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nQ,1,A,Noi,1,1\n");
	CHECK("F14", "thieu dong STT -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nSTT,1\nQ,1,A,Noi,1,5\n");
	CHECK("F15", "STT benh nhan lon hon STT da cap -> nem loi", nemLoi(e, f));
	ghiFile(f, "CAREQUEUE,1\nSTT,1\nX,1,A\n");
	CHECK("F16", "loai ban ghi la -> nem loi", nemLoi(e, f));

	// nap loi khong lam hong du lieu dang co (strong guarantee)
	HeThongBenhVien g(50);
	g.tiepNhanVaPhanLuong(1, "Giu", "Noi", 1);
	ghiFile(f, "CAREQUEUE,1\nSTT,3\nQ,2,A,Noi,1,1\nQ,3,B,Noi,99,2\n");
	bool loi = nemLoi(g, f);
	CHECK("F17", "nap file hong that bai -> du lieu dang co giu nguyen", loi && g.kichThuoc() == 1 && g.thoiGianChoDuKien(1) == 0);

	// Windows CRLF
	ghiFile(f, "CAREQUEUE,1\r\nSTT,1\r\nR,Noi\r\nQ,1,A,Noi,1,1\r\n");
	HeThongBenhVien h(50);
	bool crlf = true; try { h.docCSV(f); } catch (...) { crlf = false; }
	CHECK("F18", "file dinh dang CRLF (Windows) doc duoc", crlf && h.kichThuoc() == 1);

	CHECK("F19", "duong dan rong -> luuCSV nem loi", [] { HeThongBenhVien z; try { z.luuCSV(""); } catch (const runtime_error &) { return true; } return false; }());
	CHECK("F20", "datLai xoa sach he thong", [&] { h.datLai(); return h.laRong() && h.soPhongHienCo() == 0; }());

	for (const char *s : {"", "2", "3"}) remove((f + s).c_str());
}

int main() {
	puts("=== BO KIEM THU CAREQUEUE ===");
	testHangDoi();
	testHangUuTien();
	testDeque();
	testHeThong();
	testLuuTru();
	printf("\nKET QUA: %d/%d dat\n", dat, tong);
	return dat == tong ? 0 : 1;
}
