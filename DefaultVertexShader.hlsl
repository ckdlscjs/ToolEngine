struct VS_INPUT
{
	float4 position : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float3 direction_to_camera : TEXCOORD1;
};

//if using row_major, not transpose in cpp
cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
	float4 m_light_direction;
	float4 m_camera_position;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, matWorld);	//World	Transform
	output.direction_to_camera = normalize(output.position.xyz - m_camera_position.xyz);
	//output.direction_to_camera = normalize(m_camera_position.xyz - output.position.xyz);
	output.position = mul(output.position, matView);	//View	Transform
	output.position = mul(output.position, matProj);	//Proj	Transform

	output.tex = input.tex;
	output.normal = input.normal;
	output.color = input.color;
	return output;
}