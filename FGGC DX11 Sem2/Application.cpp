#include "Application.h"
#include"ObjectBuilder.h"
#include"EventSystem.h"

#include"Rain_ParticalSystem.h"
#include"Smoke_ParticalSystem.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Application::HandleKeyboard(MSG msg)
{
	if (_pInputConrollor != nullptr) {
		return _pInputConrollor->HandleInput(msg);
	}

	return false;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pConstantBuffer = nullptr;
	CCWcullMode=nullptr;
	CWcullMode= nullptr;
	DSLessEqual = nullptr;
	RSCullNone = nullptr;
	 _WindowHeight = 0;
	 _WindowWidth = 0;
}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }
	_pInputConrollor = new InputControllor();

	CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\stone.dds", nullptr, &_pTextureRV);

	
    // Setup Camera
	XMFLOAT3 eye = XMFLOAT3(0.0f, 2.0f, -1.0f);
	XMFLOAT3 at = XMFLOAT3(0.0f, 2.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	_camera = new Camera(eye, at, up, (float)_renderWidth, (float)_renderHeight, 0.01f, 200.0f);


	
	// Setup the scene's light
	basicLight.AmbientLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	basicLight.DiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	basicLight.SpecularLight = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	basicLight.SpecularPower = 20.0f;
	basicLight.LightVecW = XMFLOAT3(0.0f, 1.0f, -1.0f);

	//geometry
	Geometry herculesGeometry;
	objMeshData = OBJLoader::Load("donut.obj", _pd3dDevice);
	herculesGeometry.indexBuffer = objMeshData.IndexBuffer;
	herculesGeometry.numberOfIndices = objMeshData.IndexCount;
	herculesGeometry.vertexBuffer = objMeshData.VertexBuffer;
	herculesGeometry.vertexBufferOffset = objMeshData.VBOffset;
	herculesGeometry.vertexBufferStride = objMeshData.VBStride;
	
	Geometry cubeGeometry;
	MeshData cubemesh = ObjectBuilder::Cube(_pd3dDevice);
	cubeGeometry.indexBuffer = cubemesh.IndexBuffer;
	cubeGeometry.vertexBuffer = cubemesh.VertexBuffer;
	cubeGeometry.numberOfIndices = cubemesh.IndexCount;
	cubeGeometry.vertexBufferOffset = cubemesh.VBOffset;
	cubeGeometry.vertexBufferStride = cubemesh.VBStride;

	Geometry planeGeometry;
	MeshData planeGeometryMesh = ObjectBuilder::HorsontalPlane(_pd3dDevice, 1, 1);
	planeGeometry.indexBuffer = planeGeometryMesh.IndexBuffer;
	planeGeometry.vertexBuffer = planeGeometryMesh.VertexBuffer;
	planeGeometry.numberOfIndices = planeGeometryMesh.IndexCount;
	planeGeometry.vertexBufferOffset = planeGeometryMesh.VBOffset;
	planeGeometry.vertexBufferStride = planeGeometryMesh.VBStride;

	//matertials
	Material shinyMaterial;
	shinyMaterial.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	shinyMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	shinyMaterial.specularPower = 10.0f;

	Material noSpecMaterial;
	noSpecMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	noSpecMaterial.specularPower = 0.0f;

	
	


	ParticleModel* particalmodle;
	Appearance* appranance;
	Transform* transfrom = new Transform();

	transfrom->SetPosition(-50, 0, -50);
	transfrom->SetScale(2, 2, 2);
	_pGameObjectControllor = new GameObjectControllor;
	GameObject* gameObject;

	//trrain
	_Terrain = new Terrain("TerrainAssets\\Heightmap 513x513.raw", transfrom, _pd3dDevice,_pImmediateContext);
	appranance = new Appearance(shinyMaterial, cubeGeometry);
	appranance->SetTextureRV(_pTextureRV);

	//cubes
	for (auto i = 0; i < NUMBER_OF_CUBES; i++)
	{
		Transform* transfrom =  new Transform();
		
		transfrom->SetScale(0.5f, 0.5f, 0.5f);
		
		if (i >= 1 && i<5) {
			ParticleModel* particalmodle = new RigidBody(transfrom, false, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f });
			
			gameObject = new GameObject("Cube " + i, appranance, transfrom, particalmodle);
			transfrom->SetPosition(-4.0f + (i * 2.0f), 0.5f, 10.0f);
		}
		else if(i>=5 && i<6)
		{
			ParticleModel* particalmodle = new ParticleModel(transfrom, false, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, 20 * (i + 1));
			gameObject = new GameObject("Cube " + i, appranance, transfrom, particalmodle);
			transfrom->SetPosition(-10.0f, 5.0f, 0.0f);
		}
		else if (i >= 6 && i < 9)
		{
			ParticleModel* particalmodle = new ParticleModel(transfrom, true, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, 20);
			gameObject = new GameObject("Cube " + i, appranance, transfrom, particalmodle);
			if (i == 6) {
				transfrom->SetPosition(10.0f, 0.5f, -5.0f);
			}
			if (i == 7) {
				transfrom->SetPosition(10.0f, 0.5f, 5.0f);
			}
		}
		else
		{
			ParticleModel* particalmodle = new ParticleModel(transfrom, true, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f },20*(i+1));
			gameObject = new GameObject("Cube " + i, appranance, transfrom, particalmodle);
			transfrom->SetPosition(-4.0f + (i * 2.0f), 5.0f, 10.0f);
		}

		
		_pGameObjectControllor->AddGameObject(gameObject);
	}

	
	//planes for testing
	for (auto i = 0; i < NUMBER_OF_PLANE; i++)
	{
		particalmodle = new RigidBody(transfrom, false, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f });
		appranance = new  Appearance(shinyMaterial, planeGeometry);
		appranance->SetTextureRV(_pTextureRV);
		transfrom = new Transform();
		transfrom->SetRotation(-30, 0, 0);
		transfrom->SetPosition(-10, 0.5, 0);
		gameObject = new GameObject("Plane", appranance, transfrom, particalmodle);

		_pGameObjectControllor->AddGameObject(gameObject);
	}

	

	transfrom = new Transform();
	transfrom->SetScale(0.7f, 0.7f, 0.7f);
	transfrom->SetPosition(0.0f, 0.5f, -10.0f);
	particalmodle = new ParticleModel(transfrom, false, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f },0);
	appranance = new Appearance(shinyMaterial, herculesGeometry);
	appranance->SetTextureRV(_pTextureRV);
	gameObject = new GameObject("donut", appranance, transfrom, particalmodle);
	
	 _pGameObjectControllor->AddGameObject(gameObject);
	

	 CreateDDSTextureFromFile(_pd3dDevice, L"TerrainAssets\\grass.dds", nullptr, &_pTextureRV);
	 planeGeometryMesh = ObjectBuilder::HorsontalPlane(_pd3dDevice, 100, 100);
	 planeGeometry.indexBuffer = planeGeometryMesh.IndexBuffer;
	 planeGeometry.vertexBuffer = planeGeometryMesh.VertexBuffer;
	 planeGeometry.numberOfIndices = planeGeometryMesh.IndexCount;
	 planeGeometry.vertexBufferOffset = planeGeometryMesh.VBOffset;
	 planeGeometry.vertexBufferStride = planeGeometryMesh.VBStride;

	 particalmodle = new RigidBody(transfrom, false, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f });
	 appranance = new  Appearance(shinyMaterial, planeGeometry);
	 appranance->SetTextureRV(_pTextureRV);
	 transfrom = new Transform();
	
	 transfrom->SetPosition(-50, 0, -50);
	 gameObject = new GameObject("Floor", appranance, transfrom, particalmodle);

	 _pGameObjectControllor->AddGameObject(gameObject);

	//partical systems
	_ParticalSystemManager = new ParticalSystem_Manager();

	Rain_ParticalSystem* Rain = new Rain_ParticalSystem(50, { 0.0f,10.0f,-10.0f }, "Rain1");
	

	CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\rain.dds", nullptr, &_pTextureRV);
	appranance = new Appearance(shinyMaterial, cubeGeometry);
	appranance->SetTextureRV(_pTextureRV);

	Rain->Setapprance(appranance);
	Rain->SetMaxHight(10);
	Rain->SetMaxLifeSpan(10);
	Rain->SetRadius(5);
	Rain->LoadParticals();
	_ParticalSystemManager->AddParticalSystem(Rain);
	
	Smoke_ParticalSystem* Smoke = new Smoke_ParticalSystem(100, { 0.0f,0.5f,-10.0f }, "Smoke1");
	CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\stone.dds", nullptr, &_pTextureRV);
	appranance = new Appearance(shinyMaterial, cubeGeometry);
	appranance->SetTextureRV(_pTextureRV);

	Smoke->Setapprance(appranance);
	Smoke->SetMaxHight(3);
	Smoke->SetMaxLifeSpan(10);
	Smoke->SetRadius(1);
	Smoke->LoadParticals();
	
	_ParticalSystemManager->AddParticalSystem(Smoke);

	//notfy inslize observers
	EventSystem::Instance()->AddEvent(Events::ConstantBufferSet, _pConstantBuffer);
	EventSystem::Instance()->Notify();

	return S_OK;
}



HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 2080, 1080};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"FGGC Semester 2 Framework", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                         nullptr);
    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);


	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.hwndTarget = NULL;
	rid.dwFlags = 0;

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false) {
		return E_FAIL;
	}


    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	UINT sampleCount = 4;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _renderWidth;
    sd.BufferDesc.Height = _renderHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = sampleCount;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_renderWidth;
    vp.Height = (FLOAT)_renderHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	//InitShadersAndInputLayout();
	
	DefualtShader = new Shader_VS_PS(_pd3dDevice, _pImmediateContext, L"DX11 Framework.fx");

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

    if (FAILED(hr))
        return hr;

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _renderWidth;
	depthStencilDesc.Height = _renderHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = sampleCount;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

	// Rasterizer
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	_pd3dDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CWcullMode);

    return S_OK;
}

void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_pTextureRV) _pTextureRV->Release();
    if (_pConstantBuffer) _pConstantBuffer->Release();
    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();
    if (_pImmediateContext) _pImmediateContext->Release();
    if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();

	if (DSLessEqual) DSLessEqual->Release();
	if (RSCullNone) RSCullNone->Release();

	if (CCWcullMode) CCWcullMode->Release();
	if (CWcullMode) CWcullMode->Release();

	if (_camera)
	{
		delete _camera;
		_camera = nullptr;
	}

	if (_Terrain) {
		delete _Terrain;
		_Terrain = nullptr;
	}
	
}

void Application::Update()
{
	
    // Update time
    static float deltaTime = 0.0f;
	static DWORD dwTimeStart = 0;

    DWORD dwTimeCur = GetTickCount64();

    if (dwTimeStart == 0)
        dwTimeStart = GetTickCount64();;

		
	deltaTime = (dwTimeCur - dwTimeStart) / 1000.0f;
	//Lock to 60FPS
	if (deltaTime < FPS_60) {
		
		return;
	}

	//input
	_pInputConrollor->Update();
	// Update camera
	_camera->Update();
	//objects update
	_pGameObjectControllor->Update(deltaTime);
	_ParticalSystemManager->Update(deltaTime);
	

	dwTimeStart = dwTimeCur;


	//update render screen size
	RECT windowRect;
	if (GetClientRect(_hWnd, &windowRect)) {
		XMFLOAT2 windowsize = { (float)(windowRect.right - windowRect.left),(float)(windowRect.bottom - windowRect.top) };

		_renderHeight = windowsize.y;
		_renderWidth = windowsize.x;

		EventSystem::Instance()->AddEvent(Events::WindowSizeUpdate, &windowsize);
	}
	EventSystem::Instance()->Notify();
}

void Application::Draw()
{
    //
    // Clear buffers
    //

	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //
    // Setup buffers and render scene
    //

	_pImmediateContext->IASetInputLayout(DefualtShader->GetVertexLayout());

	_pImmediateContext->VSSetShader(DefualtShader->GetVertexShader(), nullptr, 0);
	_pImmediateContext->PSSetShader(DefualtShader->GetPixelShader(), nullptr, 0);

	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetSamplers(0, 1, DefualtShader->GetSamplerLinear());

    ConstantBuffer cb;

	XMFLOAT4X4 viewAsFloats = _camera->GetView();
	XMFLOAT4X4 projectionAsFloats = _camera->GetProjection();

	XMMATRIX view = XMLoadFloat4x4(&viewAsFloats);
	XMMATRIX projection = XMLoadFloat4x4(&projectionAsFloats);

	cb.View = XMMatrixTranspose(view);
	cb.Projection = XMMatrixTranspose(projection);
	
	cb.light = basicLight;
	cb.EyePosW = _camera->GetPosition();

	// Render all scene objects
	_pGameObjectControllor->Draw(_pImmediateContext, &cb);
	_ParticalSystemManager->Draw(_pImmediateContext, &cb);

	//terrain

	//load terrain shader
	_pImmediateContext->IASetInputLayout(_Terrain->GetTerrainShgader()->GetVertexLayout());

	_pImmediateContext->VSSetShader(_Terrain->GetTerrainShgader()->GetVertexShader(), nullptr, 0);
	_pImmediateContext->PSSetShader(_Terrain->GetTerrainShgader()->GetPixelShader(), nullptr, 0);

	//load terrain data
	Material material = _Terrain->GetAppreance()->GetMaterial();

	// Copy material to shader
	cb.surface.AmbientMtrl = material.ambient;
	cb.surface.DiffuseMtrl = material.diffuse;
	cb.surface.SpecularMtrl = material.specular;

	// Set world matrix
	cb.World = XMMatrixTranspose(XMLoadFloat4x4(&_Terrain->GetTransform()->GetWorldMatrix()));

	// Set texture
	if (_Terrain->GetAppreance()->HasTexture())
	{
		_pImmediateContext->PSSetShaderResources(0, 3, _Terrain->GetTextureList().data());
		cb.HasTexture = 1.0f;
	}
	else
	{
		cb.HasTexture = 0.0f;
	}

	// Update constant buffer
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	_Terrain->Draw(_pImmediateContext);


	
    //
    // Present our back buffer to our front buffer
    //
    _pSwapChain->Present(1, 0);
}