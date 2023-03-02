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
	float4 m_light_direction : TEXCOORD2;
	/*float2 tex2 : TEXCOORD3;*/
};

//if using row_major, not transpose in cpp
cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
	float4 m_light_direction;
	float4 m_camera_position;
	//int2 m_world_size;
	//float cell_distance;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, matWorld);	//World	Transform

	//output.tex2.x = (output.position.x + m_world_size.x / 2.0f * cell_distance) / m_world_size.x; //0 ~ 1
	//output.tex2.x = output.tex2.x * 10.0f;
	//output.tex2.y = 1.0f - ((output.position.z /  m_world_size.x / 2.0f * cell_distance) / 2.0f + 0.5f) //0 ~ 1
	//output.tex2.y = output.tex2.y * 10.0f;

	output.direction_to_camera = normalize(output.position.xyz - m_camera_position.xyz);
	//output.direction_to_camera = normalize(m_camera_position.xyz - output.position.xyz);
	output.position = mul(output.position, matView);	//View	Transform
	output.position = mul(output.position, matProj);	//Proj	Transform

	

	output.m_light_direction = float4(output.direction_to_camera,1.0f);
	output.normal = input.normal;
	output.tex = input.tex;
	output.color = input.color;
	return output;
}