#pragma once

class SpiderAction : public Action
{
public:
	SpiderAction(SpiderPlayer* player) { this->player = player; };

protected:
	SpiderPlayer* player;
};