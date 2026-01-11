#pragma once

class SpiderMoveDirection : public SpiderAction
{
public:
	SpiderMoveDirection(SpiderPlayer* player);

	void Update() override;
};