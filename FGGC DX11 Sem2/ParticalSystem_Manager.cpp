#include "ParticalSystem_Manager.h"
#include"EventSystem.h"
ParticalSystem_Manager::ParticalSystem_Manager()
{
	EventSystem::Instance()->AddObserver(this, Events::ConstantBufferSet);
}

ParticalSystem_Manager::~ParticalSystem_Manager()
{
}

void ParticalSystem_Manager::AddParticalSystem(ParticalSystem* newSystem)
{
	_ParticalSystems.push_back(newSystem);
}

void ParticalSystem_Manager::RemoveParticalSystem(ParticalSystem* newSystem)
{

	vector<ParticalSystem*>::iterator iter = _ParticalSystems.begin();
	while (iter != _ParticalSystems.end()) {
		if (*iter == newSystem) {
			iter = _ParticalSystems.erase(iter);
		}
		else
		{
			++iter;
		}
	}

}

void ParticalSystem_Manager::ClearList()
{
	_ParticalSystems.clear();
}

void ParticalSystem_Manager::Update(float DeltaTime)
{
	for (UINT i = 0; i < _ParticalSystems.size(); i++)
	{
		_ParticalSystems[i]->Update(DeltaTime);
	}

}

void ParticalSystem_Manager::Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* pConstantBuffer)
{
	for (UINT i = 0; i < _ParticalSystems.size(); i++)
	{
		_ParticalSystems[i]->Draw(pImmediateContext, pConstantBuffer);
	}
}

void ParticalSystem_Manager::OnNotify(const void* entertity, Events event)
{
	switch (event)
	{
		case Events::ConstantBufferSet:
			for (UINT i = 0; i < _ParticalSystems.size(); i++)
			{
				_ParticalSystems[i]->SetConstantBuffer((ID3D11Buffer*)entertity);
			}
			break;
	}
}
