#pragma once
#include <string>
#include <Windows.h>

using namespace std;

class Debug
{
public:
	Debug();
	~Debug();

	void OutputFPS(float time);
	void OutputString(string Text);
	void OuputFloat(float Float);
	void OuputInt(int Interger);

private:
	char sz[1024] = { 0 };

};

