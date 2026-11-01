CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRC      = $(wildcard *.cpp)
HDR      = $(wildcard *.h)

he_thong_benh_vien: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o he_thong_benh_vien

run: he_thong_benh_vien
	./he_thong_benh_vien

demo: he_thong_benh_vien
	./he_thong_benh_vien --demo

clean:
	rm -f he_thong_benh_vien

.PHONY: run demo clean
