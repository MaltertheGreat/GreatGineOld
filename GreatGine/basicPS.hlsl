struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main( PixelInputType _input ) : SV_TARGET
{
	float3 lightDirection = { -1.0f, 2.0f, -3.0f };
	lightDirection = normalize( lightDirection );

	float lightIntensity = saturate( dot( _input.normal, lightDirection ) );

	float4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
		color = saturate( color * lightIntensity );

	return color;
}