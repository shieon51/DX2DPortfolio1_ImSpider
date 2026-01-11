#pragma once

class Store : public Quad
{
public:
    Store();
    ~Store();

    void Update();
    void Render();
    void PostRender();

private:
    void CreateGoods();
    void OnClickGood(int key);

    void OnClickBuy();
    void OnClickSell();

private:
    vector<Button*> goods;
    int selectKey = 0;

    Button* buyButton;
    Button* sellButton;
};