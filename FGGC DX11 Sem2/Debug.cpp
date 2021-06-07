#include "Debug.h"

Debug::Debug()
{
}

Debug::~Debug()
{
}

void Debug::OutputFPS(float time)
{
	float FPS = 1 / time;
	sprintf_s(sz, "FPS: %f \n", FPS);
	OutputDebugStringA(sz);
}

void Debug::OutputString(string Text)
{

	sprintf_s(sz, "Text: %s \n", Text.c_str());
	OutputDebugStringA(sz);
}

void Debug::OuputFloat(float Float)
{
	
	sprintf_s(sz, "Number:  %f \n", Float);
	OutputDebugStringA(sz);
}

void Debug::OuputInt(int Interger)
{
	sprintf_s(sz, "Number: %i \n", Interger);
	OutputDebugStringA(sz);
}
