#include "HeThongBenhVien.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
using Socket = SOCKET;
const Socket INVALID = INVALID_SOCKET;
void closeSocket(Socket s) { closesocket(s); }
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
using Socket = int;
const Socket INVALID = -1;
void closeSocket(Socket s) { close(s); }
#endif

namespace {
const string emergencyRoom = "Cap cuu UTC2";
const vector<string> departments = {"Kham tong quat", "Chan thuong", "Nhi", "Mat",
                                    "Tai mui hong", "Chan doan hinh anh", emergencyRoom};

struct Patient {
    int id = 0;
    int birthYear = 0;
    int triage = 5;                 // Giao diá»‡n: 1 nguy ká»‹ch, 5 thÃ´ng thÆ°á»ng.
    int order = 0;
    bool transferred = false;
    string name, gender, category, symptom, department, status = "Dang cho", calledAt;
};

struct Call {
    int id = 0;
    string department, calledAt;
};

string ticket(int id) {
    ostringstream out;
    out << "BN" << setw(4) << setfill('0') << id;
    return out.str();
}

tm vietnamTime() {
    const auto now = chrono::system_clock::now() + chrono::hours(7);
    const time_t time = chrono::system_clock::to_time_t(now);
    tm local{};
#ifdef _WIN32
    gmtime_s(&local, &time);
#else
    gmtime_r(&time, &local);
#endif
    return local;
}

string nowText() {
    const tm local = vietnamTime();
    ostringstream out;
    out << put_time(&local, "%H:%M:%S %d/%m/%Y");
    return out.str();
}

int currentYear() {
    const tm local = vietnamTime();
    return local.tm_year + 1900;
}

string jsonEscape(const string& input) {
    ostringstream out;
    for (unsigned char c : input) {
        if (c == '"') out << "\\\"";
        else if (c == '\\') out << "\\\\";
        else if (c == '\n') out << "\\n";
        else if (c == '\r') out << "\\r";
        else if (c == '\t') out << "\\t";
        else if (c < 32) {
            const char* hex = "0123456789abcdef";
            out << "\\u00" << hex[c >> 4] << hex[c & 15];
        } else out << static_cast<char>(c);
    }
    return out.str();
}

string lower(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return value;
}

string trim(const string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    return value.substr(first, value.find_last_not_of(" \t\r\n") - first + 1);
}

int parseNumber(const string& value) {
    if (value.empty() || !all_of(value.begin(), value.end(),
        [](unsigned char c) { return isdigit(c) != 0; }))
        throw invalid_argument("Gia tri so khong hop le.");
    return stoi(value);
}

class WebHospital {
    HeThongBenhVien queues;
    vector<Patient> patients;
    vector<Call> calls;
    string path;
    int nextId = 1;
    int nextOrder = 1;

    Patient* find(const string& code) {
        for (auto& p : patients) if (ticket(p.id) == code) return &p;
        return nullptr;
    }
    const Patient* find(const string& code) const {
        for (const auto& p : patients) if (ticket(p.id) == code) return &p;
        return nullptr;
    }

    void save() const {
        const string temporary = path + ".tmp";
        ofstream file(temporary, ios::trunc);
        if (!file) { cerr << "Khong mo duoc file luu du lieu.\n"; exit(1); }
        file << "CAREQUEUE1 " << nextId << ' ' << nextOrder << ' ' << patients.size()
             << ' ' << calls.size() << '\n';
        for (const auto& p : patients)
            file << p.id << ' ' << p.birthYear << ' ' << p.triage << ' ' << p.order << ' '
                 << p.transferred << ' ' << quoted(p.name) << ' ' << quoted(p.gender) << ' '
                 << quoted(p.category) << ' ' << quoted(p.symptom) << ' '
                 << quoted(p.department) << ' ' << quoted(p.status) << ' '
                 << quoted(p.calledAt) << '\n';
        for (const auto& c : calls)
            file << c.id << ' ' << quoted(c.department) << ' ' << quoted(c.calledAt) << '\n';
        file.close();
        if (!file) { cerr << "Ghi du lieu khong thanh cong.\n"; exit(1); }
#ifdef _WIN32
        if (!MoveFileExA(temporary.c_str(), path.c_str(),
                         MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
            { cerr << "Khong thay the duoc file du lieu.\n"; exit(1); }
#else
        if (rename(temporary.c_str(), path.c_str()) != 0)
            { cerr << "Khong thay the duoc file du lieu.\n"; exit(1); }
#endif
    }

    void load() {
        ifstream file(path);
        if (!file) {
            if (filesystem::exists(path)) throw runtime_error("Khong doc duoc file du lieu.");
            return;
        }
        string version;
        size_t count = 0, historyCount = 0;
        if (!(file >> version >> nextId >> nextOrder >> count >> historyCount) ||
            version != "CAREQUEUE1" || count > 100000 || historyCount > MAX_HANG_DOI)
            throw runtime_error("File du lieu khong hop le.");
        for (size_t i = 0; i < count; ++i) {
            Patient p;
            if (!(file >> p.id >> p.birthYear >> p.triage >> p.order >> p.transferred
                  >> quoted(p.name) >> quoted(p.gender) >> quoted(p.category)
                  >> quoted(p.symptom) >> quoted(p.department) >> quoted(p.status)
                  >> quoted(p.calledAt)))
                throw runtime_error("Khong doc duoc ho so benh nhan.");
            patients.push_back(p);
        }
        for (size_t i = 0; i < historyCount; ++i) {
            Call c;
            if (!(file >> c.id >> quoted(c.department) >> quoted(c.calledAt)))
                throw runtime_error("Khong doc duoc lich su goi.");
            calls.push_back(c);
        }
        for (auto it = calls.rbegin(); it != calls.rend(); ++it) {
            const Patient* p = find(ticket(it->id));
            if (p == nullptr) throw runtime_error("Lich su goi khong co ho so benh nhan.");
            BenhNhan bn;
            bn.maBenhNhan = p->id;
            bn.ten = p->name;
            bn.phongKham = it->department;
            bn.doUuTien = 6 - p->triage;
            bn.soThuTu = p->order;
            queues.luuLichSuGoi(bn);
        }
        vector<Patient*> waiting;
        for (auto& p : patients) if (p.status == "Dang cho") waiting.push_back(&p);
        sort(waiting.begin(), waiting.end(),
             [](const Patient* a, const Patient* b) { return a->order < b->order; });
        for (const auto* p : waiting) {
            if (!queues.tiepNhanVaPhanLuong(p->id, p->name, p->department, 6 - p->triage))
                throw runtime_error("Khong khoi phuc duoc hang doi.");
            if (p->transferred && !queues.chuyenKhoaCapCuuUuTien(p->id, emergencyRoom))
                throw runtime_error("Khong khoi phuc duoc hang cap cuu.");
        }
    }

    vector<const Patient*> waitingOrder() const {
        vector<const Patient*> result;
        BenhNhan danhSach[MAX_HANG_DOI];
        const int count = queues.lietKeThuTuCho(danhSach, MAX_HANG_DOI);
        for (int i = 0; i < count; i++) {
            const Patient* p = find(ticket(danhSach[i].maBenhNhan));
            if (p == nullptr || p->status != "Dang cho" || p->department != danhSach[i].phongKham)
                throw runtime_error("Ho so va hang doi khong dong bo.");
            result.push_back(p);
        }
        if (count_if(patients.begin(), patients.end(),
                     [](const Patient& p) { return p.status == "Dang cho"; }) != count)
            throw runtime_error("Ho so va hang doi khong dong bo.");
        return result;
    }

    bool roomBusy(const string& department) const {
        for (const auto& p : patients)
            if (p.department == department && p.status == "Dang kham") return true;
        return false;
    }

    void recordCall(const BenhNhan& called) {
        Patient* p = find(ticket(called.maBenhNhan));
        if (p == nullptr) throw runtime_error("Khong tim thay ho so cua luot goi.");
        p->status = "Dang kham";
        p->calledAt = nowText();
        calls.insert(calls.begin(), {p->id, p->department, p->calledAt});
        if (calls.size() > 100) calls.pop_back();
        save();
    }

public:
    explicit WebHospital(string filePath) : queues(MAX_HANG_DOI), path(filePath) { load(); }

    string registerPatient(const map<string, string>& form);
    string callNext(const string& department) {
        string selectedRoom = department;
        if (!selectedRoom.empty() && roomBusy(selectedRoom))
            throw invalid_argument("Phong dang kham benh nhan khac.");
        if (selectedRoom.empty()) {
            const auto waiting = waitingOrder();
            if (waiting.empty()) throw invalid_argument("Khong co benh nhan dang cho.");
            for (const auto* p : waiting) {
                if (!roomBusy(p->department)) {
                    selectedRoom = p->department;
                    break;
                }
            }
            if (selectedRoom.empty()) throw invalid_argument("Khong co phong trong de goi so.");
        }
        BenhNhan bn;
        if (!queues.layBenhNhanTiepTheo(selectedRoom, bn))
            throw invalid_argument("Khong co benh nhan dang cho.");
        recordCall(bn);
        return ticket(bn.maBenhNhan);
    }
    string callSpecific(const string& code) {
        Patient* p = find(code);
        BenhNhan bn;
        if (p == nullptr || p->status != "Dang cho")
            throw invalid_argument("Khong tim thay benh nhan dang cho.");
        if (roomBusy(p->department)) throw invalid_argument("Phong dang kham benh nhan khac.");
        for (const auto* waiting : waitingOrder()) {
            if (waiting->department == p->department) {
                if (waiting != p) throw invalid_argument("Benh nhan chua den luot goi.");
                break;
            }
        }
        if (!queues.goiTheoMa(p->id, bn))
            throw invalid_argument("Khong tim thay benh nhan dang cho.");
        recordCall(bn);
        return code;
    }
    void complete(const string& code, bool absent) {
        Patient* p = find(code);
        if (p == nullptr || p->status != "Dang kham")
            throw invalid_argument("Khong tim thay benh nhan dang kham.");
        if (absent) p->status = "Vang mat";
        else p->status = "Da kham xong";
        save();
    }
    void cancel(const string& code) {
        Patient* p = find(code);
        if (p == nullptr || p->status != "Dang cho" || !queues.huyBenhNhan(p->id))
            throw invalid_argument("Khong tim thay benh nhan dang cho.");
        p->status = "Da huy";
        save();
    }
    void transferEmergency(const string& code) {
        Patient* p = find(code);
        if (p == nullptr || p->status != "Dang cho" ||
            !queues.chuyenKhoaCapCuuUuTien(p->id, emergencyRoom))
            throw invalid_argument("Khong chuyen duoc benh nhan sang cap cuu.");
        p->department = emergencyRoom;
        p->triage = 1;
        p->transferred = true;
        save();
    }
    void transferDepartment(const string& code, const string& department) {
        Patient* p = find(code);
        if (p == nullptr || p->status != "Dang kham" ||
            std::find(departments.begin(), departments.end(), department) == departments.end() ||
            !queues.tiepNhanVaPhanLuong(p->id, p->name, department, 6 - p->triage))
            throw invalid_argument("Khong chuyen duoc khoa.");
        p->department = department;
        p->status = "Dang cho";
        p->transferred = false;
        p->order = nextOrder++;
        save();
    }
    string stateJson() const;
    string waitingJson() const;
};

string WebHospital::registerPatient(const map<string, string>& form) {
    const auto get = [&](const string& key) -> string {
        const auto it = form.find(key);
        if (it == form.end()) return "";
        return it->second;
    };
    const string name = trim(get("name"));
    const string symptom = trim(get("symptom"));
    const string department = get("department");
    if (name.empty() || symptom.empty()) throw invalid_argument("Nhap ho ten va trieu chung.");
    if (name.size() > 160 || symptom.size() > 1000)
        throw invalid_argument("Ho ten hoac trieu chung qua dai.");
    const string gender = get("gender"), category = get("category");
    if (gender != "male" && gender != "female" && gender != "other" && gender != "unspecified")
        throw invalid_argument("Gioi tinh khong hop le.");
    if (category != "student_bhyt" && category != "employee_bhyt" &&
        category != "other_bhyt" && category != "service" && category != "other")
        throw invalid_argument("Doi tuong khong hop le.");
    if (std::find(departments.begin(), departments.end(), department) == departments.end())
        throw invalid_argument("Khoa khong hop le.");
    const int year = parseNumber(get("birthYear"));
    const int triage = parseNumber(get("triage"));
    if (year < currentYear() - 120 || year > currentYear() || triage < 1 || triage > 5)
        throw invalid_argument("Nam sinh hoac muc do uu tien khong hop le.");
    if (nextId > 1000000) throw invalid_argument("Da het ma benh nhan.");
    const int id = nextId;
    if (!queues.tiepNhanVaPhanLuong(id, name, department, 6 - triage))
        throw invalid_argument("Hang doi day hoac khoa khong hop le.");
    Patient p;
    p.id = nextId++;
    p.birthYear = year;
    p.triage = triage;
    p.order = nextOrder++;
    p.name = name;
    p.gender = gender;
    p.category = category;
    p.symptom = symptom;
    p.department = department;
    patients.push_back(p);
    save();
    return ticket(id);
}

string WebHospital::stateJson() const {
    ostringstream out;
    out << boolalpha;
    auto patientJson = [&](const Patient& p) {
        bool emergency = p.triage <= 2 || p.transferred;
        out << "{\"ticket\":\"" << ticket(p.id) << "\",\"name\":\""
            << jsonEscape(p.name) << "\",\"age\":" << currentYear() - p.birthYear
            << ",\"birthYear\":" << p.birthYear << ",\"gender\":\""
            << jsonEscape(p.gender) << "\",\"category\":\"" << jsonEscape(p.category)
            << "\",\"symptom\":\"" << jsonEscape(p.symptom) << "\",\"department\":\""
            << jsonEscape(p.department) << "\",\"triageLevel\":" << p.triage
            << ",\"status\":\"" << p.status << "\",\"emergency\":"
            << emergency << ",\"waitMinutes\":";
        if (p.status == "Dang cho") {
            int minutes = queues.thoiGianChoDuKien(p.id);
            if (roomBusy(p.department)) minutes += PHUT_MOI_LUOT;
            out << minutes;
        }
        else out << "null";
        out << '}';
    };
    out << "{\"patients\":[";
    for (size_t i = 0; i < patients.size(); ++i) {
        if (i) out << ',';
        patientJson(patients[i]);
    }
    out << "],\"waiting\":[";
    const auto waiting = waitingOrder();
    for (size_t i = 0; i < waiting.size(); ++i) {
        if (i) out << ',';
        patientJson(*waiting[i]);
    }
    out << "],\"history\":[";
    BenhNhan history[MAX_HANG_DOI];
    const int historySize = queues.lietKeLichSuGoi(history, MAX_HANG_DOI);
    if (historySize != static_cast<int>(calls.size()))
        throw runtime_error("Lich su goi va Deque khong dong bo.");
    for (size_t i = 0; i < calls.size(); ++i) {
        if (i) out << ',';
        const auto& c = calls[i];
        const auto& bn = history[i];
        if (c.id != bn.maBenhNhan || c.department != bn.phongKham)
            throw runtime_error("Lich su goi va Deque khong dong bo.");
        out << "{\"ticket\":\"" << ticket(c.id) << "\",\"name\":\""
            << jsonEscape(bn.ten) << "\",\"department\":\""
            << jsonEscape(bn.phongKham) << "\",\"calledAt\":\""
            << jsonEscape(c.calledAt) << "\"}";
    }
    out << "],\"departments\":[";
    for (size_t i = 0; i < departments.size(); ++i) {
        if (i) out << ',';
        out << '"' << jsonEscape(departments[i]) << '"';
    }
    out << "]}";
    return out.str();
}

string WebHospital::waitingJson() const {
    ostringstream out;
    out << boolalpha;
    out << "{\"waiting\":[";
    const auto waiting = waitingOrder();
    for (size_t i = 0; i < waiting.size(); ++i) {
        const auto* p = waiting[i];
        if (i) out << ',';
        bool emergency = p->triage <= 2 || p->transferred;
        out << "{\"ticket\":\"" << ticket(p->id) << "\",\"department\":\""
            << jsonEscape(p->department) << "\",\"emergency\":"
            << emergency << '}';
    }
    out << "],\"active\":[";
    bool first = true;
    for (const auto& p : patients) if (p.status == "Dang kham") {
        if (!first) out << ',';
        first = false;
        out << "{\"ticket\":\"" << ticket(p.id) << "\",\"department\":\""
            << jsonEscape(p.department) << "\"}";
    }
    out << "]}";
    return out.str();
}

struct Request { string method, path, body, authorization, origin, host; };

string urlDecode(const string& input) {
    string result;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '+') result += ' ';
        else if (input[i] == '%' && i + 2 < input.size() &&
                 isxdigit(static_cast<unsigned char>(input[i + 1])) &&
                 isxdigit(static_cast<unsigned char>(input[i + 2]))) {
            result += static_cast<char>(strtol(input.substr(i + 1, 2).c_str(), nullptr, 16));
            i += 2;
        } else result += input[i];
    }
    return result;
}

map<string, string> parseForm(const string& body) {
    map<string, string> form;
    size_t start = 0;
    while (start < body.size()) {
        const size_t end = body.find('&', start);
        string pair;
        if (end == string::npos) pair = body.substr(start);
        else pair = body.substr(start, end - start);
        const size_t equal = pair.find('=');
        if (equal == string::npos) form[urlDecode(pair)] = "";
        else form[urlDecode(pair.substr(0, equal))] = urlDecode(pair.substr(equal + 1));
        if (end == string::npos) break;
        start = end + 1;
    }
    return form;
}

string field(const map<string, string>& form, const string& key) {
    const auto it = form.find(key);
    if (it == form.end()) return "";
    return it->second;
}

string base64(const string& value) {
    const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string result;
    int bits = 0;
    unsigned int buffer = 0;
    for (unsigned char c : value) {
        buffer = (buffer << 8) | c;
        bits += 8;
        while (bits >= 6) {
            bits -= 6;
            result += alphabet[(buffer >> bits) & 63];
        }
        buffer &= (1u << bits) - 1;
    }
    if (bits) result += alphabet[(buffer << (6 - bits)) & 63];
    while (result.size() % 4) result += '=';
    return result;
}

Request readRequest(Socket client) {
    string raw;
    char buffer[4096];
    const auto deadline = chrono::steady_clock::now() + chrono::seconds(5);
    size_t headerEnd = string::npos, length = 0;
    bool parsed = false;
    Request request;
    for (;;) {
        if (chrono::steady_clock::now() >= deadline)
            throw runtime_error("Yeu cau HTTP qua thoi gian cho.");
        const int n = recv(client, buffer, sizeof(buffer), 0);
        if (n <= 0) throw runtime_error("Yeu cau HTTP bi gian doan.");
        raw.append(buffer, n);
        if (raw.size() > 1024 * 1024) throw runtime_error("Yeu cau qua lon.");
        if (!parsed) {
            headerEnd = raw.find("\r\n\r\n");
            if (headerEnd != string::npos) {
                istringstream headers(raw.substr(0, headerEnd));
                headers >> request.method >> request.path;
                string line;
                getline(headers, line);
                while (getline(headers, line)) {
                    if (!line.empty() && line.back() == '\r') line.pop_back();
                    const size_t colon = line.find(':');
                    if (colon == string::npos) continue;
                    const string name = lower(line.substr(0, colon));
                    const string value = trim(line.substr(colon + 1));
                    if (name == "content-length") length = parseNumber(value);
                    if (name == "authorization") request.authorization = value;
                    if (name == "origin") request.origin = value;
                    if (name == "host") request.host = value;
                }
                if (length > 65536) throw runtime_error("Noi dung yeu cau qua lon.");
                parsed = true;
            }
        }
        if (parsed && raw.size() >= headerEnd + 4 + length) break;
    }
    request.body = raw.substr(headerEnd + 4, length);
    return request;
}

void sendResponse(Socket client, int status, const string& type, const string& body) {
    const char* label = "Server Error";
    if (status == 200) label = "OK";
    else if (status == 401) label = "Unauthorized";
    else if (status == 404) label = "Not Found";
    else if (status == 400) label = "Bad Request";
    ostringstream out;
    out << "HTTP/1.1 " << status << ' ' << label << "\r\n"
        << "Content-Type: " << type << "; charset=utf-8\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Cache-Control: no-store\r\n"
        << "X-Content-Type-Options: nosniff\r\n";
    if (status == 401) out << "WWW-Authenticate: Basic realm=\"CareQueue Dieu Phoi\"\r\n";
    out << "Connection: close\r\n\r\n" << body;
    const string response = out.str();
    size_t sent = 0;
    const auto deadline = chrono::steady_clock::now() + chrono::seconds(5);
    while (sent < response.size() && chrono::steady_clock::now() < deadline) {
        const int n = send(client, response.data() + sent,
                           static_cast<int>(response.size() - sent), 0);
        if (n <= 0) break;
        sent += n;
    }
}

void sendFile(Socket client, const char* path, const string& type) {
    ifstream file(path, ios::binary);
    if (!file) { sendResponse(client, 404, "text/plain", "Khong tim thay file."); return; }
    ostringstream body;
    body << file.rdbuf();
    sendResponse(client, 200, type, body.str());
}

void handle(Socket client, WebHospital& hospital, const string& expectedAuth) {
    try {
        const Request request = readRequest(client);
        const bool publicPage = request.method == "GET" &&
            (request.path == "/waiting" || request.path == "/api/waiting-display" ||
             request.path == "/queue-demo" || request.path == "/health");
        if (!publicPage && !expectedAuth.empty() && request.authorization != expectedAuth) {
            sendResponse(client, 401, "text/plain", "Can dang nhap de truy cap trang dieu phoi.");
            return;
        }
        if (request.method == "GET") {
            if (request.path == "/" || request.path == "/index.html")
                sendFile(client, "wed/index.html", "text/html");
            else if (request.path == "/app.js")
                sendFile(client, "wed/app.js", "application/javascript");
            else if (request.path == "/waiting")
                sendFile(client, "wed/waiting.html", "text/html");
            else if (request.path == "/queue-demo")
                sendFile(client, "wed/queue-demo.html", "text/html");
            else if (request.path == "/api/state")
                sendResponse(client, 200, "application/json", hospital.stateJson());
            else if (request.path == "/api/waiting-display")
                sendResponse(client, 200, "application/json", hospital.waitingJson());
            else if (request.path == "/health")
                sendResponse(client, 200, "text/plain", "ok");
            else sendResponse(client, 404, "text/plain", "Khong tim thay trang.");
            return;
        }
        if (request.method != "POST") {
            sendResponse(client, 404, "application/json", "{\"ok\":false}");
            return;
        }
        if (!request.origin.empty() &&
            request.origin != "http://" + request.host &&
            request.origin != "https://" + request.host) {
            sendResponse(client, 400, "application/json",
                         "{\"ok\":false,\"message\":\"Nguon yeu cau khong hop le.\"}");
            return;
        }
        const auto form = parseForm(request.body);
        string code;
        if (request.path == "/api/register") code = hospital.registerPatient(form);
        else if (request.path == "/api/call-next")
            code = hospital.callNext(field(form, "department"));
        else if (request.path == "/api/call-specific")
            code = hospital.callSpecific(field(form, "ticket"));
        else if (request.path == "/api/complete")
            hospital.complete(field(form, "ticket"), false);
        else if (request.path == "/api/absent")
            hospital.complete(field(form, "ticket"), true);
        else if (request.path == "/api/cancel")
            hospital.cancel(field(form, "ticket"));
        else if (request.path == "/api/transfer")
            hospital.transferEmergency(field(form, "ticket"));
        else if (request.path == "/api/transfer-department")
            hospital.transferDepartment(field(form, "ticket"), field(form, "department"));
        else {
            sendResponse(client, 404, "application/json", "{\"ok\":false,\"message\":\"Khong tim thay endpoint.\"}");
            return;
        }
        sendResponse(client, 200, "application/json",
                     "{\"ok\":true,\"ticket\":\"" + jsonEscape(code) + "\"}");
    } catch (const exception& error) {
        sendResponse(client, 400, "application/json",
                     "{\"ok\":false,\"message\":\"" + jsonEscape(error.what()) + "\"}");
    }
}

unsigned short serverPort() {
    const char* value = getenv("PORT");
    if (value == nullptr || *value == 0) return 8080;
    char* end = nullptr;
    const long port = strtol(value, &end, 10);
    if (end == value || *end != 0 || port < 1 || port > 65535)
        throw invalid_argument("PORT phai nam trong khoang 1-65535.");
    return static_cast<unsigned short>(port);
}
}

int main() {
    try {
        const char* hostEnv = getenv("HOST");
        string host = "127.0.0.1";
        if (hostEnv != nullptr && *hostEnv != 0) host = hostEnv;
        const char* passwordEnv = getenv("ADMIN_PASSWORD");
        string password;
        if (passwordEnv != nullptr) password = passwordEnv;
        if (host != "127.0.0.1" && password.empty())
            throw runtime_error("Dat ADMIN_PASSWORD truoc khi mo website cong khai.");
        string expectedAuth;
        if (!password.empty()) expectedAuth = "Basic " + base64("admin:" + password);
        const char* dataEnv = getenv("HOSPITAL_DATA");
        string dataPath = "hospital-data.txt";
        if (dataEnv != nullptr && *dataEnv != 0) dataPath = dataEnv;
        WebHospital hospital(dataPath);
#ifdef _WIN32
        WSADATA wsa{};
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            throw runtime_error("Khong khoi dong duoc Winsock.");
#endif
        const unsigned short port = serverPort();
        Socket server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == INVALID) throw runtime_error("Khong tao duoc socket.");
        int reuse = 1;
        setsockopt(server, SOL_SOCKET, SO_REUSEADDR,
                   reinterpret_cast<const char*>(&reuse), sizeof(reuse));
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1)
            throw invalid_argument("HOST phai la dia chi IPv4.");
        if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 ||
            listen(server, 16) != 0)
            throw runtime_error("Khong mo duoc cong web.");
        cout << "CareQueue web: http://" << host << ':' << port << '\n';
        for (;;) {
            Socket client = accept(server, nullptr, nullptr);
            if (client == INVALID) continue;
#ifdef _WIN32
            DWORD timeout = 5000;
#else
            timeval timeout{5, 0};
#endif
            if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO,
                           reinterpret_cast<const char*>(&timeout), sizeof(timeout)) != 0 ||
                setsockopt(client, SOL_SOCKET, SO_SNDTIMEO,
                           reinterpret_cast<const char*>(&timeout), sizeof(timeout)) != 0) {
                closeSocket(client);
                throw runtime_error("Khong dat duoc thoi gian cho ket noi.");
            }
            handle(client, hospital, expectedAuth);
            closeSocket(client);
        }
    } catch (const exception& error) {
        cerr << error.what() << '\n';
        return 1;
    }
}
