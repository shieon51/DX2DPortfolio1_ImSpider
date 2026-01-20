#pragma once

class SpiderSilk : public Rope
{
public:
	SpiderSilk();
	~SpiderSilk();

	//일반적인 실의 경우는 부모 함수를 이용(적의 실 등)
	void Update();
	void Render();

	void SetInitPos(Vector2 attachPos, Vector2 firstPos)
	{
		this->attachPos = attachPos;
		this->firstPos = firstPos;
		firstSilkLength = Vector2::Distance(attachPos, firstPos);

		pos = attachPos;
		size.x = firstSilkLength;
		SetRot(0, 0, PI * 0.5);
		UpdateSilk();
	}

	void UpdateSilk();

protected:
	LineCollider* line;

	Vector2 attachPos; //부착한 위치
	Vector2 firstPos; //부착시 초기 플레이어 위치(최고점)
	float firstSilkLength; // 부착 당시 최초 거미줄 길이 
};