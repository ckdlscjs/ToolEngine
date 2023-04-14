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
	float3 direction_to_camera : TEXCOORD1;
	float4 m_light_direction : TEXCOORD2;
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

cbuffer constant : register(b2)
{
	float4 lightDirection;
	float4 cameraPosition;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 vLocal = input.position;
	float4 vWorld = mul(vLocal, matWorld);
	float4 vView = mul(vWorld, matView);
	float4 vProj = mul(vView, matProj);
	//output.direction_to_camera = normalize(output.position.xyz - m_camera_position.xyz); //dlight
	//output.direction_to_camera = normalize(m_camera_position.xyz - output.position.xyz); //plight
	output.direction_to_camera = normalize(vWorld.xyz - cameraPosition.xyz);
	output.m_light_direction = float4(output.direction_to_camera, 1.0f);


	//normalize to texcoord(vertex to 0 ~ 1)
	int world_half = worldSize.x / 2.0f;
	output.tex2.x = (vWorld.x + (float)world_half * cellDistance) / worldSize.x; //0 ~ 1
	output.tex2.y = 1.0f - (((float)vWorld.z / world_half * cellDistance) * 0.5f + 0.5f); //0 ~ 1
	output.tex2.x = output.tex2.x * (float)tileCount;
	output.tex2.y = output.tex2.y * (float)tileCount;

	//// 텍스처 변환 행렬 사용 world to tex
	//matrix matTex = 0;
	//matTex._11 = 1.0f / (((float)worldSize.x / tileCount) * cellDistance);
	//matTex._32 = -1.0f / (((float)worldSize.x / tileCount) * cellDistance);
	//matTex._41 = 0.0f; // 타일에 개수 홀수=0.5f, 짝수=0.0f
	//matTex._42 = 0.0f;

	//float4 vUV = mul(vWorld, matTex);
	//output.tex2.x = vUV.x;
	//output.tex2.y = vUV.y;

	// 투영좌표 사용
	output.tex2.z = (vProj.x / vProj.w) * 0.5f + 0.5f;
	output.tex2.w = (vProj.y / vProj.w) * 0.5f + 0.5f;

	output.position = vProj;
	output.normal = input.normal;
	output.tex = input.tex;
	output.color = input.color;
	return output;
}