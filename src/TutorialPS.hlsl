#include "Tutorial.hlsli"

const float3 color : register(b0);

[RootSignature(TUTORIAL_ROOT_SIGNATURE)]
float4 main(in VertexData pxd) : SV_TARGET
{
	float dist = smoothstep(0.0f, 2.8f * ddx(pxd.uv.x), 1.0f - length(pxd.uv));
	return float4(color * dist, 1.0f);
}
