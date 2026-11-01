cmake_minimum_required(VERSION 3.16)
project(CareQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOSPITAL_SOURCES
    code/src/HeThong_TiepNha.cpp
    code/src/HeThong_ChuyenKhoa.cpp
    code/src/HeThong_GoiSo.cpp
    code/src/HeThong_ThongKe.cpp
)

add_executable(he_thong_benh_vien code/src/main.cpp ${HOSPITAL_SOURCES})
add_executable(hospital_web code/src/websever.cpp ${HOSPITAL_SOURCES})
target_include_directories(he_thong_benh_vien PRIVATE code/include)
target_include_directories(hospital_web PRIVATE code/include)
if(WIN32)
    target_link_libraries(hospital_web PRIVATE ws2_32)
endif()
