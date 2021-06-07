#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include <vector>
#include "resource.h"

#include"Screen.h"
#include"ScreenMenu.h"
#include"ScreenGameLeveL.h"

#include"Structs.h"
#include"OBJLoader.h"

#include"GameObject.h"
#include"CameraManager.h"



using namespace DirectX;

class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11Buffer*           _pConstantBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;
	Screen* cpCurrentScreen;
	ID3D11SamplerState* _pSamplerLinear;
	

	ID3D11RasterizerState* cpWireFrame;

	bool cIsRunning;
	


	
	

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();

	UINT cWindowHeight;
	UINT cWindowWidth;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);
	bool GetIsRunning();
	void Update();
	void Draw();
};

