#pragma once
class TimeManager
{
public:
	void Init();
	void Update();
	float GetDeltaTime() { return _deltaTime; }

private:
	unsigned int _frequency = 0;
	unsigned int _prevCount = 0;
	float _deltaTime = 0.f;
};

