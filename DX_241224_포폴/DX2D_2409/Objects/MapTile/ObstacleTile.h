#pragma once

class ObstacleTile : public EditTileObject
{
public:
	ObstacleTile(Vector2 size, InstanceData* data);
	~ObstacleTile();

	void CollisionCheck() override;
	void SilkCollision(const Vector2& overlap) override;

};