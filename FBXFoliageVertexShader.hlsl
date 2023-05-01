struct VS_INPUT
{
	float4 position : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;

	//float3 positionInstance : POSITION1;
	row_major float4x4 worldInstance : WORLD;
	uint id : SV_InstanceID;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
};

//if using row_major, not transpose in cpp
cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
};

cbuffer constant : register(b1)
{
	uint instanceCount;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	input.position.w = 1.0f;
	float4 vWorld = mul(input.position, input.worldInstance);
	float4 vView = mul(vWorld, matView);
	float4 vProj = mul(vView, matProj);

	output.position = vProj;
	output.tex = input.tex;
	output.color = input.color;

	return output;
}