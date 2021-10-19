#include "Tutorial.hlsli"

const float3 color : register(b0);

[RootSignature(TUTORIAL_ROOT_SIGNATURE)]
float4 main() : SV_TARGET
{
	return float4(color, 1.0f);
}
