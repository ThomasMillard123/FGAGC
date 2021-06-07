#pragma once
#include "ParticleModel.h"
#include "Quaternion.h"
#include"Vector3.h"
/// <summary>
/// Class for riged body rotation physics
/// </summary>
class RigidBody :
    public ParticleModel
{

public:
    RigidBody(Transform* transfrome, bool useAcceleration, Vector3 initialVelocity, Vector3 initialAccelration);
    ~RigidBody();
    void Update(float time);
    
    Vector3 CaculateToutque(Vector3 Force, Vector3 pointApplied);
    void CalulateAngularAcceleration();
    void CalulateAngularVelocity(float time);
    void AddDrag(float tim);
    void CalulateCurrentOrientation(float time);

    Vector3 GetTorque() const { return _Torque; }

    Vector3 GetRotForce() const { return _RotForce; }

    void SetRotForce(Vector3 rotForce) { _RotForce = rotForce; }
    void SetPointHit(Vector3 point) { _PointHit = point; }
private:
//forces
    Vector3 _RotForce;
    Vector3 _PointHit;
    Vector3 _Torque;
   
    Vector3 _RotationDirection;
    Vector3 _AngularAcc;
    Vector3 _AngularVel;

    XMFLOAT3X3 _InertiaTensor;

    float _AngularDamping = 0.5;
};

