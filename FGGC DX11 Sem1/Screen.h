#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include<DirectXMath.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include<DirectXCollision.h>
#include <directxcolors.h>
#include <vector>

#include "resource.h"
#include"Structs.h"

//Managers
#include"GameObject.h"
#include"CameraManager.h"
#include "LightManager.h"
#include"ShaderManager.h"

//object builders
#include"DDSTextureLoader.h"
#include"ObjectBuilder.h"
#include"OBJLoader.h"

#include"JsonFilePars.h"

enum Screens
{
	Menu,
	Level,
	Exit
};

class Screen
{

public:
	Screen(ID3D11Device* d3dDevice, ID3D11DeviceContext* immediateContext, ID3D11Buffer* ConstantBuffer, UINT windowHeight, UINT windowWidth);
	~Screen();



	void CleanUp();
	void SetScreenDimentions(UINT windowHeight, UINT windowWidth);
	void SetMoucePos(POINT mouse);
	virtual void Update(float t);
	virtual void Draw(ConstantBuffer cb);

	virtual void SetUp();
	virtual void UpdateCb(ConstantBuffer& cb);


	bool IsNewScreen();
	Screens GetNextScreen();

protected:
	std::vector<CameraManager*> cpCamera;
	std::vector<GameObject*> cpGameObjects;
	std::vector<LightManager*> cpLight;

	ID3D11Buffer* cpConstantBuffer;

	//mesh Data
	std::vector<MeshData> cMeshDat;

	//How to Blend objects
	ID3D11BlendState* cpTransparency;

	//how the objects are going to be displayed
	ID3D11RasterizerState* cpSolidFill;
	ID3D11RasterizerState* cpSolidFillNoCull;
	ID3D11RasterizerState* cpWireFrame;
	//the shader
	ShaderManager* cpShader;

	//buffers on what/how to display
	ID3D11Device* cpD3dDevice;
	ID3D11DeviceContext* cpImmediateContext;

	//Maxtixes
	XMFLOAT4X4  cpWorld;
	
	float cFrameTime;


	std::vector< JsonFilePars::ObjectData> cObjectData;
	std::vector< JsonFilePars::CameraData> cCameraData;
	std::vector< JsonFilePars::LightInfo> cLightData;
	MeshData cMesh;


	UINT cWindowHeight;
	UINT cWindowWidth;
	POINT cMousePos;


	int cCurrentCamera;
	int cCurrentLight;

	bool cIsChangeScreen;
	Screens cNextScreen;
	
	
};

