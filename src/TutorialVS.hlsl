#include "Tutorial.hlsli"

const float aspectRatio : register(b1);
const float scaling : register(b2);

[RootSignature(TUTORIAL_ROOT_SIGNATURE)]
VertexData main(float2 pos : POSITION)
{
	VertexData vtx;
	vtx.pos = float4(pos.x * aspectRatio * scaling, pos.y * scaling, 0.0f, 1.0f);
	vtx.uv = pos;
	return vtx;
}
