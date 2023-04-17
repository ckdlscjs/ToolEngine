struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
	float4 depthPosition : TEXCOORD1;
};


float4 psmain(PS_INPUT input) : SV_TARGET
{
	float depthValue;

	depthValue = input.depthPosition.z / input.depthPosition.w;
	//depthValue = depthValue * depthValue * depthValue * depthValue * depthValue * depthValue;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}