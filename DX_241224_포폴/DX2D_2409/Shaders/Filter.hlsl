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

cbuffer ValueBuffer : register(b2)
{
	int select;
	int scale;
	
	int2 imageSize;
}

cbuffer ValueBuffer : register(b3)
{
	float3 radialScale;
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

float4 Grayscale(float4 baseColor)
{
	float scale = (baseColor.r + baseColor.b + baseColor.g) / 3;

	return float4(scale.xxx, baseColor.a);
}

float4 Grayscale2(float4 baseColor)
{
	//float3 grayColor;
	//grayColor.r = baseColor.r * 0.3f;
	//grayColor.g = baseColor.g * 0.59f;
	//grayColor.b = baseColor.b * 0.11f;
	//
	//float scale = grayColor.r + grayColor.g + grayColor.b;	
	
	float scale = dot(baseColor.rgb, float3(0.3f, 0.59f, 0.11f));	

	return float4(scale.xxx, baseColor.a);
}

float4 Sepia(float4 baseColor)
{
	float3 sepia;
	
	sepia.r = dot(baseColor.rgb, float3(0.393f, 0.769f, 0.189f));
	sepia.g = dot(baseColor.rgb, float3(0.349f, 0.646f, 0.168f));
	sepia.b = dot(baseColor.rgb, float3(0.272f, 0.534f, 0.131f));

	return float4(sepia, baseColor.a);
}

float4 Mosaic(float2 uv)
{
	float2 temp = floor(uv * scale) / scale;

	return map.Sample(samp, temp);
}

float4 Blur(float2 uv)
{
	float4 result = 0;
	
	[unroll(16)]
	for (int i = 0; i < scale; i++)
	{
		float2 div = (1 + i) / (float2) imageSize;

		result += map.Sample(samp, float2(uv.x + div.x, uv.y));
		result += map.Sample(samp, float2(uv.x - div.x, uv.y));
		result += map.Sample(samp, float2(uv.x, uv.y + div.y));
		result += map.Sample(samp, float2(uv.x, uv.y - div.y));
	}

	return result /= scale * 4;
}

static const float2 edges[8] =
{
	float2(-1, -1), float2(0, -1), float2(+1, -1),
	float2(-1, 0), float2(+1, 0),
	float2(-1, +1), float2(0, +1), float2(+1, +1),
};

float4 Blur2(float2 uv)
{
	float4 result = 0;
	
	for (int i = 0; i < scale; i++)
	{
		float2 div = (1 + i) / (float2) imageSize;
		
		for (int j = 0; j < 8; j++)
		{
			float2 xy = edges[j] * div + uv;
			
			result += map.Sample(samp, xy);
		}
	}
	
	return result /= scale * 8;
}

static const float weights[13] =
{
	0.0561f, 0.1353f, 0.2730f, 0.4868f, 0.7261f, 0.9231f,
	1.0f,
	0.9231f, 0.7261f, 0.4868f, 0.2730f, 0.1353f, 0.0561f
};

float4 GaussianBlur(float2 uv)
{
	float2 div = 1.0f / imageSize;
	
	float sum = 0.0f;
	float4 result = 0;
	
	for (int i = -6; i <= 6; i++)
	{
		float2 temp = uv + float2(div.x * i, 0);
		result += weights[6 + i] * map.Sample(samp, temp);
		
		temp = uv + float2(0, div.y * i);
		result += weights[6 + i] * map.Sample(samp, temp);

		sum += weights[6 + i] * 2.0f;
	}
	
	result /= sum;
	
	return result;
}

float4 RadialBlur(float2 uv)
{
	float2 radiusUV = uv - float2(0.5f, 0.5f);
	float r = length(radiusUV);
	radiusUV /= r;
	
	r = saturate(r / radialScale.y);
	
	float2 delta = -radiusUV * r * r * radialScale.z / radialScale.x;
	
	float4 result = 0;	
	
	for (int i = 0; i < radialScale.x; i++)
	{
		uv += delta;
		result += map.Sample(samp, uv);
	}
	
	return result /= radialScale.x;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float2 uv = input.uv + curFrame / maxFrame;
	
	float4 baseColor = map.Sample(samp, uv);
	
	if (baseColor.r == removeColor.r &&
		baseColor.g == removeColor.g &&
		baseColor.b == removeColor.b)
		baseColor.a = 0.0f;	
	
	if(select == 1)
		baseColor = Grayscale(baseColor);
	else if (select == 2)
		baseColor = Grayscale2(baseColor);
	else if (select == 3)
		baseColor = Sepia(baseColor);
	else if (select == 4)
		baseColor = Mosaic(input.uv);
	else if (select == 5)
		baseColor = Blur(input.uv);
	else if (select == 6)
		baseColor = Blur2(input.uv);
	else if (select == 7)
		baseColor = GaussianBlur(input.uv);
	else if (select == 8)
		baseColor = RadialBlur(input.uv);
	
	return baseColor * tintColor;
}