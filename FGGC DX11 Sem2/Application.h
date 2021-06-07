#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include "Camera.h"
#include "Structures.h"
#include "OBJLoader.h"


#include"Defines.h"
#include"Vector3.h"

#include"GameObjectControllor.h"
#include "GameObject.h"
#include"Terrain.h"

#include"ParticalSystem_Manager.h"
#include"ParticalSystem.h"

#include"Debug.h"

//objectCreation
#include"Transform.h"
#include"Appearance.h"
#include"ParticleModel.h"
#include"RigidBody.h"

//input
#include"InputControllor.h"

#include"Shader_VS_PS.h"

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

	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	ID3D11ShaderResourceView * _pTextureRV = nullptr;
	ID3D11ShaderResourceView* _pHerculesTextureRV = nullptr;


	MeshData objMeshData;

	Light basicLight;

	//partical system
	ParticalSystem_Manager* _ParticalSystemManager;
	//game world/objects
	GameObjectControllor* _pGameObjectControllor;
	Terrain* _Terrain;

	//camera
	Camera * _camera = nullptr;
	
	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

	//input 
	InputControllor* _pInputConrollor;
	
	Shader_VS_PS* DefualtShader;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update();
	void Draw();
};

