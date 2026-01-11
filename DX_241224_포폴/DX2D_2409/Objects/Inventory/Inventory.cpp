#include "Framework.h"

Inventory::Inventory() : Quad(L"Textures/UI/Pack/panel_glass.png")
{
    FONT->AddColor("Inven", 0, 0, 1);
    FONT->AddStyle("Inven", L"배달의민족 연성", 40);

    tag = "Inven";
    UpdateSize(size * Vector2(5, 10));
    pos = { 800, 350 };

    EventManager::Get()->AddIntParamEvent("Buy",
        bind(&Inventory::Buy, this, placeholders::_1));
    EventManager::Get()->AddEvent("Sell",
        bind(&Inventory::Sell, this));

    selectImage = new Quad(L"Textures/UI/ItemSelect.png");    
}

Inventory::~Inventory()
{
    for (Item* item : items)
        delete item;

    delete selectImage;
}

void Inventory::Update()
{
    for (Item* item : items)
        item->Update();

    UpdateWorld();

    selectImage->Rotate(DELTA);
    selectImage->UpdateWorld();
}

void Inventory::Render()
{
    Quad::Render();

    for (Item* item : items)
        item->Render();    

    if(selectItem)
        selectImage->Render();
}

void Inventory::PostRender()
{
    if (!IsActive()) return;

    FONT->SetColor("Inven");
    FONT->SetStyle("Inven");

    Vector2 offset(- 100, -250);
    string goldStr = "Gold : " + to_string(gold);
    FONT->RenderText(goldStr, pos + offset);
}

bool Inventory::Buy(int key)
{
    if (key == 0)
    {
        MessageBox(hWnd, L"아이템을 선택하세요!", L"Error", MB_OK);
        return false;
    }

    ItemData data = DataManager::Get()->GetItemData(key);

    if (data.price > gold)
    {
        MessageBox(hWnd, L"골드 부족!", L"Error", MB_OK);
        return false;
    }

    gold -= data.price;
    Item* item = new Item(key);
    item->SetParent(this);
    item->SetObjParamEvent(bind(&Inventory::SelectItem, this, placeholders::_1));
    item->SetObjParam(item);

    items.push_back(item);

    Sort();
}

void Inventory::Sell()
{
    if (selectItem == nullptr)
    {
        MessageBox(hWnd, L"아이템을 선택하세요!", L"Error", MB_OK);
        return;
    }

    list<Item*>::iterator findItem = find(items.begin(), items.end(), selectItem);
    
    if (findItem != items.end())
    {
        MessageBox(hWnd, L"아이템을 판매했습니다!", L"OK", MB_OK);
        gold += selectItem->GetData().price / 2;
        items.erase(findItem);        
    
        selectItem = nullptr;
    }

    //list<Item*>::iterator itemIter = items.begin();
    //
    //for (; itemIter != items.end(); )
    //{
    //    if (*itemIter == selectItem)
    //    //if ((*itemIter)->GetData().price <= 1000)
    //    {
    //        MessageBox(hWnd, L"아이템을 판매했습니다!", L"OK", MB_OK);
    //        gold += selectItem->GetData().price / 2;
    //        itemIter = items.erase(itemIter);
    //    }
    //    else
    //    {
    //        itemIter++;
    //    }
    //}

    Sort();
}

void Inventory::SelectItem(void* item)
{
    selectItem = (Item*)item;

    selectImage->SetPos(selectItem->GetGlobalPos());
}

void Inventory::Sort()
{
    Vector2 startPos = { -70, 230 };
    Vector2 interval = { 80, -80 };
    Vector2 coord;

    int count = 0;
    for (Item* item : items)
    {
        coord.x = count % 3;
        coord.y = count / 3;

        item->SetPos(startPos + interval * coord);

        count++;
    }    
}
