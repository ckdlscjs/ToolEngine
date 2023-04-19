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
	float4 tex2 : TEXCOORD3;
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
	float2 worldSize;
	float cellDistance;
	int tileCount;
};

//cbuffer constant : register(b2)
//{
//	float4 lightDirection;
//	float4 cameraPosition;
//};

cbuffer constant : register(b2)
{
	row_major float4x4 lightViewMatrix;
	row_major float4x4 lightProjectionMatrix;
	float3 lightPosition;
};


VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	input.position.w = 1.0f;
	float4 vLocal = input.position;
	float4 vWorld = mul(vLocal, matWorld);
	float4 vView = mul(vWorld, matView);
	float4 vProj = mul(vView, matProj);
	output.position = vProj;

	output.lightViewPosition = mul(input.position, matWorld);
	output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	output.lightPosition = lightPosition.xyz - vWorld.xyz;
	output.lightPosition = normalize(output.lightPosition);

	//normalize to texcoord(vertex to 0 ~ 1)
	int world_half = worldSize.x / 2.0f;
	output.tex2.x = (vWorld.x + (float)world_half * cellDistance) / worldSize.x; //0 ~ 1
	output.tex2.y = 1.0f - (((float)vWorld.z / world_half * cellDistance) * 0.5f + 0.5f); //0 ~ 1
	output.tex2.x = output.tex2.x * (float)tileCount;
	output.tex2.y = output.tex2.y * (float)tileCount;

	//// �ؽ�ó ��ȯ ��� ��� world to tex
	//matrix matTex = 0;
	//matTex._11 = 1.0f / (((float)worldSize.x / tileCount) * cellDistance);
	//matTex._32 = -1.0f / (((float)worldSize.x / tileCount) * cellDistance);
	//matTex._41 = 0.0f; // Ÿ�Ͽ� ���� Ȧ��=0.5f, ¦��=0.0f
	//matTex._42 = 0.0f;

	//float4 vUV = mul(vWorld, matTex);
	//output.tex2.x = vUV.x;
	//output.tex2.y = vUV.y;

	// ������ǥ ���
	output.tex2.z = (vProj.x / vProj.w) * 0.5f + 0.5f;
	output.tex2.w = (vProj.y / vProj.w) * 0.5f + 0.5f;

	output.normal = mul(float4(input.normal, 1.0f), matWorld).xyz;
	output.normal = normalize(output.normal);

	output.tex = input.tex;
	output.color = input.color;
	return output;
}