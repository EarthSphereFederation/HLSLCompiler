#include "DXCLoader.h"
#include "SystemUtils.h"
#include <dxc/Support/Global.h>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <cassert>
#include <wrl.h>
using namespace Microsoft::WRL;

HRESULT Test(dxc::DxcDllSupport& dll_support, EShaderBlobType target)
{
    std::string test_shader = "[shader(\"pixel\")]void main(){}";
    CComPtr<IDxcLibrary> library;
    IFR(dll_support.CreateInstance(CLSID_DxcLibrary, &library));
    CComPtr<IDxcBlobEncoding> source;
    IFR(library->CreateBlobWithEncodingFromPinned(test_shader.data(), test_shader.size(), CP_ACP, &source));

    std::vector<LPCWSTR> args;
    if (target == EShaderBlobType::kSPIRV)
    {
        args.emplace_back(L"-spirv");
    }

    CComPtr<IDxcOperationResult> result;
    CComPtr<IDxcCompiler> compiler;
    IFR(dll_support.CreateInstance(CLSID_DxcCompiler, &compiler));
    IFR(compiler->Compile(
        source,
        L"main.hlsl",
        L"",
        L"lib_6_5",
        args.data(), args.size(),
        nullptr, 0,
        nullptr,
        &result
    ));

    CComPtr<IDxcBlobEncoding> errors;
    result->GetErrorBuffer(&errors);
    if (errors && errors->GetBufferSize() > 0)
    {
        return E_FAIL;
    }

    HRESULT hr = {};
    result->GetStatus(&hr);
    return hr;
}

std::unique_ptr<dxc::DxcDllSupport> Load(const std::string& path, EShaderBlobType target)
{
#if defined(_WIN32)
    auto dxcompiler_path = std::filesystem::u8path(path) / "dxcompiler.dll";
#elif defined(__APPLE__)
    auto dxcompiler_path = std::filesystem::u8path(path) / "libdxcompiler.dylib";
#else
    auto dxcompiler_path = std::filesystem::u8path(path) / "libdxcompiler.so";
#endif
    if (!std::filesystem::exists(dxcompiler_path))
    {
        return {};
    }

#ifdef _WIN32
auto dxil_path = std::filesystem::u8path(path) / "dxil.dll";
    std::unique_ptr<dxc::DxcDllSupport> dll_support_dxil;
    if (target == EShaderBlobType::kDXIL)
    {
        dll_support_dxil = std::make_unique<dxc::DxcDllSupport>();
        if (std::filesystem::exists(dxil_path))
        {
            dll_support_dxil->InitializeForDll(dxil_path.wstring().c_str(), "DxcCreateInstance");
        }
        else
        {
            auto windows_kits_dxil_path = std::filesystem::u8path(WINDOWS_KITS_LOCATION) / "dxil.dll";
            dll_support_dxil->InitializeForDll(windows_kits_dxil_path.wstring().c_str(), "DxcCreateInstance");
        }

        if (!dll_support_dxil->IsEnabled())
        {
            return {};
        }
    }
#endif

    auto dll_support = std::make_unique<dxc::DxcDllSupport>();
    if (FAILED(dll_support->InitializeForDll(dxcompiler_path.wstring().c_str(), "DxcCreateInstance")))
    {
        return {};
    }
    if (FAILED(Test(*dll_support, target)))
    {
        return {};
    }

    return dll_support;
}

std::string GetVulkanSdkLocalion()
{
    std::string sdk_localion = GetEnvironmentVar("VULKAN_SDK");
#if defined(_WIN32)
    return sdk_localion + "/Bin";
#elif defined(__APPLE__)
    return sdk_localion + "/macOS/lib";
#else
    return sdk_localion + "/lib";
#endif
}

std::unique_ptr<dxc::DxcDllSupport> GetDxcSupportImpl(EShaderBlobType target)
{
    std::vector<std::string> localions = {
            GetExecutableDir(),
            DXC_CUSTOM_LOCATION,
#if defined(_WIN32)
            WINDOWS_KITS_LOCATION,
#endif
            GetVulkanSdkLocalion(),
    };
    for (const auto& path : localions)
    {
        auto res = Load(path, target);
        if (res)
        {
            return res;
        }
    }
    assert(false);
    return {};
}

dxc::DxcDllSupport& GetDxcSupport(EShaderBlobType target)
{
    static std::map<EShaderBlobType, std::unique_ptr<dxc::DxcDllSupport>> cache;
    auto it = cache.find(target);
    if (it == cache.end())
    {
        it = cache.emplace(target, GetDxcSupportImpl(target)).first;
    }
    return *it->second;
}
