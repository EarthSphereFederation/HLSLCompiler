//
// Created by AIC on 2022/5/3.
//

#include "SystemUtils.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#endif

#include <codecvt>
#include <locale>
#include <vector>

std::wstring utf8_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

std::string wstring_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}


std::string GetExecutablePath()
{
#if defined(_WIN32)
    char buf[MAX_PATH] = {};
    GetModuleFileNameA(nullptr, buf, std::size(buf));
    return buf;
#elif defined(__APPLE__)
    uint32_t buf_size = 0;
    if (_NSGetExecutablePath(nullptr, &buf_size) != -1)
        return {};
    std::vector<char> buf(buf_size);
    if (_NSGetExecutablePath(buf.data(), &buf_size) != 0)
        return {};
    return buf.data();
#else
    char buf[PATH_MAX] = {};
    readlink("/proc/self/exe", buf, std::size(buf) - 1);
    return buf;
#endif
}

std::string GetExecutableDir()
{
    auto path = GetExecutablePath();
    return path.substr(0, path.find_last_of("\\/"));
}

std::string GetEnvironmentVar(const std::string& name)
{
    const char* res = getenv("VULKAN_SDK");
    return res ? res : "";
}
