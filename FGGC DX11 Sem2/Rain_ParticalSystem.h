#pragma once
#include "ParticalSystem.h"
class Rain_ParticalSystem :
    public ParticalSystem
{
public:
    Rain_ParticalSystem(int maxParticals, Vector3 initPos, string name);
    ~Rain_ParticalSystem();
    void Update(float DeltaTime);

private:


};

