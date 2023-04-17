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
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPosition : TEXCOORD2;
};

//if using row_major, not transpose in cpp
cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
};

//cbuffer constant : register(b1)
//{
//	float4 lightDirection;
//	float4 cameraPosition;
//};

cbuffer constant : register(b1)
{
	row_major float4x4 lightViewMatrix;
	row_major float4x4 lightProjectionMatrix;
	float3 lightPosition;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	input.position.w = 1.0f;

	float4 vWorld = mul(input.position, matWorld);
	float4 vView = mul(vWorld, matView);
	float4 vProj = mul(vView, matProj);

	output.lightViewPosition = mul(input.position, matWorld);
	output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	output.lightPosition = lightPosition.xyz - vWorld.xyz;
	output.lightPosition = normalize(output.lightPosition);

	output.normal = mul(input.normal, (float3x3)matWorld);
	output.normal = normalize(output.normal);

	output.position = vProj;

	output.tex = input.tex;
	output.color = input.color;

	return output;
}