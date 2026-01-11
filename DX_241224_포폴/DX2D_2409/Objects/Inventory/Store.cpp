#include "Framework.h"
#include "Store.h"

Store::Store() : Quad(L"Textures/UI/Pack/panel_glass.png")
{
    tag = "Store";
    UpdateSize(size * Vector2(5, 10));
    pos = { 400, 350 };
    CreateGoods();

    buyButton = new Button();
    buyButton->SetParent(this);
    buyButton->SetText("Buy");
    buyButton->SetTag("BuyButton");
    buyButton->Load();    
    buyButton->SetEvent(bind(&Store::OnClickBuy, this));

    sellButton = new Button();
    sellButton->SetParent(this);
    sellButton->SetText("Sell");
    sellButton->SetTag("SellButton");
    sellButton->Load();
    sellButton->SetEvent(bind(&Store::OnClickSell, this));
}

Store::~Store()
{
    for (Button* good : goods)
        delete good;
}

void Store::Update()
{
    for (Button* good : goods)
        good->Update();

    buyButton->Update();
    sellButton->Update();

    UpdateWorld();
}

void Store::Render()
{
    Quad::Render();

    for (Button* good : goods)
        good->Render(); 

    buyButton->Render();
    sellButton->Render();
}

void Store::PostRender()
{
    if (!IsActive()) return;

    Vector2 offset = { -100, -250 };
    Vector2 pricePos = pos + offset;

    string str;
    ItemData data;

    if (selectKey)
    {
        data = DataManager::Get()->GetItemData(selectKey);
        str = "Price : " + to_string(data.price);
    }    

    Font::Get()->SetColor("Store");
    Font::Get()->SetStyle("Store");
    FONT->RenderText(str, pricePos);
    pricePos += Vector2::Up()*30;
    FONT->RenderText(data.name, pricePos);

    buyButton->Edit();
    sellButton->Edit();
}

void Store::CreateGoods()
{
    unordered_map<UINT, ItemData> itemDatas = DataManager::Get()->GetItemDatas();

    Vector2 startPos = { -70, 230 };
    Vector2 buttonSize = { 80, 80 };

    int count = 0;
    for (pair<UINT, ItemData> itemData : itemDatas)
    {
        string path = "Textures/UI/Item/" + itemData.second.imageFile;

        Button* button = new Button(Utility::ToWString(path));
        button->SetParent(this);
        button->UpdateSize(buttonSize);
        button->GetImage()->UpdateSize(buttonSize);
        button->SetPos(startPos + Vector2::Down() * 100 * count);
        button->SetIntParamEvent(
            bind(&Store::OnClickGood, this, placeholders::_1));
        button->SetIntParam(itemData.first);

        goods.push_back(button);

        count++;
    }
}

void Store::OnClickGood(int key)
{
    selectKey = key;
}

void Store::OnClickBuy()
{
    EventManager::Get()->ExcuteIntParamEvent("Buy", selectKey);
}

void Store::OnClickSell()
{
    EventManager::Get()->ExcuteEvent("Sell");
}
