#pragma once
#include<Windows.h>

#include"Keyboard.h"
#include"MouseInput.h"
#include"Mouse_Picking.h"
#include"EventSystem.h"

#include"Defines.h"
/// <summary>
/// Class to hold all inputs need by application
/// </summary>
class InputControllor
{

public:
	InputControllor();
	~InputControllor();

	bool HandleInput(MSG msg);
	void Update();
	void CharInputUpdate();
	void KeyInputUpdate();
	void MouseInputUpdate();
private:
	unsigned char ch;
	Keyboard _KeyInput;
	MouseInput _MouseInput;
	
};

