#pragma once
#include <string>
#include <vector>

class EdgeLauncher {
public:
    static std::wstring ToFileURI(const std::wstring& path);
    static std::wstring BuildArgs(const std::wstring& url, const std::vector<std::wstring>& args);
    static bool Launch(const std::wstring& edgePath, const std::wstring& targetPath, const std::vector<std::wstring>& args);
    static std::wstring GetFullPath(const std::wstring& relative);
};