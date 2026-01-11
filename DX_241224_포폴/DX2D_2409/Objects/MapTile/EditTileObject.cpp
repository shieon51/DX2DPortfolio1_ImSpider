#include "Framework.h"

EditTileObject::EditTileObject(Vector2 size, InstanceData* data)
	: BoxCollider(size), instanceData(data)
{
	SetTag("EditTileObject");
}

EditTileObject::~EditTileObject()
{
	  
}

void EditTileObject::Update() //
{
	if (!IsActive()) return;

	Vector2 overlap;
	//++ 위치 나중에 옮기기
	if (IsLineCollision(silkCollider, &overlap))
	{
		if (player->IsHanging() && overlap == player->GetSpiderSilk()->GetAttachPos())
			return; //현재 매달려있는 상태 & 현재 충돌부분이 이미 부착된 위치와 같으면 무시 

		if (overlap.y < GetGlobalPos().y + HalfSize().y
			&& overlap.x > GetGlobalPos().x - HalfSize().x 
			&& overlap.x < GetGlobalPos().x + HalfSize().x) //아래만 부착가능 / 아직 매달린 상태가 아니면
		{
			SilkCollision(overlap); //장애물 별 충돌처리
		}
		else //만약 뻗은 줄이 다른 장애물에 충돌이 나면 -> 줄 끊기
		{
			player->WebCut();
		}
	}
}

