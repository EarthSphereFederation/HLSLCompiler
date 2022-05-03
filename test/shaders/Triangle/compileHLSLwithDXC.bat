dxc.exe -spirv -T vs_6_0 -E mainVS shader.hlsl -Fo vert.spv
dxc.exe -spirv -T ps_6_0 -E mainPS shader.hlsl -Fo frag.spv