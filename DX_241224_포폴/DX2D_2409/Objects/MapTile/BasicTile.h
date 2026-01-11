#pragma once

class BasicTile : public EditTileObject
{
public:
	BasicTile(Vector2 size, InstanceData* data);
	~BasicTile();

	void CollisionCheck() override;
	void SilkCollision(const Vector2& overlap) override;

};