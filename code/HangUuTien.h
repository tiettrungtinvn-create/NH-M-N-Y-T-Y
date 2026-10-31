#ifndef HANGUUTIEN_H
#define HANGUUTIEN_H

#include "BenhNhan.h"

class HangUuTien {
private:
	struct Nut {
		BenhNhan du;
		Nut* tiep;
		Nut(const BenhNhan& x) : du(x), tiep(NULL) {}
	};
	Nut* dau;
	int dem;

public:
	HangUuTien() : dau(NULL), dem(0) {}
	~HangUuTien() {
		while (dau != NULL) {
			Nut* t = dau;
			dau = dau->tiep;
			delete t;
		}
	}
	HangUuTien(const HangUuTien&) = delete;
	HangUuTien& operator=(const HangUuTien&) = delete;

	int kichThuoc() const { return dem; }
	bool laRong() const { return dem == 0; }

	bool them(const BenhNhan& bn) {
		if (dem == MAX_HANG_DOI) return false;
		Nut* p = new Nut(bn);
		if (dau == NULL || dau->du < bn) {   
			p->tiep = dau;
			dau = p;
		} else {
			Nut* i = dau;
			while (i->tiep != NULL && !(i->tiep->du < bn)) i = i->tiep;
			p->tiep = i->tiep;
			i->tiep = p;
		}
		dem++;
		return true;
	}

	bool lay(BenhNhan& bn) {
		if (dau == NULL) return false;
		Nut* t = dau;
		bn = t->du;
		dau = dau->tiep;
		delete t;
		dem--;
		return true;
	}

	bool xemDau(BenhNhan& bn) const {
		if (dau == NULL) return false;
		bn = dau->du;
		return true;
	}

	bool xemTaiViTri(int i, BenhNhan& bn) const {
		if (i < 0 || i >= dem) return false;
		Nut* p = dau;
		for (int k = 0; k < i; k++) p = p->tiep;
		bn = p->du;
		return true;
	}

	bool layTheoMa(int ma, BenhNhan& bn) {
		Nut* truoc = NULL;
		for (Nut* i = dau; i != NULL; truoc = i, i = i->tiep) {
			if (i->du.maBenhNhan == ma) {
				bn = i->du;
				if (truoc == NULL) dau = i->tiep;
				else truoc->tiep = i->tiep;
				delete i;
				dem--;
				return true;
			}
		}
		return false;
	}
};

#endif
