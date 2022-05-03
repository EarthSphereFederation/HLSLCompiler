#pragma once
#include <string>
#include <map>
#include <cassert>

enum class EShaderBlobType
{
    kDXIL,
    kSPIRV,
};

enum class EShaderType
{
    kUnknown,
    kVertex,
    kPixel,
    kCompute,
    kGeometry,
    kAmplification,
    kMesh,
    kLibrary,
};

enum class EShaderFeatureLevel
{
    kUnknown,
    k6_0,
    k6_1,
    k6_2,
    k6_3,
    k6_4,
    k6_5,
    k6_6,
    k6_7,
};

struct ShaderDesc
{
    std::string shader_path;
    std::string entrypoint;
    EShaderType type;
    std::string model;
    std::map<std::string, std::string> defines;

    ShaderDesc(const std::string& shader_path, const std::string& entrypoint, EShaderType type, EShaderFeatureLevel featureLevel)
    : shader_path(shader_path)
    , entrypoint(entrypoint)
    , type(type)
    {
        switch (featureLevel)
        {
            case EShaderFeatureLevel::k6_0: model = "6_0"; break;
            case EShaderFeatureLevel::k6_1: model = "6_1"; break;
            case EShaderFeatureLevel::k6_2: model = "6_2"; break;
            case EShaderFeatureLevel::k6_3: model = "6_3"; break;
            case EShaderFeatureLevel::k6_4: model = "6_4"; break;
            case EShaderFeatureLevel::k6_5: model = "6_5"; break;
            case EShaderFeatureLevel::k6_6: model = "6_6"; break;
            case EShaderFeatureLevel::k6_7: model = "6_7"; break;
            default: assert(false);
        }
    }
};
