struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float3 worldPosition : TEXCOORD0;
	float3 cameraPosition : TEXCOORD1;
};

float4 main( PixelInputType _input ) : SV_TARGET
{
	float3 lightDirection = { -1.0f, 3.0f, -2.0f };
	lightDirection = normalize( lightDirection );

	float ambient = 0.333f;
	float diffuse = saturate( dot( _input.normal, lightDirection ) );
	float light = max( diffuse, ambient );

	float3 viewDirection = normalize( _input.cameraPosition - _input.worldPosition );
	float3 reflection = normalize( 2 * _input.normal * dot( _input.normal, lightDirection ) - lightDirection );
	float specular = pow( max( 0, dot( reflection, viewDirection ) ), 4.0f ) / 4.0f;

	return saturate( mul( _input.color, light ) + specular );
}