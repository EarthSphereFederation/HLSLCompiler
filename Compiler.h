#pragma once
#include "ShaderBaseTypes.h"
#include <vector>

std::vector<uint8_t> Compile(const ShaderDesc& shader, EShaderBlobType blob_type);
