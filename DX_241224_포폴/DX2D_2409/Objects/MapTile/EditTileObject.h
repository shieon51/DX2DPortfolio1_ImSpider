#pragma once

class EditTileObject : public BoxCollider
{
public:
	EditTileObject(Vector2 size, InstanceData* data);
	~EditTileObject();

	void Update();
	virtual void CollisionCheck() = 0;
	virtual void SilkCollision(const Vector2 &overlap) = 0;

	void SetSilkCollider(LineCollider* spiderSilk) { this->silkCollider = spiderSilk; }
	void SetPlayer(class SpiderPlayer* player) { this->player = player; }

	InstanceData* GetInstanceData() { return instanceData; }
	
	int GetTileType() { return tileType; }
	void SetTileType(int type) { tileType = type; }

	bool GetIsEdited() { return isEdited; }
	void SetIsEdited(bool edited) { isEdited = edited; }


protected:
	InstanceData* instanceData;
	int tileType;
	bool isEdited = false; //해당 타일이 수정되었는지

	LineCollider* silkCollider;
	class SpiderPlayer* player;
};