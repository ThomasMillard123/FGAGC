#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include "resource.h"
#include"Structs.h"


class LightManager
{
public:
	LightManager(DirectX::XMFLOAT3 LightPos, DirectX::XMFLOAT4 DiffuseLight,  DirectX::XMFLOAT4 AmbientLight,  DirectX::XMFLOAT4 SpecularLight);
	LightManager(LightData lightData);

	~LightManager();


	//get Light data
	LightData GetLightData();

	//set Light Componets
	void UpdateLight(LightData UpdateLightData);
	
	void SetLightPos(DirectX::XMFLOAT3 LightPos);

	void SetDiffuse( DirectX::XMFLOAT4 DiffuseLight);
	void SetAmbient( DirectX::XMFLOAT4 AmbientLight);
	void SetSpecular( DirectX::XMFLOAT4 SpecularLight);

private:
	LightData cLightData;

};

