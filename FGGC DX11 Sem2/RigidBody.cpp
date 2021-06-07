#include "RigidBody.h"

RigidBody::RigidBody(Transform* transfrome, bool useAcceleration, Vector3 initialVelocity, Vector3 initialAccelration):ParticleModel(transfrome, useAcceleration, initialVelocity, initialAccelration,10)
{
	//set _InertiaTensor
	_InertiaTensor._11 = (_Mass*((1 * 1) + (1 * 1)))/12;
	_InertiaTensor._12 = 0;
	_InertiaTensor._13 = 0;

	_InertiaTensor._21 = 0;
	_InertiaTensor._22 = (_Mass * ((1 * 1) + (1 * 1))) / 12;
	_InertiaTensor._23 = 0;


	_InertiaTensor._31 = 0;
	_InertiaTensor._32 = 0;
	_InertiaTensor._33 = (_Mass * ((1 * 1) + (1 * 1))) / 12;


	_AngularVel = { 0,0,0 };
	_RotForce = { 0,0,0 };
	_PointHit= { 0,0,0 };
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update(float time)
{
	ParticleModel::Update(time);
	
	CaculateToutque(_RotForce, _PointHit);
	CalulateAngularAcceleration();
	CalulateAngularVelocity(time);
	AddDrag(time);
	CalulateCurrentOrientation(time);
}
//get routque
Vector3 RigidBody::CaculateToutque(Vector3 Force, Vector3 pointApplied)
{
	_Torque = pointApplied.cross_product(Force);
	return _Torque;
}
//get angluar accaleration
void RigidBody::CalulateAngularAcceleration()
{
	XMVECTOR TorqueVec = { _Torque.x, _Torque.y, _Torque.z };
	XMVECTOR angularAccelration=XMVector3Transform(TorqueVec, XMMatrixInverse(nullptr, XMLoadFloat3x3(&_InertiaTensor)));
	_AngularAcc = { XMVectorGetX(angularAccelration),XMVectorGetY(angularAccelration),XMVectorGetZ(angularAccelration) };

}
//get vel
void RigidBody::CalulateAngularVelocity(float time)
{
	_AngularVel += _AngularAcc * time;
}
//get drag force
void RigidBody::AddDrag(float time)
{
	_AngularVel *= pow(_AngularDamping,time);
}
//put rotating ation on object
void RigidBody::CalulateCurrentOrientation(float time)
{

	Quaternion orination= transform->GetOrination();
	orination.addScaledVector( _AngularVel, time);
	orination.normalise();
	transform->Setorination(orination);
	
}
