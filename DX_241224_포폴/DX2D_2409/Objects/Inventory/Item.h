#pragma once

class Item : public Button
{
protected:
    enum EquipType
    {
        Weapon, Armor, Potion, Inventory
    };

public:
    Item(UINT key);
    ~Item();

    ItemData GetData() { return data; }

private:
    ItemData data;
    EquipType curEquipState;
};