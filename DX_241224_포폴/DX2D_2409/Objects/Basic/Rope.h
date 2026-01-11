#pragma once

class Rope : public GameObject
{
public:
	Rope(Vector2 size = { 100, 100 });
	Rope(wstring textureFile, Vector2 startUV = Vector2(), Vector2 endUV = Vector2(1, 1));

	void UpdateSize(Vector2 size);

	Vector2 GetSize() { return size; }
	void Edit();

private:
	void MakeMesh(Vector2 startUV, Vector2 endUV);

protected:
	Vector2 size;
	Vector2 originSize;
};