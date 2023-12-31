Texture2D TextureColor : register(t0);
sampler TextureSamplerColor : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
	float3 direction_to_camera : TEXCOORD1;
	float4 m_light_direction : TEXCOORD2;
	float linearFogAmount : LINEAR_FOG_AMOUNT;
	float expFogAmount : EXP_FOG_AMOUNT;
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

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 tex = TextureColor.Sample(TextureSamplerColor, input.tex);

	//AmbientLight
	float ka = 0.1f;
	float3 ia = float3(1.0f, 1.0f, 1.0f);
	ia *= tex.rgb;
	float3 ambient_light = ka * ia;

	//DiffuseLight
	float kd = 0.8f;
	float3 id = float3(1.0f, 1.0f, 1.0f);
	id *= tex.rgb;

	float amount_diffuse_light = max(0.0f, dot(input.normal , -input.m_light_direction.xyz));
	float3 diffuse_light = kd * amount_diffuse_light * id;

	//SpecularLight
	float ks = 1.0f;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	is *= tex.rgb;
	float3 reflected_light = reflect(input.m_light_direction.xyz, input.normal);
	float shininess = 10.0f;
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.direction_to_camera)), shininess);

	float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	//fog
	float3 fogColor = float3(0.5f, 0.5f, 0.5f);

	// 선형 Fog와 지수 Fog의 양을 결합하여 최종 Fog 양을 계산5
	float fogAmount = lerp(input.linearFogAmount, input.expFogAmount, 0.5f);

	// 개체의 색상에 Fog를 적용
	float3 finalColor = lerp(tex.rgb, fogColor, fogAmount);
	return float4(tex.rgb, 1.0f);
}