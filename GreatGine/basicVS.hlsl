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
	float3 normal : NORMAL;
};

PixelInputType main( float4 _position : POSITION, float3 _normal : NORMAL )
{
	PixelInputType output;

	output.position = mul( _position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.normal = mul( _normal, (float3x3)worldMatrix );
	output.normal = normalize( output.normal );

	return output;
}