#pragma once
#include "ParticalSystem.h"
class Smoke_ParticalSystem :
    public ParticalSystem
{
public:
    Smoke_ParticalSystem(int maxParticals, Vector3 initPos, string name);
    ~Smoke_ParticalSystem();
    void Update(float DeltaTime);
};

