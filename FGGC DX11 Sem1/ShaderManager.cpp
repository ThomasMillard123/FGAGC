#include "ShaderManager.h"

ShaderManager::ShaderManager(ID3D11DeviceContext* immediateContext, ID3D11Device* d3dDevice,ShaderType shaderType)
{
    cpImmediateContext = immediateContext;
    cpD3dDevice = d3dDevice;
    // Define the shader type
    cShaderType = shaderType;

    switch (cShaderType)
    {
    case NoLighting:
        cpFile = L"NoLighting.fx";
        break;
    case PhongLighting:
        cpFile = L"PhongLighting.fx";
        break;
    case GourandLighting:
        cpFile = L"Gouraud.fx";
        break;
    case SkyBox:
        cpFile = L"SKYBOX.fx";
        break;
    default:
        cpFile = L"DX11 Framework.fx";
        break;
    }



    InitShadersAndInputLayout();
}

ShaderManager::~ShaderManager()
{
    CleanUp();
}


HRESULT ShaderManager::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    
    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT ShaderManager::InitShadersAndInputLayout()
{
    HRESULT hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    
    hr = CompileShaderFromFile(cpFile, "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    // Create the vertex shader
    hr = cpD3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &cpVertexShader);

    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    // Compile the pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(cpFile, "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    // Create the pixel shader
    hr = cpD3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &cpPixelShader);
    pPSBlob->Release();

    if (FAILED(hr))
        return hr;
   
    // Define the input layout
    SetLayout(pVSBlob);

   
    // Set the input layout
    cpImmediateContext->IASetInputLayout(cpVertexLayout);

    return hr;
}

ID3D11VertexShader* ShaderManager::GetVertexShader()
{
    return cpVertexShader;
}

ID3D11PixelShader* ShaderManager::GetPixelShader()
{
    return cpPixelShader;
}

void ShaderManager::UpdateShader(ShaderType shaderType)
{
    cShaderType = shaderType;
    switch (cShaderType)
    {
    case NoLighting:
        cpFile = L"NoLighting.fx";
        break;
    case PhongLighting:
        cpFile = L"PhongLighting.fx";
        break;
    case GourandLighting:
        cpFile = L"Gouraud.fx";
        break;
    case SkyBox:
        cpFile = L"SKYBOX.fx";
        break;
    default:
        cpFile = L"DX11 Framework.fx";
        break;
    }
    InitShadersAndInputLayout();
}



void ShaderManager::SetLayout(ID3DBlob* pVSBlob)
{
    UINT numElements;

 

    //set input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"TEXTCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        numElements = ARRAYSIZE(layout);
        cpD3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &cpVertexLayout);

    
   
       
    pVSBlob->Release();
}

void ShaderManager::CleanUp()
{
    if (cpImmediateContext) cpImmediateContext->ClearState();
    if (cpImmediateContext) cpImmediateContext->Release();
    if (cpD3dDevice) cpD3dDevice->Release();
    if (cpVertexLayout) cpVertexLayout->Release();
    if (cpVertexShader) cpVertexShader->Release();
    if (cpPixelShader) cpPixelShader->Release();
    delete cpFile;
    cpFile = nullptr;
}
