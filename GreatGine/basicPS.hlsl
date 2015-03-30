struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

float4 main( PixelInputType _input ) : SV_TARGET
{
	float3 lightDirection = { -1.0f, 3.0f, -2.0f };
	lightDirection = normalize( lightDirection );

	float diffuse = saturate( dot( _input.normal, lightDirection ) );
	float ambient = 0.2f;
	float light = max( diffuse, ambient );

	return mul( _input.color, light );
}