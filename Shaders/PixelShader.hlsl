float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

cbuffer ConstBuffer
{
	float4 face_colors[6];
}

float4 main(uint triangleId : SV_PrimitiveID) : SV_TARGET
{
	return face_colors[triangleId/2];
}