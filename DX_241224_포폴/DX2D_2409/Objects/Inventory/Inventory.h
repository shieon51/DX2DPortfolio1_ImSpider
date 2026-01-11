#pragma once

class Inventory : public Quad
{
public:
    Inventory();
    ~Inventory();

    void Update();
    void Render();
    void PostRender();

    bool Buy(int key);    
    void Sell();

    void SelectItem(void* item);

private:
    void Sort();

private:
    UINT gold = 5000;
    list<Item*> items;
    Quad* selectImage;

    Item* selectItem = nullptr;
};