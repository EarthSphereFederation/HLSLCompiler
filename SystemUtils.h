//
// Created by AIC on 2022/5/3.
//

#pragma once
#include <string>

std::wstring utf8_to_wstring(const std::string& str);
std::string wstring_to_utf8(const std::wstring& str);
std::string GetExecutablePath();
std::string GetExecutableDir();
std::string GetEnvironmentVar(const std::string& name);
