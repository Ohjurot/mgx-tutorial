#include "Tutorial.hlsli"

const float3 color : register(b0);
const Texture2D<float4> textures[4] : register(t0);
sampler defaultSampler : register(s0);

[RootSignature(TUTORIAL_ROOT_SIGNATURE)]
float4 main(in VertexData pxd) : SV_TARGET
{
	// Circle computations
	float dist = smoothstep(0.0f, 2.8f * ddx(pxd.uv.x), 1.0f - length(pxd.uv));
	// Sample from texture
	float4 textureColor = textures[0].Sample(defaultSampler, (float2(pxd.uv.x, -pxd.uv.y) + 1.0f) / 2.0f);
    // Composit
	return float4(textureColor.rgb * dist, 1.0f);
}
