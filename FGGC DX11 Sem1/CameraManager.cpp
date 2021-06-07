#include "CameraManager.h"

CameraManager::CameraManager(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 at, DirectX::XMFLOAT3 up, CameraMode cameraMode, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	cEye = eye;
	cAt = at;
	cUp = up;
	cFirstEye = eye;
	cFirstAt = at;
	cFirstUp = up;
	cCameraMode = cameraMode;
	UpdateViewMatix();

	cWindowHeight = windowHeight;
	cWindowWidth = windowWidth;
	cNearDepth = nearDepth;
	cFarDepth = farDepth;
	UpdateProjectionMatrix();
}
void CameraManager::Update(float t)
{	
	float rotationY = 0, rotationX = 0;
	float movementX = 0, movementY = 0, movementZ = 0;

	DirectX::XMMATRIX translationMatrix;
	DirectX::XMVECTOR rotationYVector, rotationXVector;
	switch (cCameraMode)
	{
	case Static:	

		

		break;
	case LookAtMovemntCam:
		//find vectro between at and eye to get direction and length
		DirectX::XMVECTOR EyeAtVector = DirectX::XMVECTOR{ cAt.x - cEye.x,cAt.y - cEye.y,cAt.z - cEye.z };
		DirectX::XMVECTOR Direction = DirectX::XMVector3Normalize(EyeAtVector);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(EyeAtVector);

		//camrea movemt decided  by direction of camera
		if (GetAsyncKeyState(VK_UP))
		{
			if (DirectX::XMVectorGetX(Length) > 4) {
				if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ 0.0f,0.0f,1.0f })) {
					movementZ += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ 1.0f,0.0f,0.0f })) {
					movementX += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ 0.0f,0.0f,-1.0f })) {
					movementZ -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ -1.0f,0.0f,0.0f })) {
					movementX -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 1.0f && DirectX::XMVectorGetZ(Direction) < 1.0f && DirectX::XMVectorGetX(Direction) > 0.0f && DirectX::XMVectorGetZ(Direction) > 0.0f) {
					movementZ += cCameraMoveSpeed * t;
					movementX += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 0.0f && DirectX::XMVectorGetZ(Direction) < 1.0f && DirectX::XMVectorGetX(Direction) > -1.0f && DirectX::XMVectorGetZ(Direction) > 0.0f) {
					movementZ += cCameraMoveSpeed * t;
					movementX -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 1.0f && DirectX::XMVectorGetZ(Direction) < 0.0f && DirectX::XMVectorGetX(Direction) > 0.0f && DirectX::XMVectorGetZ(Direction) > -1.0f) {
					movementZ -= cCameraMoveSpeed * t;
					movementX += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 0.0f && DirectX::XMVectorGetZ(Direction) < 0.0f && DirectX::XMVectorGetX(Direction) > -1.0f && DirectX::XMVectorGetZ(Direction) > -1.0f) {
					movementZ -= cCameraMoveSpeed * t;
					movementX -= cCameraMoveSpeed * t;
				}
			}
			else {

			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (DirectX::XMVectorGetX(Length) < 10) {
				if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ 0.0f,0.0f,1.0f })) {
					movementZ -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ 1.0f,0.0f,0.0f })) {
					movementX -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ 0.0f,0.0f,-1.0f })) {
					movementZ += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVector3Equal(Direction, DirectX::XMVECTOR{ -1.0f,0.0f,0.0f })) {
					movementX += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 1.0f && DirectX::XMVectorGetZ(Direction) < 1.0f && DirectX::XMVectorGetX(Direction) > 0.0f && DirectX::XMVectorGetZ(Direction) > 0.0f) {
					movementZ -= cCameraMoveSpeed * t;
					movementX -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 0.0f && DirectX::XMVectorGetZ(Direction) < 1.0f && DirectX::XMVectorGetX(Direction) > -1.0f && DirectX::XMVectorGetZ(Direction) > 0.0f) {
					movementZ -= cCameraMoveSpeed * t;
					movementX += cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 1.0f && DirectX::XMVectorGetZ(Direction) < 0.0f && DirectX::XMVectorGetX(Direction) > 0.0f && DirectX::XMVectorGetZ(Direction) > -1.0f) {
					movementZ += cCameraMoveSpeed * t;
					movementX -= cCameraMoveSpeed * t;
				}
				else if (DirectX::XMVectorGetX(Direction) < 0.0f && DirectX::XMVectorGetZ(Direction) < 0.0f && DirectX::XMVectorGetX(Direction) > -1.0f && DirectX::XMVectorGetZ(Direction) > -1.0f) {
					movementZ += cCameraMoveSpeed * t;
					movementX += cCameraMoveSpeed * t;
				}
			}
		}
		if (GetAsyncKeyState(VK_LEFT))
		{

			rotationY += cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{

			rotationY -= cCameraMoveSpeed * t;

		}

		else if (GetAsyncKeyState(VK_INSERT))
		{
			movementX -= cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_DELETE))
		{

			movementX += cCameraMoveSpeed * t;

		}
		//rotat camera
		rotationYVector = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&cUp), rotationY);
		DirectX::XMStoreFloat3(&cEye, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&cEye), rotationYVector));

		// move camra 
		translationMatrix = DirectX::XMMatrixTranslation(movementX, movementY, movementZ);
		DirectX::XMStoreFloat3(&cEye, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&cEye), translationMatrix));

		break;
	case FirstPerson:
		
		//first person camra movement

		if (GetAsyncKeyState(VK_LEFT))
		{
			rotationY -= cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{

			rotationY += cCameraMoveSpeed * t;

		}

		rotationYVector = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&cUp), rotationY);
		DirectX::XMStoreFloat3(&cAt, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&cAt), rotationYVector));
		DirectX::XMStoreFloat3(&cFirstEye, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&cFirstEye), rotationYVector));
		
		break;
	case ThirdPerson:	
		//third person camra movement

		

		 if (GetAsyncKeyState(VK_LEFT))
		{
			 rotationY -= cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{

			 rotationY += cCameraMoveSpeed * t;

		}

		 rotationYVector = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&cUp), rotationY);
		 DirectX::XMStoreFloat3(&cFirstEye, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&cFirstEye), rotationYVector));
	
		break;
	case Free:
		//free camrea movement
		if (GetAsyncKeyState(VK_UP))
		{

			movementZ += cCameraMoveSpeed * t;

		}
		else if (GetAsyncKeyState(VK_DOWN))
		{

			movementZ -= cCameraMoveSpeed * t;

		}
		else if (GetAsyncKeyState(VK_LEFT))
		{

			movementX += cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{

			movementX -= cCameraMoveSpeed * t;

		}
		else if(GetAsyncKeyState(VK_NUMPAD9))
		{
			movementY += cCameraMoveSpeed * t;
		}
		else if (GetAsyncKeyState(VK_NUMPAD8))
		{
			movementY -= cCameraMoveSpeed * t;
		}


		if (GetAsyncKeyState(VK_HOME))
		{
			rotationY -= cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_END))
		{

			rotationY += cCameraMoveSpeed * t;

		}
		else if (GetAsyncKeyState(VK_INSERT))
		{
			rotationX -= cCameraMoveSpeed * t;


		}
		else if (GetAsyncKeyState(VK_DELETE))
		{

			rotationX += cCameraMoveSpeed * t;

		}
		//rotate camera
		rotationXVector = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&cUp), rotationX);
		DirectX::XMStoreFloat3(&cAt, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&cAt), rotationXVector));
		rotationYVector = DirectX::XMQuaternionRotationAxis(DirectX::XMVECTOR{ 1.0f,0.0f,0.0f }, rotationY);
		DirectX::XMStoreFloat3(&cAt, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&cAt), rotationYVector));
		//move camera
		translationMatrix = DirectX::XMMatrixTranslation(movementX, movementY, movementZ);
		DirectX::XMStoreFloat3(&cEye, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&cEye), translationMatrix));
		
		

		break;
	default:

		break;
	}

	//update the camera maxrixs
	UpdateViewMatix();
	UpdateProjectionMatrix();
}
void CameraManager::ChangeCamera(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 at, DirectX::XMFLOAT3 up)
{
	cEye = eye;
	cAt = at;
	cUp = up;
}
void CameraManager::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	cWindowWidth = windowWidth;
	cWindowHeight = windowHeight;
	cNearDepth = nearDepth;
	cFarDepth = farDepth;

}
void CameraManager::SetEye(DirectX::XMFLOAT3 eyePos)
{
	cEye = eyePos;
}
void CameraManager::SetAt(DirectX::XMFLOAT3 atDirection)
{
	cAt = atDirection;
}
void CameraManager::SetUp(DirectX::XMFLOAT3 upDirection)
{
	cUp = upDirection;
}
void CameraManager::UpdateViewMatix()
{
	DirectX::XMVECTOR Eye =DirectX::XMLoadFloat3(&cEye);
	DirectX::XMVECTOR At = DirectX::XMLoadFloat3(&cAt);
	DirectX::XMVectorAdd(Eye,  DirectX::XMVector4Normalize(At));
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&cUp);

	//declide to look at or look to
	switch (cCameraMode)
	{
	case Static:
	case LookAtMovemntCam:
	case ThirdPerson:
		DirectX::XMStoreFloat4x4(&cView, DirectX::XMMatrixLookAtLH(Eye, At, Up));
		break;
	case FirstPerson:
		
	case Free:
		DirectX::XMStoreFloat4x4(&cView, DirectX::XMMatrixLookToLH(Eye, At, Up));
		break;
	default:
		break;
	}
	
	

	
}

void CameraManager::UpdateProjectionMatrix()
{
	// Initialize the projection matrix
	DirectX::XMStoreFloat4x4(&cProjection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, (FLOAT)cWindowWidth / (FLOAT)cWindowHeight, cNearDepth,cFarDepth ));
}

void CameraManager::SetParent(DirectX::XMFLOAT3 parent)
{
	//set up the camera data to with the pareant 
	cParaentObjectPosition = parent;
	switch (cCameraMode)
	{
	
	case ThirdPerson:
		//rais it to position
		DirectX::XMFLOAT3 moveTo = cFirstEye;
		DirectX::XMFLOAT3 parentEyePosThirdPerson;
		parentEyePosThirdPerson.x = cParaentObjectPosition.x + moveTo.x;
		parentEyePosThirdPerson.y = cParaentObjectPosition.y + moveTo.y;
		parentEyePosThirdPerson.z = cParaentObjectPosition.z + moveTo.z;
		cEye = parentEyePosThirdPerson;
		//set it to look at object
		cAt = cParaentObjectPosition;
		break;
	case FirstPerson:
		
		
		//rais camra so it can see out and is with the object
		DirectX::XMFLOAT3 raisToPosition = cFirstEye;
		DirectX::XMFLOAT3 parentEyePosFirstPerson;
		parentEyePosFirstPerson.x = cParaentObjectPosition.x + raisToPosition.x;
		parentEyePosFirstPerson.y = cParaentObjectPosition.y + raisToPosition.y;
		parentEyePosFirstPerson.z = cParaentObjectPosition.z + raisToPosition.z;

		cEye = parentEyePosFirstPerson;
		
		break;
	default:
		break;
	}

}

DirectX::XMFLOAT4X4 CameraManager::GetViewMatrix()
{
	return cView;
}

DirectX::XMFLOAT4X4 CameraManager::GetProjectionMatrix()
{
	return cProjection;
}

DirectX::XMFLOAT3 CameraManager::GetEye()
{
	return cEye;
}

DirectX::XMFLOAT3 CameraManager::GetAt()
{
	return cAt;
}

DirectX::XMFLOAT3 CameraManager::GetUp()
{
	return cUp;
}

void CameraManager::CleanUp()
{

}

CameraManager::~CameraManager()
{
	CleanUp();
}


