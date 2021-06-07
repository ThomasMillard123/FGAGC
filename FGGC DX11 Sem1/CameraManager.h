#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "resource.h"

enum CameraMode
{
	Static = 0,
	LookAtMovemntCam,
	FirstPerson,//first person view
	ThirdPerson,//thirrd person view
	Free//free movement

};


class CameraManager
{
public:
	CameraManager(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 at, DirectX::XMFLOAT3 up,CameraMode cameraMode, FLOAT
		windowWidth, FLOAT windowHeight, FLOAT
		nearDepth, FLOAT farDepth
	);
	~CameraManager();

	void CleanUp();

	void Update(float t);

	

	//set camera vlaues
	void ChangeCamera(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 at, DirectX::XMFLOAT3 up);
	
	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	
	void SetEye(DirectX::XMFLOAT3 eyePos);
	void SetAt(DirectX::XMFLOAT3 atDirection);
	void SetUp(DirectX::XMFLOAT3 upDirection);

	//set parate for thired and firstperson camrea
	void SetParent(DirectX::XMFLOAT3 parent);
	//get camera data
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	
	DirectX::XMFLOAT3 GetEye();
	DirectX::XMFLOAT3 GetAt();
	DirectX::XMFLOAT3 GetUp();



private:
	const float cCameraMoveSpeed=0.1f;
	//camera varables
	DirectX::XMFLOAT3 cEye;
	DirectX::XMFLOAT3 cAt;
	DirectX::XMFLOAT3 cUp;


	DirectX::XMFLOAT3 cFirstEye;
	DirectX::XMFLOAT3 cFirstAt;
	DirectX::XMFLOAT3 cFirstUp;

	DirectX::XMFLOAT3 cParaentObjectPosition = { 0.0f,0.0f,0.0f };
	//view vloume
	FLOAT cWindowWidth;
	FLOAT cWindowHeight;
	FLOAT cNearDepth;
	FLOAT cFarDepth;

	
	DirectX::XMFLOAT4X4 cView;
	DirectX::XMFLOAT4X4 cProjection;

	// camera modes
	CameraMode cCameraMode;

	//update camera data
	void UpdateViewMatix();
	void UpdateProjectionMatrix();
};

