// Luu / doc trang thai he thong ra file CSV.
//
// Dinh dang (moi dong mot ban ghi, phan tach bang dau phay):
//   CAREQUEUE,1                      <- dau file + phien ban
//   STT,<so thu tu da cap>
//   R,<ten phong>                    <- phong (giu nguyen thu tu)
//   Q,<ma>,<ten>,<phong>,<uu tien>,<stt>   <- benh nhan dang cho (theo thu tu hang)
//   H,<ma>,<ten>,<phong>,<uu tien>,<stt>   <- lich su goi (cu -> moi)
#include <cstdio>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include "HeThongBenhVien.h"

namespace {

struct BanGhi {
    char loai;       // 'Q' hoac 'H'
    BenhNhan bn;
    int dong;        // so dong trong file (de bao loi)
};

[[noreturn]] void loiDong(int dong, const string &msg) {
    throw runtime_error("Dong " + to_string(dong) + ": " + msg);
}

vector<string> tach(const string &line) {
    vector<string> out;
    string cur;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == ',') { out.push_back(cur); cur.clear(); }
        else cur += line[i];
    }
    out.push_back(cur);
    return out;
}

int docSo(const string &s, int dong, const char *tenTruong) {
    if (s.empty()) loiDong(dong, string("thieu gia tri ") + tenTruong);
    size_t pos = 0;
    long long v;
    try {
        v = stoll(s, &pos);
    } catch (const exception &) {
        loiDong(dong, string(tenTruong) + " khong phai so: \"" + s + "\"");
    }
    if (pos != s.size()) loiDong(dong, string(tenTruong) + " khong phai so: \"" + s + "\"");
    if (v < -2000000000LL || v > 2000000000LL) loiDong(dong, string(tenTruong) + " ngoai pham vi");
    return (int)v;
}

}  // namespace

void HeThongBenhVien::datLai() {
    BenhNhan tam;
    for (int i = 0; i < SO_PHONG_TOI_DA; i++) {
        while (phong[i].hangCapCuu.lay(tam)) {}
        while (phong[i].hangThuong.lay(tam)) {}
        phong[i].ten.clear();
    }
    while (lichSuGoi.layDau(tam)) {}
    soPhong = 0;
    soLuong = 0;
    sttTuDong = 0;
    loiGanNhat.clear();
}

void HeThongBenhVien::luuCSV(const string &duongDan) const {
    if (duongDan.empty()) throw runtime_error("Duong dan file rong.");
    filesystem::path dich(duongDan);
    string tam = duongDan + ".tmp";
    try {
        if (dich.has_parent_path()) filesystem::create_directories(dich.parent_path());
    } catch (const filesystem::filesystem_error &e) {
        throw runtime_error(string("Khong tao duoc thu muc: ") + e.what());
    }
    {
        ofstream f(tam.c_str(), ios::out | ios::trunc);
        if (!f) throw runtime_error("Khong mo duoc file de ghi: " + tam);
        f << "CAREQUEUE,1\n";
        f << "STT," << sttTuDong << "\n";
        for (int i = 0; i < soPhong; i++) f << "R," << phong[i].ten << "\n";
        BenhNhan bn;
        for (int i = 0; i < soPhong; i++) {
            for (int j = 0; j < phong[i].hangCapCuu.kichThuoc(); j++) {
                phong[i].hangCapCuu.xemTaiViTri(j, bn);
                f << "Q," << bn.maBenhNhan << ',' << bn.ten << ',' << bn.phongKham << ','
                  << bn.doUuTien << ',' << bn.soThuTu << "\n";
            }
            for (int j = 0; j < phong[i].hangThuong.kichThuoc(); j++) {
                phong[i].hangThuong.xemTaiViTri(j, bn);
                f << "Q," << bn.maBenhNhan << ',' << bn.ten << ',' << bn.phongKham << ','
                  << bn.doUuTien << ',' << bn.soThuTu << "\n";
            }
        }
        for (int j = 0; j < lichSuGoi.kichThuoc(); j++) {
            lichSuGoi.xemTaiViTri(j, bn);
            f << "H," << bn.maBenhNhan << ',' << bn.ten << ',' << bn.phongKham << ','
              << bn.doUuTien << ',' << bn.soThuTu << "\n";
        }
        f.flush();
        if (!f) {
            f.close();
            remove(tam.c_str());
            throw runtime_error("Ghi file that bai (day dia / mat quyen?): " + tam);
        }
    }
    error_code ec;
    filesystem::rename(tam, dich, ec);
    if (ec) {   // Windows: rename khong ghi de duoc -> xoa roi doi ten lai
        filesystem::remove(dich, ec);
        error_code ec2;
        filesystem::rename(tam, dich, ec2);
        if (ec2) throw runtime_error("Khong thay the duoc file dich: " + duongDan);
    }
}

void HeThongBenhVien::docCSV(const string &duongDan) {
    ifstream f(duongDan.c_str());
    if (!f) throw runtime_error("Khong mo duoc file de doc: " + duongDan);

    // ---- Giai doan 1: doc + kiem tra toan bo, CHUA dong vao he thong ----
    string line;
    int dong = 0;
    if (!getline(f, line)) throw runtime_error("File rong.");
    dong++;
    if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
    if (line != "CAREQUEUE,1") throw runtime_error("Sai dinh dang file (thieu dong \"CAREQUEUE,1\").");

    int stt = -1;
    vector<string> dsPhong;
    vector<BanGhi> dsQ, dsH;
    set<int> maDaCo, sttDaCo;

    while (getline(f, line)) {
        dong++;
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;
        vector<string> t = tach(line);
        const string &loai = t[0];
        if (loai == "STT") {
            if (t.size() != 2) loiDong(dong, "dong STT phai co 2 truong");
            if (stt >= 0) loiDong(dong, "dong STT bi lap");
            stt = docSo(t[1], dong, "STT");
            if (stt < 0) loiDong(dong, "STT am");
        } else if (loai == "R") {
            if (t.size() != 2) loiDong(dong, "dong R phai co 2 truong");
            if (!tenHopLe(t[1])) loiDong(dong, "ten phong khong hop le");
            for (size_t i = 0; i < dsPhong.size(); i++)
                if (dsPhong[i] == t[1]) loiDong(dong, "phong bi lap: " + t[1]);
            dsPhong.push_back(t[1]);
        } else if (loai == "Q" || loai == "H") {
            if (t.size() != 6) loiDong(dong, "dong " + loai + " phai co 6 truong");
            BanGhi g;
            g.loai = loai[0];
            g.dong = dong;
            g.bn.maBenhNhan = docSo(t[1], dong, "ma");
            g.bn.ten = t[2];
            g.bn.phongKham = t[3];
            g.bn.doUuTien = docSo(t[4], dong, "uu tien");
            g.bn.soThuTu = docSo(t[5], dong, "stt");
            if (g.bn.maBenhNhan <= 0) loiDong(dong, "ma benh nhan phai duong");
            if (!tenHopLe(g.bn.ten)) loiDong(dong, "ho ten khong hop le");
            if (!tenHopLe(g.bn.phongKham)) loiDong(dong, "ten phong khong hop le");
            if (!((g.bn.doUuTien >= 1 && g.bn.doUuTien <= 5) || g.bn.doUuTien == DO_UU_TIEN_CAP_CUU))
                loiDong(dong, "do uu tien ngoai 1..5 / 999");
            if (g.bn.soThuTu <= 0) loiDong(dong, "STT phai duong");
            (g.loai == 'Q' ? dsQ : dsH).push_back(g);
        } else {
            loiDong(dong, "loai ban ghi la: \"" + loai + "\"");
        }
    }
    if (f.bad()) throw runtime_error("Loi khi doc file.");
    if (stt < 0) throw runtime_error("File thieu dong STT.");

    // Rang buoc nghiep vu
    vector<string> tatCaPhong = dsPhong;
    auto themPhong = [&](const string &ten) {
        for (size_t i = 0; i < tatCaPhong.size(); i++) if (tatCaPhong[i] == ten) return;
        tatCaPhong.push_back(ten);
    };
    int demHang[SO_PHONG_TOI_DA + 1][2] = {};
    for (size_t i = 0; i < dsQ.size(); i++) {
        const BenhNhan &b = dsQ[i].bn;
        if (!maDaCo.insert(b.maBenhNhan).second) loiDong(dsQ[i].dong, "ma benh nhan bi trung");
        if (!sttDaCo.insert(b.soThuTu).second) loiDong(dsQ[i].dong, "STT bi trung");
        if (b.soThuTu > stt) loiDong(dsQ[i].dong, "STT lon hon STT da cap");
        themPhong(b.phongKham);
    }
    for (size_t i = 0; i < dsH.size(); i++) {
        if (dsH[i].bn.soThuTu > stt) loiDong(dsH[i].dong, "STT lon hon STT da cap");
        themPhong(dsH[i].bn.phongKham);
    }
    if ((int)tatCaPhong.size() > SO_PHONG_TOI_DA) throw runtime_error("Vuot qua so phong toi da.");
    if ((int)dsQ.size() > sucChua) throw runtime_error("So benh nhan trong file vuot suc chua he thong.");
    if ((int)dsH.size() > MAX_HANG_DOI) throw runtime_error("Lich su goi vuot gioi han.");
    for (size_t i = 0; i < dsQ.size(); i++) {
        size_t p = 0;
        while (tatCaPhong[p] != dsQ[i].bn.phongKham) p++;
        int k = (dsQ[i].bn.doUuTien == 1) ? 0 : 1;
        if (++demHang[p][k] > MAX_HANG_DOI) loiDong(dsQ[i].dong, "hang doi cua phong bi qua day");
    }

    // ---- Giai doan 2: da hop le -> nap vao he thong ----
    datLai();
    sttTuDong = stt;
    for (size_t i = 0; i < tatCaPhong.size(); i++) taoPhong(tatCaPhong[i]);
    for (size_t i = 0; i < dsQ.size(); i++) {
        const BenhNhan &b = dsQ[i].bn;
        int vt = timPhong(b.phongKham);
        bool ok = (b.doUuTien == 1) ? phong[vt].hangThuong.them(b) : phong[vt].hangCapCuu.them(b);
        if (!ok) throw logic_error("Loi noi bo khi nap du lieu.");
        soLuong++;
    }
    for (size_t i = 0; i < dsH.size(); i++) lichSuGoi.themCuoi(dsH[i].bn);
}
