#ifndef BENHNHAN_H
#define BENHNHAN_H

#include <iostream>
#include <string>
using namespace std;

const int MAX_HANG_DOI = 100;      
const int SO_PHONG_TOI_DA = 10;    
const int PHUT_MOI_LUOT = 10;      

struct BenhNhan {
	int maBenhNhan;
	string ten;
	string phongKham;
	int doUuTien;
	int soThuTu;

	BenhNhan() : maBenhNhan(0), ten(""), phongKham(""), doUuTien(0), soThuTu(0) {}

	bool operator<(const BenhNhan& o) const {
		if (doUuTien != o.doUuTien) return doUuTien < o.doUuTien;
		return soThuTu > o.soThuTu;
	}
	bool operator>(const BenhNhan& o) const { return o < *this; }
	bool operator==(const BenhNhan& o) const { return maBenhNhan == o.maBenhNhan; }
};

inline ostream& operator<<(ostream& os, const BenhNhan& bn) {
	os << "STT " << bn.soThuTu << " | Ma " << bn.maBenhNhan << " | " << bn.ten
	   << " | " << bn.phongKham << " | uu tien " << bn.doUuTien;
	return os;
}

#endif
