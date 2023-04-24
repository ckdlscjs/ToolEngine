Texture2D TextureColor : register(t0);
sampler TextureSamplerColor : register(s0);

//Texture2D DepthmapColor : register(t1);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPosition : TEXCOORD2;
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

	if (tex.a < 0.5f)
		discard;

	////AmbientLight
	//float ka = 0.1f;
	//float3 ia = float3(1.0f, 1.0f, 1.0f);
	//ia *= finalColor.rgb;
	//float3 ambient_light = ka * ia;

	////DiffuseLight
	//float kd = 0.8f;
	//float3 id = float3(1.0f, 1.0f, 1.0f);
	//id *= finalColor.rgb;

	//float amount_diffuse_light = max(0.0f, dot(input.normal , -input.lightDirection.xyz));
	//float3 diffuse_light = kd * amount_diffuse_light * id;

	////SpecularLight
	//float ks = 1.0f;
	//float3 is = float3(1.0f, 1.0f, 1.0f);
	//is *= finalColor.rgb;
	//float3 reflected_light = reflect(input.lightDirection.xyz, input.normal);
	//float shininess = 10.0f;
	//float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.lightDirection.xyz)), shininess);

	//float3 specular_light = ks * amount_specular_light * is;

	//float3 final_light = ambient_light + diffuse_light + specular_light;

	//depthMap
	/*float4 color = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float2 projectTexcoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	projectTexcoord.x = input.lightViewPosition.x / input.lightViewPosition.w * 0.5f + 0.5f;
	projectTexcoord.y = -input.lightViewPosition.y / input.lightViewPosition.w * 0.5f + 0.5f;

	if ((saturate(projectTexcoord.x) == projectTexcoord.x) && (saturate(projectTexcoord.y) == projectTexcoord.y))
	{
		depthValue = DepthmapColor.Sample(TextureSamplerColor, projectTexcoord).r;

		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		lightDepthValue -= 0.001f;

		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(input.normal, normalize(input.lightViewPosition.xyz)));

			if (lightIntensity > 0.0f)
			{
				color = lightIntensity * color;

				color = saturate(color);
			}
		}
	}

	return float4(tex.rgb * color.rgb, 1.0f);*/

	//fog
	float3 fogColor = float3(0.5f, 0.5f, 0.5f);

	// 선형 Fog와 지수 Fog의 양을 결합하여 최종 Fog 양을 계산
	float fogAmount = lerp(input.linearFogAmount, input.expFogAmount, 0.5f);

	// 개체의 색상에 Fog를 적용
	float3 finalColor = lerp(tex.rgb, fogColor, fogAmount);
	return float4(finalColor, 1.0f);
}