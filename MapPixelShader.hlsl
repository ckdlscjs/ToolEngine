Texture2D TextureColor : register(t0);
Texture2D g_txMaskTex : register(t1);
Texture2D g_txTex2 : register(t2);

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

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 tex = TextureColor.Sample(TextureSamplerColor, input.tex);
	float4 mask = g_txMaskTex.Sample(TextureSamplerColor, input.tex.xy);
	float4 tex2 = g_txTex2.SampleLevel(TextureSamplerColor, input.tex.xy * 1.0f, 0) * mask.r;
	tex2 = g_txTex2.SampleLevel(TextureSamplerColor, input.tex.xy * 1.0f, 0) * mask.g;
	tex2 = g_txTex2.SampleLevel(TextureSamplerColor, input.tex.xy * 1.0f, 0) * mask.b;
	tex2 = g_txTex2.SampleLevel(TextureSamplerColor, input.tex.xy * 1.0f, 0) * mask.a;

	float4 fBlendColor = 0;
	fBlendColor.r = tex2.r;
	fBlendColor.g = tex2.g;
	fBlendColor.b = tex2.b;
	fBlendColor.a = tex2.a;

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

	return float4(final_light, 1.0f);
}