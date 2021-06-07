#pragma once
/// <summary>
/// 3D menu
/// Picking
/// Mulit camra Topdown and front
/// Cubes for butions
/// </summary>
#include<iostream>
#include<algorithm>
#include"Screen.h"

class ScreenMenu : public Screen
{

public:

	ScreenMenu(ID3D11Device* d3dDevice, ID3D11DeviceContext* immediateContext, ID3D11Buffer* constantBuffer, UINT windowHeight, UINT windowWidth);
	~ScreenMenu();

	 void Update(float t);
	 void Draw(ConstantBuffer cb);

	 void SetUp();
	 void UpdateCb(ConstantBuffer& cb);
private:
	//3D menu

	
	void Picking();
};


