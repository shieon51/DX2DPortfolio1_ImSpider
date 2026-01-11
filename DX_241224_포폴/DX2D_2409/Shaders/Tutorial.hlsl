//Sementic : 매개변수나 반환 자료형의 사용 용도
//SV(System Value)

cbuffer VPBuffer : register(b0)
{
	//matrix == flaot4x4	
	float4x4 view;
	matrix projection;
}

cbuffer WorldBuffer : register(b1)
{
	matrix world;
}

cbuffer ColorBuffer : register(b5)
{
	float4 color;
}

struct VertexInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PixelInput VS( VertexInput input )
{	
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.color = input.color;
		
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	return input.color * color;
}