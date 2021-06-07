#include "GameObjectControllor.h"
#include"EventSystem.h"
#include"ParticleModel.h"
#include"RigidBody.h"
#include"Appearance.h"
#include"Transform.h"

GameObjectControllor::GameObjectControllor()
{
	SubscribedEvents();
}

GameObjectControllor::~GameObjectControllor()
{
	CleanUp();
}
//subscrived events
void GameObjectControllor::SubscribedEvents()
{
	EventSystem::Instance()->AddObserver(this, Events::ConstantBufferSet);
	EventSystem::Instance()->AddObserver(this, Events::keyInput);
	EventSystem::Instance()->AddObserver(this, Events::NumPress);
	EventSystem::Instance()->AddObserver(this, Events::WInput);
	EventSystem::Instance()->AddObserver(this, Events::SInput);
	EventSystem::Instance()->AddObserver(this, Events::AInput);
	EventSystem::Instance()->AddObserver(this, Events::DInput);
	EventSystem::Instance()->AddObserver(this, Events::RightDown);
	EventSystem::Instance()->AddObserver(this, Events::MousePosition);
}

void GameObjectControllor::AddGameObject(GameObject* NewObject)
{
	_pGameObjects.push_back(NewObject);
}

void GameObjectControllor::AddGameObject(string name, Transform* NewTransfrom, Appearance* newApprance, ParticleModel* newPhysics)
{
	_pGameObjects.push_back(new GameObject(name, newApprance, NewTransfrom, newPhysics));
}

void GameObjectControllor::RemoveGameObject(GameObject* removeGameObject)
{
	ListHelpers::RemoveFromVector<GameObject*>(&_pGameObjects, &removeGameObject);
}

void GameObjectControllor::Update(float DeltaTime)
{
	// Update objects
	for (auto gameObject : _pGameObjects)
	{
		gameObject->Update(DeltaTime);

	}

	Test1CollitionOnTiltedPlane(DeltaTime);
	Test2CollitionOnTiltedMoveingObjects(DeltaTime);
}

void GameObjectControllor::Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* pConstantBuffer)
{
	ConstantBuffer cb = *pConstantBuffer;
	for (auto gameObject : _pGameObjects)
	{
		// Get render material
		Material material = gameObject->GetAppearance()->GetMaterial();

		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		// Set world matrix
		cb.World = XMMatrixTranspose(XMLoadFloat4x4(&gameObject->GetTransfrom()->GetWorldMatrix()));

		// Set texture
		if (gameObject->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = gameObject->GetAppearance()->GetTextureRV();
			pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		// Draw object
		gameObject->Draw(pImmediateContext);
	}
}
void GameObjectControllor::CleanUp()
{
	_pGameObjects.clear();
	EventSystem::Instance()->RemoveObserver(this);
}
void GameObjectControllor::Test1CollitionOnTiltedPlane(float deltatTime)
{
	//Plane at an angle 
	int j=8;
			//check Collision with AABB and sphere to show they are working
				if (_pGameObjects[5]->GetParticalModel()->CollisionCheck(_pGameObjects[j]->GetTransfrom()->GetVec3Pos(), _pGameObjects[j]->GetParticalModel()->GetBoundingSphere().Radius)) {
					if (_pGameObjects[5]->GetParticalModel()->AABBCollisonCheck(_pGameObjects[j]->GetTransfrom()->GetVec3Pos(), _pGameObjects[j]->GetParticalModel()->GetAABB())) {

						//get Plane normal
						Vector3 N;
						XMMATRIX transfromation;
						CalculateTransformMatrixRowMajor(transfromation, { 0,0,0 }, _pGameObjects[j]->GetTransfrom()->GetOrination());
						XMFLOAT3 normla;
						XMStoreFloat3(&normla, XMVector3TransformCoord({ 0, 1, 0 }, transfromation));
						N = { normla.x,normla.y,normla.z };

						//caluate vel
						Vector3 negVel = { -_pGameObjects[5]->GetParticalModel()->GetVelocity().x,-_pGameObjects[5]->GetParticalModel()->GetVelocity().y,-_pGameObjects[5]->GetParticalModel()->GetVelocity().z };
						Vector3 p = N*(negVel.dot_product(N));
						Vector3 Rvel = p * 2 + _pGameObjects[5]->GetParticalModel()->GetVelocity();

						Vector3 impualse = Rvel * _pGameObjects[5]->GetParticalModel()->GetMass();
						Vector3 newVel = impualse * (1 / _pGameObjects[5]->GetParticalModel()->GetMass());

						_pGameObjects[5]->GetParticalModel()->SetVelocity(newVel);

					}
				}
			
	
}

void GameObjectControllor::Test2CollitionOnTiltedMoveingObjects(float deltatTime)
{
	//aluminum 
	float e = 0.1;
	//check Collision with AABB and sphere to show they are working
	if (_pGameObjects[6]->GetParticalModel()->CollisionCheck(_pGameObjects[7]->GetTransfrom()->GetVec3Pos(), _pGameObjects[7]->GetParticalModel()->GetBoundingSphere().Radius)) {
		if (_pGameObjects[6]->GetParticalModel()->AABBCollisonCheck(_pGameObjects[7]->GetTransfrom()->GetVec3Pos(), _pGameObjects[7]->GetParticalModel()->GetAABB())) {

			//get velcositys
			Vector3 a = _pGameObjects[6]->GetParticalModel()->GetVelocity() * -e + _pGameObjects[7]->GetParticalModel()->GetVelocity() * e;
			Vector3 b = _pGameObjects[6]->GetParticalModel()->GetVelocity() + _pGameObjects[7]->GetParticalModel()->GetVelocity();

			Vector3 v2 = (a - b)/2;
			Vector3 v1 = (a + b)/2;

		
			//caluate vel for game object 6
			Vector3 N = { 0,0,1 };
			Vector3 negVel;
			 negVel = { -v1.x,-v1.y,-v1.z };
			Vector3 p = N * (v1.dot_product(N));
			Vector3 Rvel = p * 2 + _pGameObjects[6]->GetParticalModel()->GetVelocity();

			Vector3 impualse = Rvel * _pGameObjects[6]->GetParticalModel()->GetMass();
			Vector3 newVel = impualse * (1 / _pGameObjects[6]->GetParticalModel()->GetMass());

			_pGameObjects[6]->GetParticalModel()->SetVelocity(newVel);

			//caluate vel for Game object 7
			 N = { 0,0,-1 };
			 negVel = { -v2.x,-v2.y,-v2.z };
			 p = N * (v2.dot_product(N));
			 Rvel = p * 2 + _pGameObjects[7]->GetParticalModel()->GetVelocity();

			 impualse = Rvel * _pGameObjects[7]->GetParticalModel()->GetMass();
			 newVel = impualse * (1 / _pGameObjects[7]->GetParticalModel()->GetMass());

			_pGameObjects[7]->GetParticalModel()->SetVelocity(newVel);

		}
	}
	
}

void GameObjectControllor::OnNotify(const void* entertity, Events event)
{
	switch (event)
	{
	case Events::ConstantBufferSet:
		_pConstantBuffer = (ID3D11Buffer*)entertity;
		break;
	case Events::keyInput: {

		unsigned char key = *(unsigned char*)entertity;
		//stop object
		if (key == 'Q') {
			_pGameObjects[0]->GetParticalModel()->SetThruust({ 0,0,0 });
		}
	}
		break;
	
	case Events::WInput:
		//thurst up
		_pGameObjects[0]->GetParticalModel()->AgustThrust({ 0.0f,10.0f,0.0f }
			
		);

		break;
	case Events::SInput:
		//thurst down
		_pGameObjects[0]->GetParticalModel()->AgustThrust({ 0.0f,-10.0f,0.0f });
		break;
	case Events::AInput:
		//thurst left
		_pGameObjects[0]->GetParticalModel()->AgustThrust({ -10.0f,00.0f,0.0f });
		break;
	case Events::DInput:
		//thurst right
		_pGameObjects[0]->GetParticalModel()->AgustThrust({ 10.0f,0.0f,0.0f});
		break;
	case Events::NumPress: {
		unsigned char key = *(unsigned char*)entertity;
		//state colision test
		switch (key)
		{
		case '0':
		case VK_NUMPAD0:
			break;
		case '1':
		case VK_NUMPAD1:
			//test 2: cube on titled plane
			_pGameObjects[5]->GetParticalModel()->SetUseAcceleration(true);

			break;
		case '2':
		case VK_NUMPAD2:
			//test 3: cube moveing into eachother

			_pGameObjects[6]->GetParticalModel()->SetThruust({ 0,0,50 });
			_pGameObjects[7]->GetParticalModel()->SetThruust({ 0,0,-60 });
			break;
		case '3':
		case VK_NUMPAD3:
			break;
		case '4':
		case VK_NUMPAD4:
			break;
		case '5':
		case VK_NUMPAD5:
			break;
		case '6':
		case VK_NUMPAD6:
			break;
		case '7':
		case VK_NUMPAD7:
			break;
		case '8':
		case VK_NUMPAD8:
			break;
		case '9':
		case VK_NUMPAD9:
			break;
		default:
			break;
		}
	}
		break;
	case Events::RightDown: {
		//when clicked rotate in direct direction
		BoundingSphere bond = _pGameObjects[1]->GetParticalModel()->GetBoundingSphere();
		bond.Center = _pGameObjects[1]->GetTransfrom()->GetPosition();
		//force right
		if (_MousePicking.IsPicked({ _MousePos.x,_MousePos.y }, bond)) {
			GameObject* ob = _pGameObjects[1];
			RigidBody* hom = static_cast<RigidBody*> (ob->GetParticalModel());
			hom->SetRotForce({ -1,0,0 });
			hom->SetPointHit({ -1,0,-1 });
		}
		bond = _pGameObjects[2]->GetParticalModel()->GetBoundingSphere();
		bond.Center = _pGameObjects[2]->GetTransfrom()->GetPosition();
		//force left
		if (_MousePicking.IsPicked({ _MousePos.x,_MousePos.y }, bond)) {
			GameObject* ob = _pGameObjects[2];
			RigidBody* hom = static_cast<RigidBody*> (ob->GetParticalModel());
			hom->SetRotForce({ 1,0,0 });
			hom->SetPointHit({ 1,0,-1 });
		}
		bond = _pGameObjects[3]->GetParticalModel()->GetBoundingSphere();
		bond.Center = _pGameObjects[3]->GetTransfrom()->GetPosition();
		//force top
		 if (_MousePicking.IsPicked({ _MousePos.x,_MousePos.y }, bond)) {
			GameObject* ob = _pGameObjects[3];
			RigidBody* hom = static_cast<RigidBody*> (ob->GetParticalModel());
			hom->SetRotForce({ 0,1,0 });
			hom->SetPointHit({ 0,1,-1 });
		}
		 bond = _pGameObjects[4]->GetParticalModel()->GetBoundingSphere();
		 bond.Center = _pGameObjects[4]->GetTransfrom()->GetPosition();
		//force bot
		 if (_MousePicking.IsPicked({ _MousePos.x,_MousePos.y }, bond)) {
			GameObject* ob = _pGameObjects[4];
			RigidBody* hom = static_cast<RigidBody*> (ob->GetParticalModel());
			hom->SetRotForce({ 0,-1,0 });
			hom->SetPointHit({ 0,-1,-1 });
		}
	}
		break;
	case Events::MousePosition:
		_MousePos = *(XMFLOAT2*)entertity;
		break;
	}
}
