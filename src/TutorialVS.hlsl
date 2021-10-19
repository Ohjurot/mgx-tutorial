#include "Tutorial.hlsli"

[RootSignature(TUTORIAL_ROOT_SIGNATURE)]
float4 main(float2 pos : POSITION) : SV_POSITION
{
	return float4(pos, 0.0f, 1.0f);
}
