#include "NativeWebView.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

static std::string Utf8Encode(const std::wstring& w) {
    int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &result[0], size, nullptr, nullptr);
    result.pop_back();
    return result;
}

static bool IsUnreserved(unsigned char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '-' || c == '_' || c == '.' || c == '/' || c == ':';
}

static std::string UrlEncode(const std::string& input) {
    std::ostringstream oss;
    for (unsigned char c : input) {
        if (IsUnreserved(c)) {
            oss << c;
        } else {
            oss << '%' << std::uppercase << std::hex
                << (int)c;
            oss << std::nouppercase << std::dec;
        }
    }
    return oss.str();
}

std::wstring EdgeLauncher::ToFileURI(const std::wstring& path) {
    std::string utf8 = Utf8Encode(path);

    for (auto& c : utf8) {
        if (c == '\\') c = '/';
    }

    std::string encoded = "file:///" + UrlEncode(utf8);

    return std::wstring(encoded.begin(), encoded.end());
}

std::wstring EdgeLauncher::BuildArgs(const std::wstring& url, const std::vector<std::wstring>& args) {
    std::wstringstream ss;
    ss << L"--app=\"" << url << L"\" ";
    for (const auto& a : args) {
        ss << a << L" ";
    }
    return ss.str();
}

bool EdgeLauncher::Launch(const std::wstring& edgePath, const std::wstring& targetPath, const std::vector<std::wstring>& args) {
    std::wstring url = ToFileURI(targetPath);
    std::wstring cmd = L"\"" + edgePath + L"\" " + BuildArgs(url, args);

    std::vector<wchar_t> buffer(cmd.begin(), cmd.end());
    buffer.push_back(L'\0');

    STARTUPINFOW si{};
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi{};

    BOOL ok = CreateProcessW(
        nullptr,
        buffer.data(),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (ok) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    return ok;
}

std::wstring EdgeLauncher::GetFullPath(const std::wstring& relative) {
    wchar_t buffer[MAX_PATH];
    GetFullPathNameW(relative.c_str(), MAX_PATH, buffer, nullptr);
    return buffer;
}
