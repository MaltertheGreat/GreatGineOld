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

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	float4 output = mul( pos, worldMatrix );
	output = mul( output, viewMatrix );
	output = mul( output, projectionMatrix );

	return output;
}