//Instancing : 같은 메쉬를 가지는 객체 여러개를 한번에 드로우 하는 방법

//VS ConstantBuffer
cbuffer VPBuffer : register(b0)
{
	float4x4 view;
	matrix projection;
}

cbuffer WorldBuffer : register(b1)
{
	matrix world;
}

//PS ConstantBuffer
cbuffer ColorBuffer : register(b0)
{
	float4 tintColor;
	float4 removeColor;
}

cbuffer FrameBuffer : register(b1)
{
	float2 curFrame;
	float2 maxFrame;
}

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	
	matrix transform : INSTANCE_TRANSFORM;
	float2 curFrame : INSTANCE_CURFRAME;
	float2 maxFrame : INSTANCE_MAXFRAME;
	//int textureIndex : BINORMAL;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	
	float2 curFrame : INSTANCE_CURFRAME;
	float2 maxFrame : INSTANCE_MAXFRAME;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, input.transform);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.curFrame = input.curFrame;
	output.maxFrame = input.maxFrame;
		
	return output;
}

Texture2D map : register(t0);
//Texture2DArray maps : 
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{
	float2 uv = input.uv + input.curFrame / input.maxFrame;
	
	float4 baseColor = map.Sample(samp, uv);	
		
	if (baseColor.r == removeColor.r &&
		baseColor.g == removeColor.g &&
		baseColor.b == removeColor.b)
		baseColor.a = 0.0f;
	
	return baseColor * tintColor;
}