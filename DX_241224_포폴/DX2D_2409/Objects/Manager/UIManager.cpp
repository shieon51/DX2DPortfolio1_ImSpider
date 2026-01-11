#include "Framework.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

bool UIManager::IsContain(string strName, Vector2 curPos)
{
    //해당 판넬이 꺼져있으면 체크안함
    if (!panels[strName]->IsOn()) return false;

    Vector2 panelHalfSize = panels[strName]->GetSize() * 0.5f;
    Vector2 panelPos = panels[strName]->GetPos();

    //UI 바깥이면 false
    if (curPos.x < panelPos.x - panelHalfSize.x) return false;
    if (curPos.x > panelPos.x + panelHalfSize.x) return false;
    if (curPos.y < panelPos.y - panelHalfSize.y) return false;
    if (curPos.y > panelPos.y + panelHalfSize.y) return false;

    //아니면 true
    return true;
}
