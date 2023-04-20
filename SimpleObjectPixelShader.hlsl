Texture2D TextureColor : register(t0);
Texture2D DepthmapColor : register(t1);

//sampler TextureSamplerColor : register(s0);

struct PS_INPUT
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
	row_major float4x4 lightViewMatrix;
	row_major float4x4 lightProjectionMatrix;
	float3 lightPosition;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	////depthMap
	//float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//float4 lightColor = float4(0.15f, 0.15f, 0.15f, 1.0f);
	//float2 projectTexcoord;
	//float depthValue;
	//float lightDepthValue;
	//float lightIntensity;
	//projectTexcoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	//projectTexcoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	//if ((saturate(projectTexcoord.x) == projectTexcoord.x) && (saturate(projectTexcoord.y) == projectTexcoord.y))
	//{
	//	depthValue = DepthmapColor.Sample(TextureSamplerColor, projectTexcoord).r;

	//	lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
	//	/*color = float4(1.0f, 1.0f, 1.0f, 1.0f);*/
	//	// lightDepthValue에서 바이어스를 뺍니다.
	//	lightDepthValue = lightDepthValue - 0.001f;

	//	if (lightDepthValue > depthValue)
	//	{
	//		lightIntensity = saturate(dot(input.normal, input.lightPosition));

	//		if (lightIntensity > 0.0f)
	//		{
	//			color = lightIntensity * color;

	//			color = saturate(color);
	//		}
	//	}
	//}
	//lightIntensity = saturate(dot(input.normal, input.lightPosition));

	//if (lightIntensity > 0.0f)
	//{
	//	lightColor += (lightIntensity * float4(1.0f, 1.0f, 1.0f, 1.0f));
	//}
	//lightColor = saturate(lightColor);
	//return input.color * lightColor;
	return input.color;
}