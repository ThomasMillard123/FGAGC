#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "resource.h"
#include"Structs.h"

#include<iostream>
enum MaterialType{
	SuperShiny,
	Shiny,
	Center,
	noShine,
	LittleReflection
	
};


enum TransformationType
{
	Parent,
	Child
};

class GameObject
{

public:

	GameObject(MeshData mesh, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale, MaterialType materalType);
	~GameObject();

	 void Update(float t, TransformationType transformationType);
	 void Draw(ID3D11DeviceContext* immediateContext);
	
	 //recive the matrix of this object
	 DirectX::XMFLOAT4X4 GetWorldMatrix();
	 DirectX::XMFLOAT4X4 GettransfromationMatrix();
	 DirectX::XMFLOAT3 getPos();
	 DirectX::XMFLOAT3 getDirection();
	 DirectX::BoundingSphere getBoundingSphere();
	 //set new trasformations
	
	 void UpdateTranfromationMatrix(TransformationType transformationType);

	 void SetWorld(DirectX::XMFLOAT4X4 world);

	 void SetRotation(DirectX::XMFLOAT3 rotationSpeed);
	 void SetMovement(DirectX::XMFLOAT3 movmentSpeed);
	 void SetPosition(DirectX::XMFLOAT3 pos);
	 void SetScale(DirectX::XMFLOAT3 size);
	 void SetTextures(ID3D11ShaderResourceView* textureDiffuse, ID3D11ShaderResourceView* textureSpec, ID3D11ShaderResourceView* textureNormMap);
	 //update data
	 void UpdateConstantBuffer(ConstantBuffer& cb, DirectX::XMFLOAT4X4 parent);
	 
	 void SetMaterialType();
	MaterialType GetMaterialType();

	void CleanUp();
private:
	MeshData cMesh;
	DirectX::XMFLOAT4X4  cObjectWorld,cTransformationMatrix;
	DirectX::XMFLOAT3 cPos,cRotation,cScale;
	DirectX::XMFLOAT3 cDirection;
	MaterialType cMaterialType;
	DirectX::XMFLOAT4 cDiffuseMtrl;
	DirectX::XMFLOAT4 cAmbientMaterial;
	DirectX::XMFLOAT4 cSpecularMtrl;
	float cSpecularPower;

	ID3D11ShaderResourceView* cpTextureRV;
	ID3D11ShaderResourceView* cpTextureRVSpec;
	ID3D11ShaderResourceView* cpTextureRVNorm;
	
	bool cNRMEnable;
	bool cSpecEnable;



	DirectX::BoundingSphere cpBoundingSphere;
	void CreatBoundingItems();



	//new
	std::string _ObjectName;
	//Transfrom
	//apprance
	//Partiucal
	
};
