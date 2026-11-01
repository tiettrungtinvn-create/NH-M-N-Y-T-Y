#ifndef HANGDOIHAIDAU_H
#define HANGDOIHAIDAU_H

#include "BenhNhan.h"

class HangDoiHaiDau {
private:
	struct Nut {
		BenhNhan du;
		Nut* truoc;
		Nut* sau;
		Nut(const BenhNhan& x) : du(x), truoc(NULL), sau(NULL) {}
	};
	Nut* dau;
	Nut* cuoi;
	int dem;

public:
	HangDoiHaiDau() : dau(NULL), cuoi(NULL), dem(0) {}
	~HangDoiHaiDau() {
		while (dau != NULL) {
			Nut* t = dau;
			dau = dau->sau;
			delete t;
		}
	}
	HangDoiHaiDau(const HangDoiHaiDau&) = delete;
	HangDoiHaiDau& operator=(const HangDoiHaiDau&) = delete;

	int kichThuoc() const { return dem; }
	bool laRong() const { return dem == 0; }

	bool themDau(const BenhNhan& bn) {
		Nut* p = new Nut(bn);
		if (dem == 0) {
			dau = cuoi = p;
		} else {
			p->sau = dau;
			dau->truoc = p;
			dau = p;
		}
		dem++;
		return true;
	}

	bool themCuoi(const BenhNhan& bn) {
		Nut* p = new Nut(bn);
		if (dem == 0) {
			dau = cuoi = p;
		} else {
			p->truoc = cuoi;
			cuoi->sau = p;
			cuoi = p;
		}
		dem++;
		return true;
	}

	
