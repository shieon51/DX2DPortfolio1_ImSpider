#include "Framework.h"
#include "SpiderSilk.h"

SpiderSilk::SpiderSilk() : Rope(L"Textures/Spider/SpiderSilk.png")
{
	size.x = 0;
	line = new LineCollider(0, 0);
	line->SetParent(this);
}

SpiderSilk::~SpiderSilk()
{
	delete line;
}

void SpiderSilk::Update()
{
	UpdateWorld();
	line->UpdateWorld();
}

void SpiderSilk::Render()
{
	Rope::Render();
	line->Render();
}

void SpiderSilk::UpdateSilk()
{
	Rope::UpdateSize(size);
	line->UpdateStartAndEndPos(Vector2(), Vector2(-size.x, 0));
}

