#include "Camera.h"
#include"EventSystem.h"
Camera::Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
	: _eye(position), _at(at), _up(up), _windowWidth(windowWidth), _windowHeight(windowHeight), _nearDepth(nearDepth), _farDepth(farDepth)
{
	SubscribeToEvents();

	_Rot = { 0.0f,0.0f,0.0f };

	Update();
}

Camera::~Camera()
{
	CleanUp();
}

void Camera::Update()
{
	//get quaturnion rotation
	XMFLOAT4 orientation;
	Quaternion qOrientation;
	XMMATRIX RotationMatrix;

	XMMATRIX rotation = XMMatrixRotationX(_Rot.x) * XMMatrixRotationY(_Rot.y) * XMMatrixRotationZ(_Rot.z);
	XMStoreFloat4(&orientation, XMQuaternionRotationMatrix(rotation));
	qOrientation.r = orientation.w;
	qOrientation.i = orientation.x;
	qOrientation.j = orientation.y;
	qOrientation.k = orientation.z;
	
	CalculateTransformMatrixRowMajor(RotationMatrix, { 0,0,0 }, qOrientation);

    // Initialize the view matrix

	XMVECTOR cmaTarget = XMVector3Transform(XMLoadFloat3(&_DefualtFord), RotationMatrix);
	cmaTarget += XMLoadFloat3(&_eye);
	XMVECTOR upDir = XMVector3TransformCoord(XMLoadFloat3(&_DefualtUP), RotationMatrix);
	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMLoadFloat3(&_eye), cmaTarget, upDir));


	XMStoreFloat3(&_VecFord,XMVector3TransformCoord(XMLoadFloat3(&_DefualtFord), RotationMatrix));
	XMStoreFloat3(&_VecBack, XMVector3TransformCoord(XMLoadFloat3(&_DefaultBack), RotationMatrix));
	XMStoreFloat3(&_VecLeft, XMVector3TransformCoord(XMLoadFloat3(&_DefaultLeft), RotationMatrix));
	XMStoreFloat3(&_VecRight, XMVector3TransformCoord(XMLoadFloat3(&_DefaultRight), RotationMatrix));
	
    // Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(0.25f * XM_PI, _windowWidth / _windowHeight, _nearDepth, _farDepth));



	//add events
	EventSystem::Instance()->AddEvent(Events::ViewMatix, &_view);
	EventSystem::Instance()->AddEvent(Events::ProjectionMatrix, &_projection);
	EventSystem::Instance()->AddEvent(Events::EyePos, &_eye);
}

void Camera::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = nearDepth;
	_farDepth = farDepth;
}

void Camera::SetPosition(XMFLOAT3 position)
{
	_eye = position;
}

void Camera::AgustPos(XMFLOAT3 position)
{
	_eye.x += position.x;
	_eye.y += position.y;
	_eye.z += position.z;
}

void Camera::AgustRot(XMFLOAT3 rot)
{
	_Rot.x += rot.x;
	_Rot.y += rot.y;
	_Rot.z += rot.z;


	if (_Rot.x >= XMConvertToRadians(90.0f))
		_Rot.x = XMConvertToRadians(90.0f);

	if (_Rot.x <= XMConvertToRadians(-90.0f))
		_Rot.x = XMConvertToRadians(-90.0f);
}

void Camera::OnNotify(const void* entertity, Events event)
{
	switch (event)
	{
	case Events::WindowSizeUpdate:
		//update window size
		XMFLOAT2 windowSize = *(XMFLOAT2*)entertity;
		_windowWidth = windowSize.x;
		_windowHeight = windowSize.y;
		break;

	case Events::UpInput: 
		//move forward
		this->AgustPos({
						this->GetVecFord().x * _cameraSpeed,
						this->GetVecFord().y * _cameraSpeed ,
						this->GetVecFord().z * _cameraSpeed
					});
		break;
	case Events::DownInput:
		//move back
		this->AgustPos({
						this->GetVecBack().x * _cameraSpeed,
						this->GetVecBack().y * _cameraSpeed ,
						this->GetVecBack().z * _cameraSpeed
					});
		break;
	case Events::LeftInput:
		//move left
	this->AgustPos({
							this->GetVecLeft().x * _cameraSpeed,
							this->GetVecLeft().y * _cameraSpeed ,
							this->GetVecLeft().z * _cameraSpeed
						});
		break;
	case Events::RightInput:
		//move right
		this->AgustPos({
						this->GetVecRight().x * _cameraSpeed,
						this->GetVecRight().y * _cameraSpeed ,
						this->GetVecRight().z * _cameraSpeed
					});
		break;	
	case Events::LeftDown: {
		//rotate to movement of mouse
		this->AgustRot(
							XMFLOAT3(
								_MousePos.y * 0.005f,
								_MousePos.x * 0.005f,
								0.0f
							)
						);
	}
		break;
	case Events::MousePositionRaw:
		//get mouse pos
		_MousePos = *(XMFLOAT2*)entertity;
		break;

	}
}

void Camera::SubscribeToEvents()
{
	EventSystem::Instance()->AddObserver(this, Events::WindowSizeUpdate);
	EventSystem::Instance()->AddObserver(this, Events::UpInput);
	EventSystem::Instance()->AddObserver(this, Events::DownInput);
	EventSystem::Instance()->AddObserver(this, Events::LeftInput);
	EventSystem::Instance()->AddObserver(this, Events::RightInput);
	EventSystem::Instance()->AddObserver(this, Events::MousePositionRaw);
	EventSystem::Instance()->AddObserver(this, Events::LeftDown);
}

void Camera::CleanUp()
{
	EventSystem::Instance()->RemoveObserver(this);
}

XMFLOAT4X4 Camera::GetViewProjection() const 
{ 
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, view * projection);

	return viewProj;
}