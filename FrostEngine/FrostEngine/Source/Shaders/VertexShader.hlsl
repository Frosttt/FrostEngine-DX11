

cbuffer CBuf
{
	matrix transform;
	float3 padding;
	float time;
};

float4 main( float3 pos : Position) : SV_Position
{
	return  mul(float4(pos, 1.0f), transform);
}