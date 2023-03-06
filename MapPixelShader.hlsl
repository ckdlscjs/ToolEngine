Texture2D TextureColor : register(t0);
Texture2D g_txMaskTex : register(t1);
Texture2D g_txTex2 : register(t2);
Texture2D g_txTex3 : register(t3);
Texture2D g_txTex4 : register(t4);
Texture2D g_txTex5 : register(t5);

sampler TextureSamplerColor : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float3 direction_to_camera : TEXCOORD1;
	float4 m_light_direction : TEXCOORD2;
	float3 world : TEXCOORD3;
	float4 tex2 : TEXCOORD4;
};

cbuffer constant : register(b0)
{
	row_major float4x4 matWorld;
	row_major float4x4 matView;
	row_major float4x4 matProj;
	float4 m_light_direction;
};

float3 blend(float4 texture1, float a1, float4 texture2, float a2) 
{ 
	float depth = 0.2; 
	float ma = max(texture1.a + a1, texture2.a + a2) - depth; 
	float b1 = max(texture1.a + a1 - ma, 0); 
	float b2 = max(texture2.a + a2 - ma, 0); 
	return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2); 
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 tex = TextureColor.Sample(TextureSamplerColor, input.tex);
	float4 mask = g_txMaskTex.Sample(TextureSamplerColor, input.tex.xy);
	float4 fColorTile_r = g_txTex2.Sample(TextureSamplerColor, input.tex) * mask.r;
	float4 fColorTile_g = g_txTex3.Sample(TextureSamplerColor, input.tex) * mask.g;
	float4 fColorTile_b = g_txTex4.Sample(TextureSamplerColor, input.tex) * mask.b;
	float4 fColorTile_a = g_txTex5.Sample(TextureSamplerColor, input.tex) * mask.a;
	float fTotalWeight = mask.r + mask.g + mask.b + mask.a;
	float4 fBlendColor = fColorTile_r + fColorTile_g + fColorTile_b + fColorTile_a;
	fBlendColor = ((1.0f - fTotalWeight / 4) * tex.a) > fTotalWeight ? (1.0f - fTotalWeight / 4.0f) * tex : fBlendColor;

	//AmbientLight
	float ka = 0.1f;
	float3 ia = float3(1.0f, 1.0f, 1.0f);
	ia *= fBlendColor.rgb;
	float3 ambient_light = ka * ia;

	//DiffuseLight
	float kd = 0.8f;
	float3 id = float3(1.0f, 1.0f, 1.0f);
	id *= fBlendColor.rgb;

	float amount_diffuse_light = max(0.0f, dot(input.normal , -input.m_light_direction.xyz));
	float3 diffuse_light = kd * amount_diffuse_light * id;

	//SpecularLight
	float ks = 1.0f;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	is *= fBlendColor.rgb;
	float3 reflected_light = reflect(input.m_light_direction.xyz, input.normal);
	float shininess = 10.0f;
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.direction_to_camera)), shininess);

	float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light.rgb, 1.0f);
}