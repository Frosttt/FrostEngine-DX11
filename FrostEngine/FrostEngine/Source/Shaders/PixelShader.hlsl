cbuffer CBuf
{
	row_major matrix transform;
	float3 padding;
	float time;
};

float4 main(float3 color : Color) : SV_TARGET
{
  float red =   color.r * sin( 0.1f + time) * 0.5f + 0.5f; // Use sine wave for red component
  float green = color.g * cos( 0.1f + time) * 0.5f + 0.5f; // Use sine wave for green component
  float blue =  color.b * sin( 0.5f + time) * 0.5f + 0.5f; // Use combined sine wave for blue component

 return float4(red, green, blue, 1.0f);
}