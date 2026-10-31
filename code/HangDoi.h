#ifndef HANGDOI_H
#define HANGDOI_H

#include "BenhNhan.h"

class HangDoi {
private:
	BenhNhan a[MAX_HANG_DOI];
	int dau;
	int dem;

public:
	HangDoi() : dau(0), dem(0) {}

	int kichThuoc() const { return dem; }
	bool laRong() const { return dem == 0; }
	bool laDay() const { return dem == MAX_HANG_DOI; }

	bool them(const BenhNhan& bn) {
		if (laDay()) return false;
		a[(dau + dem) % MAX_HANG_DOI] = bn;
		dem++;
		return true;
	}

	bool lay(BenhNhan& bn) {
		if (laRong()) return false;
		bn = a[dau];
		dau = (dau + 1) % MAX_HANG_DOI;
		dem--;
		return true;
	}

	bool xemDau(BenhNhan& bn) const {
		if (laRong()) return false;
		bn = a[dau];
		return true;
	}

	bool xemTaiViTri(int i, BenhNhan& bn) const {
		if (i < 0 || i >= dem) return false;
		bn = a[(dau + i) % MAX_HANG_DOI];
		return true;
	}

	bool layTheoMa(int ma, BenhNhan& bn) {
		for (int i = 0; i < dem; i++) {
			if (a[(dau + i) % MAX_HANG_DOI].maBenhNhan == ma) {
				bn = a[(dau + i) % MAX_HANG_DOI];
				for (int k = i; k < dem - 1; k++)
					a[(dau + k) % MAX_HANG_DOI] = a[(dau + k + 1) % MAX_HANG_DOI];
				dem--;
				return true;
			}
		}
		return false;
	}
};

#endif
