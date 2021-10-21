#define TUTORIAL_ROOT_SIGNATURE "" \
"RootFlags( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)," \
"RootConstants(num32BitConstants=4, b0)," \
"RootConstants(num32BitConstants=4, b1)," \
"RootConstants(num32BitConstants=4, b2)," \
"DescriptorTable(" \
"   SRV(t0, numDescriptors = 4, flags = DESCRIPTORS_VOLATILE)" \
")," \
"StaticSampler(s0)"

/*
* RootSignature Layout:
* 0 - RootConstants(3) - b0 - Color
* 1 - RootConstants(1) - b1 - Aspect ratio
* 2 - RootConstants(1) - b2 - scaling
* 3 - DescriptorTable 
*              SRV(4)  - t0 - textures
*/

struct VertexData
{
    float4 pos : SV_Position;
    float2 uv : Texcord;
};
