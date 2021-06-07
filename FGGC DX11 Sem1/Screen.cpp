#include "Screen.h"

Screen::Screen(ID3D11Device* pd3dDevice, ID3D11DeviceContext* ImmediateContext, ID3D11Buffer* _pConstantBuffer, UINT WindowHeight, UINT WindowWidth)
{
	cpD3dDevice = pd3dDevice;
	cpImmediateContext = ImmediateContext;
	cpConstantBuffer = _pConstantBuffer;
	cWindowHeight = WindowHeight;
	cWindowWidth = WindowWidth;
	XMStoreFloat4x4(&cpWorld, XMMatrixIdentity());
	cCurrentCamera = 0;
	cCurrentLight = 0;
}

Screen::~Screen()
{
	CleanUp();
}

void Screen::CleanUp()
{
	for (int i = 0; i < cpCamera.size(); i++) {
		delete cpCamera[i];
	}
	 cpCamera.clear();

	 for (int i = 0; i < cpGameObjects.size(); i++) {
		 delete cpGameObjects[i];
	 }
	 cpGameObjects.clear();

	for (int i = 0; i < cpLight.size(); i++) {
		delete cpLight[i];
	}
	cpLight.clear();

	cpShader = nullptr;
	
	 cpTransparency = nullptr;
	 cpConstantBuffer = nullptr;
	cpD3dDevice = nullptr;
	cpImmediateContext = nullptr;
	cpSolidFill = nullptr;
}

void Screen::SetScreenDimentions(UINT WindowHeight, UINT WindowWidth)
{
	cWindowHeight = WindowHeight;
	cWindowWidth = WindowWidth;
}

void Screen::SetMoucePos(POINT Mouse)
{
	cMousePos = Mouse;
}

void Screen::Update(float t)
{
	cpCamera[cCurrentCamera]->Reshape(cWindowWidth, cWindowHeight, 0.01f, 1000.0f);
	int camSelected=-1;


	if (GetAsyncKeyState(VK_NUMPAD0)) {
		camSelected = 0;
	}
	else if (GetAsyncKeyState(VK_NUMPAD1)) {
		camSelected = 1;
	}
	else if (GetAsyncKeyState(VK_NUMPAD2)) {
		camSelected = 2;
	}
	else if (GetAsyncKeyState(VK_NUMPAD3)) {
		camSelected = 3;
	}
	else if (GetAsyncKeyState(VK_NUMPAD4)) {
		camSelected = 4;
	}


	if (camSelected == -1) {

	}
	else if (camSelected < cCameraData.size()) {
		cCurrentCamera = camSelected;
	}
	else
	{
		
	}


	cFrameTime = t;
}

void Screen::Draw(ConstantBuffer cb)
{

}

void Screen::SetUp()
{
}

void Screen::UpdateCb(ConstantBuffer& cb)
{
	cb.View = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetViewMatrix()));
	cb.Projection = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetProjectionMatrix()));

	//light Data
	cb.LightVecW = cpLight[cCurrentLight]->GetLightData().LightVecW;
	cb.DiffuseLight = cpLight[cCurrentLight]->GetLightData().DiffuseLight;
	cb.AmbientLight = cpLight[cCurrentLight]->GetLightData().AmbientLight;
	cb.SpecularLight = cpLight[cCurrentLight]->GetLightData().SpecularLight;
	cb.EyePos = cpCamera[cCurrentLight]->GetEye();

	//extra data
	cb.gTime = cFrameTime;
}

bool Screen::IsNewScreen()
{
	return cIsChangeScreen;
}

Screens Screen::GetNextScreen()
{
	return cNextScreen;
}
