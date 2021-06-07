#include "Rain_ParticalSystem.h"

Rain_ParticalSystem::Rain_ParticalSystem(int maxParticals, Vector3 initPos, string name):ParticalSystem(maxParticals, initPos, name)
{
}

Rain_ParticalSystem::~Rain_ParticalSystem()
{
}

void Rain_ParticalSystem::Update(float DeltaTime)
{
	for (auto partical : _Particals) {

		partical->Update(DeltaTime);

		if (partical->GetTransfrom()->GetVec3Pos().y <= 0.5f) {

			if (partical->GetLifeSpan() == MaxlifeSpan) {
				ResetPartical(partical);
			}
			else
			{
				partical->GetTransfrom()->SetScale({ 0.4,0.1,0.4 });
				partical->AddToLifeSpan(1);
			}
		}

	}
}
