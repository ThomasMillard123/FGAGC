#include "Smoke_ParticalSystem.h"
#include"C++HelperFunctions.h"
Smoke_ParticalSystem::Smoke_ParticalSystem(int maxParticals, Vector3 initPos, string name):ParticalSystem(maxParticals, initPos, name)
{
}

Smoke_ParticalSystem::~Smoke_ParticalSystem()
{
}

void Smoke_ParticalSystem::Update(float DeltaTime)
{

	for (auto partical : _Particals) {

		partical->Update(DeltaTime);

		

		if (partical->GetLifeSpan() == MaxlifeSpan) {
			ResetPartical(partical);
		}
		else
		{
				partical->GetParticalModel()->SetThruust({ (float)RandomGen::random<int>(0,20),500,(float)RandomGen::random<int>(0,20) });
				partical->AddToLifeSpan(1);
		}

	}
}
