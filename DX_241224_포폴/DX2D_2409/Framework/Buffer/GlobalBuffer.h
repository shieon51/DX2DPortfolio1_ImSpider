#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
	MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
	{
		matrix = XMMatrixIdentity();
	}

	void Set(Matrix value)
	{
		matrix = XMMatrixTranspose(value);
	}

private:
	Matrix matrix;
};

class IntValueBuffer : public ConstBuffer
{
public:
	IntValueBuffer() : ConstBuffer(values, sizeof(int) * 4)
	{
	}

	int* Get() { return values; }

private:
	int values[4] = {};
};

class FloatValueBuffer : public ConstBuffer
{
public:
	FloatValueBuffer() : ConstBuffer(values, sizeof(int) * 4)
	{
	}

	float* Get() { return values; }

private:
	float values[4] = {};
};

class ColorValueBuffer : public ConstBuffer
{
public:
	ColorValueBuffer() : ConstBuffer(&values, sizeof(Float4))
	{
	}

	Float4& Get() { return values; }

private:
	Float4 values = {};
};


//class OutlineBuffer : public ConstBuffer
//{
//private:
//	struct Data
//	{
//		Float4 color;
//
//		Float2 imageSize;
//		int scale;
//		float padding;
//	};
//
//public:
//	FloatValueBuffer() : ConstBuffer(&data, sizeof(Data))
//	{
//	}
//
//	float* Get() { return values; }
//
//private:
//	Data data;
//};