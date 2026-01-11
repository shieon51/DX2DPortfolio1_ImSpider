#pragma once

struct ItemData
{
    UINT key;
    string name;
    UINT price;
    string imageFile;
};

class DataManager : public Singleton<DataManager>
{
private:
    friend class Singleton;

    DataManager();
    ~DataManager();

public:
    ItemData GetItemData(UINT key)
    {
        if (itemDatas.count(key) == 0)
            assert(false);

        return itemDatas[key];
    }
    //{ return itemDatas[key]; }

    unordered_map<UINT, ItemData> GetItemDatas() { return itemDatas; }
private:
    void LoadItemData();

private:
    unordered_map<UINT, ItemData> itemDatas;
};