#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>


#include<vector>

#include"C++HelperFunctions.h"
#include"Structures.h"
#include"Observer.h"
#include"GameObject.h"
#include"Mouse_Picking.h"

using namespace std;
using namespace DirectX;
/// <summary>
/// class to communicate to all game objects in its list
/// </summary>
class GameObjectControllor:public Observer
{
public:
	GameObjectControllor();
	~GameObjectControllor();

	//subscribe to events
	void SubscribedEvents();
	//list controlls
	void AddGameObject(GameObject* NewObject);
	void AddGameObject(string name,Transform* NewTransfrom,Appearance* newApprance,ParticleModel* newPhysics);
	void RemoveGameObject(GameObject* removeGameObject);

	void Update(float DeltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* pConstantBuffer);
	//get event data
	void OnNotify(const void* entertity, Events event);
private:
	void CleanUp();
	//Tests for colitions
	void Test1CollitionOnTiltedPlane(float deltatTime);
	void Test2CollitionOnTiltedMoveingObjects(float deltatTime);
private:
	//list of game objecst to act on
	vector<GameObject*>_pGameObjects;

	//mouse data
	XMFLOAT2 _MousePos;
	Mouse_Picking _MousePicking;

	//CB
	ID3D11Buffer* _pConstantBuffer;
};

