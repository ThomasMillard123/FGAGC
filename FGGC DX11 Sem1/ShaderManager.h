#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include<string>
#include <vector>

#include "resource.h"
#include"Structs.h"

enum ShaderType
{
	NoLighting,
	PhongLighting,
	GourandLighting,
	SkyBox
};

class ShaderManager
{

public:

	ShaderManager(ID3D11DeviceContext* immediateContext, ID3D11Device* d3dDevice,ShaderType shaderType);
	~ShaderManager();

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	void UpdateShader(ShaderType shaderType);
	
private:

	//vertex/pixle shader
	ID3D11VertexShader* cpVertexShader;
	ID3D11PixelShader* cpPixelShader;
	ID3D11InputLayout* cpVertexLayout;
	WCHAR* cpFile;
	ID3D11DeviceContext* cpImmediateContext;
	ID3D11Device* cpD3dDevice;
	

	ShaderType cShaderType;
	void SetLayout(ID3DBlob* pVSBlob);
	void CleanUp();




};

