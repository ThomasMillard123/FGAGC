#pragma once
#include "Screen.h"


class ScreenGameLeveL :
    public Screen
{

public:
	ScreenGameLeveL(ID3D11Device* d3dDevice, ID3D11DeviceContext* immediateContext, ID3D11Buffer* constantBuffer, UINT windowHeight, UINT windowWidth);
	~ScreenGameLeveL();

	void Update(float t);
	void Draw(ConstantBuffer cb);

	void SetUp();
	void UpdateCb(ConstantBuffer& cb);

	void CleanUp();

private:
	GameObject* cpFloor;
	GameObject* cpSkyBox;
	
	ShaderManager* cpSkyBoxShader;
	ID3D11DepthStencilState* cpDSLessEqual;

	std::vector<GameObject*> cpWallList;

};

