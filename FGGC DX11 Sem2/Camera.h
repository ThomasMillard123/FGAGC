#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "Quaternion.h"
#include"Observer.h"
using namespace DirectX;
/// <summary>
/// Camera Class: 
/// updates the camera
/// moves and rotates the camera
/// </summary>
class Camera:public Observer
{
public:
	Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	void Update();

	XMFLOAT4X4 GetView() const { return _view; }
	XMFLOAT4X4 GetProjection() const { return _projection; }
	XMFLOAT4X4 GetViewProjection() const;

	XMFLOAT3 GetPosition() const { return _eye; }
	XMFLOAT3 GetLookAt() const { return _at; }
	XMFLOAT3 GetUp() const { return _up; }


	XMFLOAT3 GetVecFord() { return _VecFord; }
	XMFLOAT3 GetVecBack() { return _VecBack; }
	XMFLOAT3 GetVecLeft() { return _VecLeft; }
	XMFLOAT3 GetVecRight() { return _VecRight; }
	XMFLOAT3 GetVecUP() { return _VecUP; }

	void SetPosition(XMFLOAT3 position);
	void AgustPos(XMFLOAT3 position);

	void SetLookAt(XMFLOAT3 lookAt) { _at = lookAt; }
	void SetUp(XMFLOAT3 up) { _up = up; }

	void AgustRot(XMFLOAT3 rot);
	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	void OnNotify(const void* entertity, Events event);


private:
	void SubscribeToEvents();
	void CleanUp();
private:

	//movement data
	const XMFLOAT3 _DefualtUP = { 0.0f,1.0f,0.0f };
	const XMFLOAT3 _DefualtFord = { 0.0f,0.0f,1.0f };
	const XMFLOAT3 _DefaultBack = { 0.0f,0.0f,-1.0f };
	const XMFLOAT3 _DefaultLeft = { -1.0f,0.0f,0.0f };
	const XMFLOAT3 _DefaultRight = { 1.0f,0.0f,0.0f };

	 XMFLOAT3 _VecUP;
	 XMFLOAT3 _VecFord ;
	 XMFLOAT3 _VecBack ;
	 XMFLOAT3 _VecLeft ;
	 XMFLOAT3 _VecRight;


	//cam data
	XMFLOAT3 _Rot;
	float _cameraSpeed = 2.0f;


	XMFLOAT3 _eye; 
	XMFLOAT3 _at;
	XMFLOAT3 _up;

	FLOAT _windowWidth;
	FLOAT _windowHeight;
	FLOAT _nearDepth;
	FLOAT _farDepth;

	//matrixes 
	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

	//mouse data
	XMFLOAT2 _MousePos;

};

