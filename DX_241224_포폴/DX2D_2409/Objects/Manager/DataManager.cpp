#include "Framework.h"

DataManager::DataManager()
{
    LoadItemData();
}

DataManager::~DataManager()
{
}

void DataManager::LoadItemData()
{
    string fileName = "DataTables/ItemTable.csv";

    ifstream loadFile(fileName);

    string line;

    getline(loadFile, line);

    while (getline(loadFile, line))
    {
        vector<string> rowData = Utility::SplitString(line, ",");

        ItemData data;
        data.key = stoi(rowData[0]);
        data.name = rowData[1];
        data.price = stoi(rowData[2]);
        data.imageFile = rowData[3];

        itemDatas[data.key] = data;
    }
}
