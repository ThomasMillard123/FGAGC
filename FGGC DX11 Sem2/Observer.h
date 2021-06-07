#pragma once
#include"Events_Enum.h"
/// <summary>
/// tmeplate for other observors to inherrit off
/// </summary>
class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify(const void* entertity, Events event) = 0;
};

