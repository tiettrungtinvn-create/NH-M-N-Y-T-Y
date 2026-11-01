FROM gcc:12-bookworm AS build
WORKDIR /app
COPY code/ ./code/
RUN g++ -std=c++17 -O2 -Wall -Wextra -Icode/include \
    code/src/websever.cpp code/src/HeThong_TiepNha.cpp \
    code/src/HeThong_ChuyenKhoa.cpp code/src/HeThong_GoiSo.cpp \
    code/src/HeThong_ThongKe.cpp -o /app/hospital_web

FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /app/hospital_web ./hospital_web
COPY wed/ ./wed/
RUN mkdir -p /var/data
ENV HOST=0.0.0.0 PORT=10000 HOSPITAL_DATA=/var/data/hospital-data.txt
EXPOSE 10000
CMD ["/app/hospital_web"]
