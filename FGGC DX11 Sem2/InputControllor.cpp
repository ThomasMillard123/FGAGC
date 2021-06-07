#include "InputControllor.h"

InputControllor::InputControllor()
{
}

InputControllor::~InputControllor()
{
}

bool InputControllor::HandleInput(MSG msg)
{
	//get input from windows
	switch (msg.message)
	{
		//Keyborad Input
	case WM_KEYUP: {
		unsigned char ch = static_cast<unsigned char>(msg.wParam);
		_KeyInput.OnKeyRelace(ch);
		return true;
	}
				 break;
	case WM_KEYDOWN: {
		unsigned char ch = static_cast<unsigned char>(msg.wParam);
		if (_KeyInput.IsKeysAutoRepat()) {

			_KeyInput.OnKeyPress(ch);
		}
		else
		{
			const bool wasPressed = msg.lParam & WasP;
			if (!wasPressed) {
				_KeyInput.OnKeyPress(ch);
			}

		}
		return true;
	}
				   break;
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(msg.wParam);
		if (_KeyInput.IsCharAutoRepat()) {
			_KeyInput.OnChar(ch);
		}
		else
		{
			const bool wasPressed = msg.lParam & WasP;
			if (!wasPressed) {
				_KeyInput.OnChar(ch);
			}

		}
		return true;
	}
	break;
	//mouse input
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnMouseMove(x, y);
		return true;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnLeftPress(x, y);
		return true;
	}
	break;
	case WM_LBUTTONUP:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnLeftReleased(x, y);
		return true;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnRightPress(x, y);
		return true;
	}
	break;
	case WM_RBUTTONUP:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnRightReleased(x, y);
		return true;
	}
	break;
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnMiddlePress(x, y);
		return true;
	}
	break;
	case WM_MBUTTONUP:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);

		_MouseInput.OnMiddleReleased(x, y);
		return true;
	}
	break;
	case WM_MOUSEHWHEEL:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		if (GET_WHEEL_DELTA_WPARAM(msg.wParam) > 0) {
			_MouseInput.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(msg.wParam) < 0) {
			_MouseInput.OnWheelDown(x, y);
		}



		return true;
	}
	break;

	case WM_INPUT:
	{
		UINT dataSize = 0u;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(msg.lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(msg.lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					_MouseInput.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}


		return DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
	break;
	}
	return false;
}

void InputControllor::Update()
{
	CharInputUpdate();
	KeyInputUpdate();
	MouseInputUpdate();
}

void InputControllor::CharInputUpdate()
{
	//keyborad input
	while (!_KeyInput.CharBufferIsEmpty())
	{
		unsigned char ch = _KeyInput.Raedchar();

		_KeyInput.EnableAutoRepeatChars();

	}
	

}

void InputControllor::KeyInputUpdate()
{
	while (!_KeyInput.KeyBufferIsEmpty())
		{
			KeyboardEvent keyEvent = _KeyInput.ReadKey();
			 ch = keyEvent.GetKeyCode();

			_KeyInput.EnableAutoRepeatKeys();

			//cam controlls
			if (_KeyInput.KeyIsPress(VK_UP)) {
				EventSystem::Instance()->AddEvent(Events::UpInput);
			}
			if (_KeyInput.KeyIsPress(VK_DOWN)) {
				EventSystem::Instance()->AddEvent(Events::DownInput);
			}
			if (_KeyInput.KeyIsPress(VK_RIGHT)) {
				EventSystem::Instance()->AddEvent(Events::RightInput);
			}
			if (_KeyInput.KeyIsPress(VK_LEFT)) {
				EventSystem::Instance()->AddEvent(Events::LeftInput);
			}

			//object controlls
			if (_KeyInput.KeyIsPress('0') || _KeyInput.KeyIsPress(VK_NUMPAD0))
			{
				EventSystem::Instance()->AddEvent(Events::NumPress, &ch);
			}
			if (_KeyInput.KeyIsPress('1') || _KeyInput.KeyIsPress(VK_NUMPAD1))
			{
				EventSystem::Instance()->AddEvent(Events::NumPress, &ch);
			}
			if (_KeyInput.KeyIsPress('2')|| _KeyInput.KeyIsPress(VK_NUMPAD2))
			{
				EventSystem::Instance()->AddEvent(Events::NumPress,&ch);
			}
			
			if (_KeyInput.KeyIsPress('W')) {
				EventSystem::Instance()->AddEvent(Events::WInput);
			}
			else if (_KeyInput.KeyIsPress('S'))
			{
				EventSystem::Instance()->AddEvent(Events::SInput);
			}
			else if (_KeyInput.KeyIsPress('A'))
			{
				EventSystem::Instance()->AddEvent(Events::AInput);
			}
			else if (_KeyInput.KeyIsPress('D'))
			{
				EventSystem::Instance()->AddEvent(Events::DInput);
			}
			if (_KeyInput.KeyIsPress('Q')) {
				EventSystem::Instance()->AddEvent(Events::keyInput,&ch);
			}

		}
}

static XMFLOAT2 mousePos;
void InputControllor::MouseInputUpdate()
{
//mouse input
	while (!_MouseInput.EventBufferIsEmpty())
	{
		MouseEvent mEvent = _MouseInput.ReadEvent();
		
		if (_MouseInput.IsLeftDown())
		{
			// update raw camera movement
			if (mEvent.GetType() == RawMove)
			{
				mousePos = { static_cast<float>(mEvent.GetMouseX()),static_cast<float>(mEvent.GetMouseY()) };
				EventSystem::Instance()->AddEvent(Events::MousePositionRaw, &mousePos);
				EventSystem::Instance()->AddEvent(Events::LeftDown);
		
			}

		}

		if (_MouseInput.IsRightDown())
		{
			mousePos = { (float)mEvent.GetMouseX(), (float)mEvent.GetMouseY() };
			EventSystem::Instance()->AddEvent(Events::MousePosition, &mousePos);

			EventSystem::Instance()->AddEvent(Events::RightDown);


		}
		
	}
}
