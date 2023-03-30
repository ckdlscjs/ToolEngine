struct VS_INPUT
{
	float4 position : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;

	float4 index : INDEX0;
	float4 weight : WEIGHT0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
	float3 direction_to_camera : TEXCOORD1;
	float4 m_light_direction : TEXCOORD2;
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
	float4 lightDirection;
	float4 cameraPosition;
};

cbuffer constant_animation : register(b2)
{
	row_major float4x4 matBone[255];
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 vLocal = input.position;
	float4 vAnim = 0;
	float4 vAnimNormal = 0;
	for (int iBone = 0; iBone < 4; iBone++)
	{
		uint iBoneIndex = input.index[iBone];
		float fWeight = input.weight[iBone];
		vAnim += mul(vLocal, matBone[iBoneIndex]) * fWeight;
		vAnimNormal += mul(input.normal, matBone[iBoneIndex]) * fWeight;
	}
	float4 vWorld = mul(vAnim, matWorld);
	float4 vView = mul(vWorld, matView);
	float4 vProj = mul(vView, matProj);
	
	output.position = vProj;
	output.normal = vAnimNormal;
	output.tex = input.tex;
	output.color = input.color;
	output.direction_to_camera = normalize(vWorld.xyz - cameraPosition.xyz);
	output.m_light_direction = float4(output.direction_to_camera, 1.0f);
	return output;
}