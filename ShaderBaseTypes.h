#pragma once
#include <string>
#include <map>

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

enum class EFeatureLevel
{
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

    ShaderDesc(const std::string& shader_path, const std::string& entrypoint, EShaderType type, const std::string& model)
    : shader_path(shader_path)
    , entrypoint(entrypoint)
    , type(type)
    , model(model)
    {
    }
};
