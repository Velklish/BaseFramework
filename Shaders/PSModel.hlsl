cbuffer lightBuffer : register(b1)
{
	float3 ambientLightColor;
	float ambientLightStrength;
}

cbuffer dynamicLightBuffer : register(b2)
{
	float3 dynamicLightColor;
	float dynamicLightStrength;
	float3 dynamicLightPosition;
	float3 viewPosition;
}

struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
	float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 lightDir = float3(0,0,-1);
	float normal = normalize(input.inNormal.xyz);
	float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);

	float3 ambientLight = ambientLightColor * ambientLightStrength;
	
	float3 appliedLight = ambientLight;
	
	float3 vectorToLight = normalize(-dynamicLightPosition - input.inWorldPos);
	
	float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal),0);
	
	float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightStrength;
	
	appliedLight += diffuseLight;
	float3 spec = pow(max(0,dot(vectorToLight,normalize(reflect(lightDir,normal)))), 2.1f) * 2.1f;
	appliedLight += spec;
	float3 finalColor = sampleColor * appliedLight;
	
	return float4(finalColor, 1.0f);
	//return float4(sampleColor, 1.0f);

	/*float4 diffVal = objTexture.Sample(objSamplerState, input.inTexCoord);
	float3 kd = diffVal.xyz;
	float3 normal = normalize(input.inNormal.xyz);
	float3 viewDir = normalize(viewPosition.xyz - input.inWorldPos.xyz);
	float3 lightDir = -dynamicLightPosition;
	float3 refVec = normalize(reflect(lightDir,normal));

	float3 diffuse = max(0, dot(lightDir, normal)) * kd;
	float3 ambient = kd * 1.0f * ambientLightStrength;
	float3 spec = pow(max(0,dot(viewDir,refVec)), 0.05f) * 0.05f;

	return float4(dynamicLightColor * (diffuse + ambient + spec), 1.0f);*/
}