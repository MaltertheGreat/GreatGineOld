cbuffer WorldBuffer : register(b0)
{
	matrix worldMatrix;
}

cbuffer ViewBuffer : register(b1)
{
	matrix viewMatrix;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix projectionMatrix;
}

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType main( float4 _position : POSITION, float4 _color : COLOR )
{
	PixelInputType output;

	output.position = mul( _position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.color = _color;

	return output;
}