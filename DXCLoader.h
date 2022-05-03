#pragma once
#include "ShaderBaseTypes.h"
#include <dxc/Support/WinIncludes.h>
#include <dxc/Support/dxcapi.use.h>

dxc::DxcDllSupport& GetDxcSupport(EShaderBlobType type);
