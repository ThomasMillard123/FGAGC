#pragma once

#include <d3d11_1.h>
#include <directxmath.h>
#include<vector>
#include<DirectXCollision.h>

#include"Transform.h"
#include"Vector3.h"

using namespace DirectX;
using namespace std;

/// <summary>
/// Class for partical physics
/// </summary>
class ParticleModel
{
public:
	ParticleModel(Transform* transfrome,bool useAcceleration, Vector3 initialVelocity, Vector3 initialAccelration,float mass);
	~ParticleModel();

	virtual void Update(float t);
	
	void UpdateAccelraton();

	//movemnt
	void MoveConstVelocity(float deltaTime);
	void MoveConstAcceleration(float deltaTime);
	//Velocity
	void SetVelocity(Vector3 newVelocity) { _Velocity = newVelocity; }
	Vector3 GetVelocity() const { return _Velocity; }

	//Accelration
	void SetAcceleration(Vector3 newAccelration) { _Acceleration = newAccelration; }
	Vector3 GetAccleration() const { return _Acceleration; }

	//net force
	void SetNetForce(Vector3 newNetForce) { _NetForce = newNetForce; }
	Vector3 GetNetForce()const { return _NetForce; }

	//mass
	void SetMass(float newMass) { _Mass = newMass; }
	float GetMass() const { return _Mass; }

	//thrust
	void SetThruust(Vector3 newThrust) { _Thrust = newThrust; }
	void AgustThrust(Vector3 thrust) { _Thrust += thrust; }
	Vector3 GetThrust()const { return _Thrust; }

	//set drag type
	void SetUseLamnier(bool use) { useLamnier = use; }

	//movemnt type
	void SetUseAcceleration(bool useAcceleration) { _UseAcceleration = useAcceleration; }


	BoundingSphere GetBoundingSphere() { return _BoundingSphere; }
	BoundingBox GetAABB() { return _AABB; }
	bool CollisionCheck(Vector3 position, float radius);
	bool AABBCollisonCheck(Vector3 position, BoundingBox Box);



	Vector3 GetUp() { return Vector3{ -_Weight.x,-_Weight.y,-_Weight.z }; }

protected:
	
	void AddForce(Vector3 Force);
	//drag claulations
	void CaulateDrag();
	void DragLamFlow();
	void DragTurbFlow();
	//friction caluations
	void CalulateFriction(float deltaTime);
	
	void CleanUp();
protected:

	Transform* transform;
	

	Vector3 _Velocity;
	Vector3 _Acceleration;

	//Forces
	Vector3 _NetForce;
	Vector3 _Weight;
	Vector3 _Drag;
	Vector3 _Friction;
	Vector3 _Thrust;

	bool useLamnier;
	bool _UseAcceleration;

	//Bounding items
	BoundingSphere _BoundingSphere;
	BoundingBox _AABB;

	//item data
	float gravity = 9.8f;
	float _Mass;

	//DragFacot of cube
	float _DragFactor=0.8f;
	//Aluminum	and Aluminum
	float _FrinctionFacor = 0.3f;
	float _ItemArea=1;
	float _DensityOfFluid=0.5f;
};

