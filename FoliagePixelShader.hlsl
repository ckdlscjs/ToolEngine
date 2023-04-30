Texture2D TextureColor : register(t0);
sampler TextureSamplerColor : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 tex = TextureColor.Sample(TextureSamplerColor, input.tex);
	if (tex.r < 0.1)
		discard;
	return float4(tex.rgb, 1.0f);
}