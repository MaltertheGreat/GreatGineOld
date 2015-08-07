cbuffer WorldBuffer : register(b0)
{
	matrix projectionMatrix;
}

cbuffer ViewBuffer : register(b1)
{
	matrix viewMatrix;
	float4 cameraPosition;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix worldMatrix;
}

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

PixelInputType main( float4 _position : POSITION, float3 _normal : NORMAL, float4 _color : COLOR )
{
	PixelInputType output;

	output.position = mul( _position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.normal = mul( _normal, (float3x3)worldMatrix );
	output.normal = normalize( output.normal );

	output.color = _color;

	return output;
}