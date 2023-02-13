struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 color : COLOR0;
};

cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return float4(input.color, 1);
}