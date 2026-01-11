#pragma once

class GoalTile : public EditTileObject
{
public:
	GoalTile(Vector2 size, InstanceData* data);
	~GoalTile();

	void CollisionCheck() override;
	void SilkCollision(const Vector2& overlap) override {};

};