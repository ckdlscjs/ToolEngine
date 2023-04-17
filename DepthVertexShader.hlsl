struct VS_INPUT
{
	float4 position : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
	float4 depthPosition : TEXCOORD1;
};

//if using row_major, not transpose in cpp
cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	input.position.w = 1.0f;
	output.position = mul(input.position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProj);

	output.depthPosition = output.position;
	
	output.normal = mul(input.normal, (float3x3)matWorld);
	output.normal = normalize(output.normal);

	output.tex = input.tex;
	output.color = input.color;
	return output;
}