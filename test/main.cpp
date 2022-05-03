//
// Created by AIC on 2022/5/3.
//

#include "HLSLCompiler/Compiler.h"
#include <iostream>

int main()
{
    ShaderDesc vs_desc = { ASSETS_PATH"shaders/Triangle/Shader.hlsl", "mainVS", EShaderType::kVertex, "6_5" };
    ShaderDesc ps_desc = { ASSETS_PATH"shaders/Triangle/Shader.hlsl", "mainPS", EShaderType::kPixel, "6_5" };

    std::vector<uint8_t> vs_spv = Compile(vs_desc, EShaderBlobType::kSPIRV);
    std::vector<uint8_t> ps_spv = Compile(ps_desc, EShaderBlobType::kSPIRV);

    if (!vs_spv.empty() && !ps_spv.empty())
    {
        std::cout << "Shaders have compiled successfully!" << std::endl;
    }
    else
    {
        std::cout << "Shaders compiled failed!" << std::endl;
    }

    return 0;
}
