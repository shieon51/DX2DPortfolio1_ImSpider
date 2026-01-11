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

cbuffer OutlineBuffer : register(b3)
{
	float2 imageSize;
	float weight;
}

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
		
	return output;
}

Texture2D map : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{
	float2 uv = input.uv + curFrame / maxFrame;
	
	float4 baseColor = map.Sample(samp, uv);
	
	if (baseColor.r == removeColor.r &&
		baseColor.g == removeColor.g &&
		baseColor.b == removeColor.b)
		baseColor.a = 0.0f;
	
	float count = 0;
	
	if(baseColor.a > 0.0f)
		return baseColor;
	
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			float2 offset = (float2(x, y) / imageSize) * weight;
			count += map.Sample(samp, input.uv + offset).a;
		}
	}
	
	if (count > 0 && count < 9)
		return tintColor;
	
	return baseColor;
}