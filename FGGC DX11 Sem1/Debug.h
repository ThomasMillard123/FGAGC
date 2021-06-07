#pragma once
class Debug
{
public:
	Debug();
	~Debug();

	void NumberOfObjects();
	void FPS();

private:
	float _FPS;
	__int32 _TotalObjects;
};

