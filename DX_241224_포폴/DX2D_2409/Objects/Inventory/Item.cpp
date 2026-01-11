#include "Framework.h"

Item::Item(UINT key)
{
    data = DataManager::Get()->GetItemData(key);

    string path = "Textures/UI/Item/" + data.imageFile;
    image->GetMaterial()->SetTexture(Utility::ToWString(path));
    image->UpdateSize({ 50, 50 });
    UpdateSize({ 50, 50 });
}

Item::~Item()
{
}
