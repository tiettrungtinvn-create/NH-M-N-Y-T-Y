@echo off
setlocal
cd /d "%~dp0"
title CareQueue UTC2
set "CORE=src\HeThong_ChuyenKhoa.cpp src\HeThong_GoiSo.cpp src\HeThong_LuuTru.cpp src\HeThong_ThongKe.cpp src\HeThong_TiepNha.cpp"
set "COMPILER="
where g++ >nul 2>nul
if not errorlevel 1 set "COMPILER=gpp"
if defined COMPILER goto have
where cl >nul 2>nul
if not errorlevel 1 set "COMPILER=msvc"
:have
if exist he_thong_benh_vien.exe goto run
if not defined COMPILER goto nocompiler
echo Dang bien dich, vui long cho...
if "%COMPILER%"=="gpp" goto build_gpp
cl /nologo /EHsc /std:c++17 /O2 /Iinclude src\main.cpp %CORE% /Fe:he_thong_benh_vien.exe
goto built
:build_gpp
g++ -std=c++17 -O2 -Iinclude src\main.cpp %CORE% -o he_thong_benh_vien.exe
:built
if not exist he_thong_benh_vien.exe goto fail
:run
chcp 65001 >nul
he_thong_benh_vien.exe %*
goto end

:nocompiler
echo.
echo [LOI] May chua co trinh bien dich C++ (khong tim thay g++ hoac cl).
echo.
echo Cach nhanh nhat:
echo   1. Tai w64devkit (file zip, khong can cai dat):
echo      https://github.com/skeeto/w64devkit/releases
echo   2. Giai nen, chay w64devkit.exe de mo terminal.
echo   3. Trong terminal do, go:  cd /duong/dan/toi/CareQueue_CPP   roi   ./run.sh
echo.
echo Hoac cai Visual Studio Build Tools va mo "Developer Command Prompt".
goto end

:fail
echo.
echo [LOI] Bien dich that bai - xem thong bao phia tren.

:end
echo.
pause
