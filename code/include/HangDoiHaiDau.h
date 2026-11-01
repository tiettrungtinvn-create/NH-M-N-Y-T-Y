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
bool layDau(BenhNhan& bn) {
		if (dem == 0) return false;
		Nut* t = dau;
		bn = t->du;
		dau = dau->sau;
		if (dau == NULL) cuoi = NULL;
		else dau->truoc = NULL;
		delete t;
		dem--;
		return true;
	}

	bool layCuoi(BenhNhan& bn) {
		if (dem == 0) return false;
		Nut* t = cuoi;
		bn = t->du;
		cuoi = cuoi->truoc;
		if (cuoi == NULL) dau = NULL;
		else cuoi->sau = NULL;
		delete t;
		dem--;
		return true;
	}

	bool xemTaiViTri(int i, BenhNhan& bn) const {
		if (i < 0 || i >= dem) return false;
		Nut* p = dau;
		for (int k = 0; k < i; k++) p = p->sau;
		bn = p->du;
		return true;
	}
};

#endif

	
