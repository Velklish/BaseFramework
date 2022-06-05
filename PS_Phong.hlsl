cbuffer LightBuf
{
	float3 lightPos;
};

static const float3 materialColor = { 0.7f,0.7f,0.9f };
static const float3 ambient = { 0.05f,0.05f,0.05f };
static const float3 diffuseColor = { 1.0f,1.0f,1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attlib = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
	const float3 vTol = lightPos - worldPos;
	const float distTol = length(vTol);
	const float dirTol = vTol / distTol;
	const float att = 1.0f/(attConst + attLin * distTol + attQuad * (distTol * distTol));
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirTol, n));
	return float4(saturate(diffuse + ambient),1.0f);
}