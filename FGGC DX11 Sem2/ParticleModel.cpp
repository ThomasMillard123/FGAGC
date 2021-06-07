#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transfrome, bool useAcceleration, Vector3 initialVelocity, Vector3 initialAccelration,float mass): transform(transfrome),_UseAcceleration(useAcceleration), _Velocity(initialVelocity),_Acceleration(initialAccelration),_Mass(mass)
{
	
	//drag type
	useLamnier = false;

	//colloitions boxes
	_BoundingSphere.Radius = 0.5f;
	_BoundingSphere.Center = transform->GetPosition();
	_AABB.CreateFromSphere(_AABB, _BoundingSphere);

	//Weight calulation
	_Weight.x = 0.0f;
	_Weight.y = -(_Mass * gravity);
	_Weight.z = 0.0f;
	AddForce(_Weight);
	
	_Thrust = { 0.0f,0.0f,0.0f };
	_Drag={ 0.0f, 0.0f, 0.0f };
	
}

ParticleModel::~ParticleModel()
{
}

void ParticleModel::Update(float t)
{	
	//add forces
	CaulateDrag();
	CalulateFriction(t);
	AddForce(_Thrust);
	AddForce(_Weight);
	
	
	if (_UseAcceleration) {
		UpdateAccelraton();
		MoveConstAcceleration(t);
	}
	else
	{
		MoveConstVelocity(t);
	}
	
	Vector3 Pos = transform->GetVec3Pos();

	//stop item falling below 0.5
	if (Pos.y < 0.5f)
	{
		_Velocity.y = 0.0f;
		Pos.y = 0.5f;
		transform->SetPosition({ Pos.x,Pos.y,Pos.z });
	}


	//reset net force for next frame
	_NetForce = { 0.0f, 0.0f, 0.0f };
}
//get accleration from mass and forces
void ParticleModel::UpdateAccelraton()
{
	_Acceleration.x = _NetForce.x / _Mass;
	_Acceleration.y = _NetForce.y / _Mass;
	_Acceleration.z = _NetForce.z / _Mass;

}
//constat velocity
void ParticleModel::MoveConstVelocity(float deltaTime)
{
	Vector3 NewPos = transform->GetVec3Pos();


	NewPos.x = NewPos.x+ _Velocity.x * deltaTime;
	NewPos.y = NewPos.y+ _Velocity.y * deltaTime;
	NewPos.z = NewPos.z+ _Velocity.z * deltaTime;

	

	XMFLOAT3 Pos = { NewPos.x,NewPos.y,NewPos.z };
	transform->SetPosition(Pos);
}
//constat accleration
void ParticleModel::MoveConstAcceleration(float deltaTime)
{
	
	Vector3 NewPos = transform->GetVec3Pos();
	NewPos.x = NewPos.x +_Velocity.x * deltaTime+ 0.5f*_Acceleration.x*deltaTime*deltaTime;
	NewPos.y = NewPos.y+ _Velocity.y * deltaTime + 0.5f * _Acceleration.y * deltaTime * deltaTime;
	NewPos.z = NewPos.z +_Velocity.z * deltaTime + 0.5f * _Acceleration.z * deltaTime * deltaTime;
	

	_Velocity.x += _Acceleration.x * deltaTime;
	_Velocity.y += _Acceleration.y * deltaTime;
	_Velocity.z += _Acceleration.z * deltaTime;
	
	XMFLOAT3 position= { NewPos.x,NewPos.y,NewPos.z };
	transform->SetPosition(position);
}

//decide which drag to use
void ParticleModel::CaulateDrag()
{
	if (useLamnier) {
		DragLamFlow();
	}
	else
	{
		DragTurbFlow();
	}

}

void ParticleModel::CleanUp()
{
	transform = nullptr;
}

void ParticleModel::DragLamFlow()
{
	_Drag.x = -(_DragFactor * _Velocity.x);
	_Drag.y = -(_DragFactor * _Velocity.y);
	_Drag.z = -(_DragFactor * _Velocity.z);
	AddForce(_Drag);
	
}
void ParticleModel::DragTurbFlow()
{
	float VelMag= _Velocity.magnitude();
	float dragMag =0.5f* _DensityOfFluid * _DragFactor * _ItemArea *VelMag * VelMag;

	Vector3 unitVelocity = _Velocity.normalization();
	
	_Drag.x = -dragMag * unitVelocity.x;
	_Drag.y = -dragMag  * unitVelocity.y;
	_Drag.z = -dragMag  * unitVelocity.z;
	AddForce(_Drag);
}

void ParticleModel::CalulateFriction(float deltaTime)
{

	Vector3 unitVelocity = _Velocity.normalization();
	float frictionMag = _FrinctionFacor * _Mass * 9.8f;
	_Friction.x = -frictionMag * unitVelocity.x;
	_Friction.y = -frictionMag  * unitVelocity.y;
	_Friction.z = -frictionMag  * unitVelocity.z;

	
	AddForce(_Friction);
}	

bool ParticleModel::CollisionCheck(Vector3 position, float radius)
{
	BoundingSphere collison2;
	collison2.Radius = radius;
	_BoundingSphere.Center = transform->GetPosition();
	collison2.Center = { position.x,position.y,position.z };
	if (_BoundingSphere.Intersects(collison2)) {
		return true;
	}
	return false;


}

bool ParticleModel::AABBCollisonCheck(Vector3 position, BoundingBox Box)
{
	BoundingBox collison2 = Box;
	collison2.Center = XMFLOAT3{ position.x,position.y,position.z };
	_AABB.Center = transform->GetPosition();
	
	if (_AABB.Intersects(collison2)) {
		return true;
	}
	return false;
	
}

//add to netForce
void ParticleModel::AddForce(Vector3 Force)
{
	_NetForce += Force;
}

