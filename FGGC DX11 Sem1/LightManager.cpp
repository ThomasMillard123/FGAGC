#include "LightManager.h"


LightManager::LightManager(DirectX::XMFLOAT3 LightPos, DirectX::XMFLOAT4 DiffuseLight, DirectX::XMFLOAT4 AmbientLight, DirectX::XMFLOAT4 SpecularLight)
{	//set Light data to be used
	cLightData.AmbientLight = AmbientLight;
	cLightData.DiffuseLight = DiffuseLight;
	cLightData.SpecularLight = SpecularLight;
	cLightData.LightVecW = LightPos;
}

LightManager::LightManager(LightData lightData)
{
	cLightData = lightData;
}

LightManager::~LightManager()
{
}

LightData LightManager::GetLightData()
{
	return cLightData;
}

void LightManager::UpdateLight(LightData UpdateLightData)
{
	cLightData = UpdateLightData;
}

void LightManager::SetLightPos(DirectX::XMFLOAT3 LightPos)
{
	cLightData.LightVecW = LightPos;
}

void LightManager::SetDiffuse(DirectX::XMFLOAT4 DiffuseLight)
{
	cLightData.DiffuseLight = DiffuseLight;
}

void LightManager::SetAmbient(DirectX::XMFLOAT4 AmbientLight)
{
	cLightData.AmbientLight = AmbientLight;
}

void LightManager::SetSpecular(DirectX::XMFLOAT4 SpecularLight)
{
	cLightData.SpecularLight = SpecularLight;
}
