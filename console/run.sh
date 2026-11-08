#!/usr/bin/env bash
# Chay CareQueue tren terminal (Linux / macOS / Git Bash / WSL).
# Cach dung:  ./run.sh            -> menu tuong tac
#             ./run.sh --demo     -> kich ban demo
#             ./run.sh test       -> chay bo kiem thu
set -e
cd "$(dirname "$0")"
CXX_BIN="${CXX:-}"
if [ -z "$CXX_BIN" ]; then
  for c in g++ clang++ c++; do command -v "$c" >/dev/null 2>&1 && CXX_BIN="$c" && break; done
fi
if [ -z "$CXX_BIN" ]; then
  echo "Khong tim thay trinh bien dich C++. Cai g++ (Ubuntu: sudo apt install g++ | macOS: xcode-select --install)." >&2
  exit 1
fi
FLAGS="-std=c++17 -O2 -Wall -Wextra -Iinclude"
if [ "$1" = "test" ]; then
  "$CXX_BIN" $FLAGS tests/test_all.cpp src/HeThong_*.cpp -o run_tests && exec ./run_tests
fi
if [ ! -x he_thong_benh_vien ] || [ -n "$(find src include -newer he_thong_benh_vien -type f 2>/dev/null | head -1)" ]; then
  echo "Dang bien dich..."
  "$CXX_BIN" $FLAGS src/main.cpp src/HeThong_*.cpp -o he_thong_benh_vien
fi
exec ./he_thong_benh_vien "$@"
