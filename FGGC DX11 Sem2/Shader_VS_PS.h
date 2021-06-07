#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

using namespace DirectX;
/// <summary>
/// class to load and store shader data
/// </summary>
class Shader_VS_PS
{
public:
	Shader_VS_PS(ID3D11Device* _pd3dDevice, ID3D11DeviceContext* _pImmediateContext,WCHAR* File);
	~Shader_VS_PS();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();
	ID3D11InputLayout* GetVertexLayout();
	ID3D11SamplerState** GetSamplerLinear();
private:

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout(ID3D11Device* _pd3dDevice, ID3D11DeviceContext* _pImmediateContext, WCHAR* File);

	void CleanUp();

private:

	WCHAR* _FileName;

	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;
	ID3D11SamplerState* _pSamplerLinear = nullptr;

};

