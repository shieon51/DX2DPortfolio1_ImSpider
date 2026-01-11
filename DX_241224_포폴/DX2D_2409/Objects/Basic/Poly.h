#pragma once

class Poly : public GameObject
{
public:
    Poly(int edge, float size = 50.0f);
    Poly(int edge, wstring textureFile);

private:
    void MakeMesh();

private:
    int edge;
    float size;
};