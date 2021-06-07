#pragma once

/// <summary>
/// Enum for all events requered in the system
/// </summary>
enum class Events
{
	//window data
	WindowSizeUpdate,
	//camData
	EyePos,
	ViewMatix,
	ProjectionMatrix,

	//BufferData
	ConstantBufferSet,

	//input
	keyInput,
	//Keys
	WInput,
	SInput,
	AInput,
	DInput,

	//arrow keys
	UpInput,
	DownInput,
	RightInput,
	LeftInput,
	
	//numPress
	NumPress,

	//mouseinput
	LeftDown,
	RightDown,
	MiddleDown,
	WheelUp,
	WheelDown,
	MousePosition,
	MousePositionRaw
};
