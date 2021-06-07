#include "ScreenGameLeveL.h"

ScreenGameLeveL::ScreenGameLeveL(ID3D11Device* pd3dDevice, ID3D11DeviceContext* ImmediateContext, ID3D11Buffer* _pConstantBuffer, UINT WindowHeight, UINT WindowWidth):Screen(pd3dDevice, ImmediateContext, _pConstantBuffer,  WindowHeight,  WindowWidth)
{
	SetUp();
}

ScreenGameLeveL::~ScreenGameLeveL()
{
	delete cpFloor;
	cpFloor = nullptr;

	delete cpSkyBox;
	cpSkyBox = nullptr;

	delete cpSkyBoxShader;
	cpSkyBoxShader = nullptr;

	if(cpDSLessEqual) cpDSLessEqual->Release();

	 cpWallList.clear();

	CleanUp();
}

void ScreenGameLeveL::Update(float t)
{
	Screen::Update(t);

	//camra Update
	cpCamera[0]->Update(t);
	cpCamera[1]->Update(t);
	cpCamera[2]->Update(t);

	if (cCurrentCamera != 2) {

		//user object Movement
		if (GetAsyncKeyState(VK_UP)) {

			cpGameObjects[1]->SetMovement(XMFLOAT3{ 0.0f,0.0f,1.0f*t });

		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			cpGameObjects[1]->SetMovement(XMFLOAT3{ 0.0f,0.0f,-1.0f*t });
		}
		if (GetAsyncKeyState(VK_LEFT)) {

			cpGameObjects[1]->SetRotation(XMFLOAT3{ 0.0f,-0.1f*t,0.0f });
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {

			cpGameObjects[1]->SetRotation(XMFLOAT3{ 0.0f,0.1f*t,0.0f });
		}
		else if (GetAsyncKeyState(VK_INSERT)) {
			cpGameObjects[1]->SetMovement(XMFLOAT3{ 1.0f*t,0.0f,0.0f });
		}
		else if (GetAsyncKeyState(VK_DELETE)) {
			cpGameObjects[1]->SetMovement(XMFLOAT3{ -1.0f*t,0.0f,0.0f });
		}
}
		//object movemnet
		cpGameObjects[0]->SetRotation(XMFLOAT3{ 0.1f*t,0.1f*t,0.1f*t });
		cpGameObjects[2]->SetRotation(XMFLOAT3{ 0.0f,0.1f/**t*/,0.0f });
		cpGameObjects[3]->SetRotation(XMFLOAT3{ 0.0f,0.1f*t,0.0f });

		//update game objects
		cpGameObjects[0]->Update(t, Parent);
		cpGameObjects[1]->Update(t, Parent);
		cpGameObjects[2]->Update(t, Parent);
		cpGameObjects[3]->Update(t, Parent);

		//set camera objects to follow
		cpCamera[0]->SetParent(cpGameObjects[1]->getPos());
		cpCamera[1]->SetParent(cpGameObjects[1]->getPos());
	
	//skybox update and move with camrea
	cpSkyBox->Update(0, Parent);
		cpSkyBox->SetScale(XMFLOAT3(10.0f, 10.0f, 10.0f));
		cpSkyBox->SetPosition(cpCamera[cCurrentCamera]->GetEye());
}

void ScreenGameLeveL::Draw(ConstantBuffer cb)
{
	//update the shader settings
	cpImmediateContext->VSSetShader(cpShader->GetVertexShader(), nullptr, 0);
	cpImmediateContext->VSSetConstantBuffers(0, 1, &cpConstantBuffer);
	cpImmediateContext->PSSetConstantBuffers(0, 1, &cpConstantBuffer);
	cpImmediateContext->PSSetShader(cpShader->GetPixelShader(), nullptr, 0);

	// "fine-tune" the blending equation
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };

	// Set the default blend state (no blending) for opaque objects
	cpImmediateContext->OMSetBlendState(0, 0, 0xffffffff);

	//draw floor
	cpFloor->UpdateConstantBuffer(cb,cpWorld);
	cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
	cpFloor->Draw(cpImmediateContext);

	
		cpGameObjects[0]->UpdateConstantBuffer(cb, cpWorld);
		cb.WorldViewProjection = XMMatrixTranspose(XMLoadFloat4x4(&cpGameObjects[0]->GetWorldMatrix()) * XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetViewMatrix()) * XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetProjectionMatrix()));
		cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
		cpGameObjects[0]->Draw(cpImmediateContext);
		
		cpImmediateContext->RSSetState(cpSolidFillNoCull);
		cpGameObjects[1]->UpdateConstantBuffer(cb, cpWorld);
		cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
		cpGameObjects[1]->Draw(cpImmediateContext);
		
		cpImmediateContext->RSSetState(cpSolidFillNoCull);
		cpGameObjects[3]->UpdateConstantBuffer(cb, cpGameObjects[2]->GetWorldMatrix());
		cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
		cpGameObjects[3]->Draw(cpImmediateContext);


		for (int i = 0; i < cpWallList.size(); i++) {
			cpWallList[i]->UpdateConstantBuffer(cb,cpWorld);
			cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
			cpWallList[i]->Draw(cpImmediateContext);
		}
	
		cpImmediateContext->RSSetState(cpWireFrame);
		cpGameObjects[4]->UpdateConstantBuffer(cb, cpWorld);
		cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
		cpGameObjects[4]->Draw(cpImmediateContext);

	/* Set the blend state for transparent objects*/
	cpImmediateContext->OMSetBlendState(cpTransparency, blendFactor, 0xffffffff);
	
	cpImmediateContext->RSSetState(cpSolidFillNoCull);
	cpGameObjects[2]->UpdateConstantBuffer(cb, cpWorld);
	cb.WorldViewProjection = XMMatrixTranspose(XMLoadFloat4x4(&cpGameObjects[2]->GetWorldMatrix()) * XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetViewMatrix()) * XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetProjectionMatrix()));
	cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
	cpGameObjects[2]->Draw(cpImmediateContext);
		

	//skybox
	cpImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	cpImmediateContext->RSSetState(cpSolidFillNoCull);
	cpImmediateContext->VSSetShader(cpSkyBoxShader->GetVertexShader(), nullptr, 0);
	cpImmediateContext->VSSetConstantBuffers(0, 1, &cpConstantBuffer);
	cpImmediateContext->PSSetConstantBuffers(0, 1, &cpConstantBuffer);
	cpImmediateContext->PSSetShader(cpSkyBoxShader->GetPixelShader(), nullptr, 0);
	cpImmediateContext->OMSetDepthStencilState(cpDSLessEqual, 0);
	

	cpSkyBox->UpdateConstantBuffer(cb, cpWorld);
	cb.WorldViewProjection = XMMatrixTranspose(XMLoadFloat4x4(&cpSkyBox->GetWorldMatrix()) * XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetViewMatrix()) * XMLoadFloat4x4(&cpCamera[cCurrentCamera]->GetProjectionMatrix()));
	cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
	cpSkyBox->Draw(cpImmediateContext);

	cpImmediateContext->OMSetDepthStencilState(NULL, 0);
	cpImmediateContext->RSSetState(cpSolidFill);

	

	
}

void ScreenGameLeveL::SetUp()
{ 
	//load objects from json file
	JsonFilePars::LoadFile("Level.json", cObjectData, cCameraData, cLightData);
		
	//setup vertex/pixle shaders
	cpShader = new ShaderManager(cpImmediateContext, cpD3dDevice,PhongLighting);
	cpSkyBoxShader = new ShaderManager(cpImmediateContext, cpD3dDevice, SkyBox);
	
	//set up depth state to allow for skybox to be drawn 
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	cpD3dDevice->CreateDepthStencilState(&dssDesc, &cpDSLessEqual);
	
	//blending
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	cpD3dDevice->CreateBlendState(&blendDesc, &cpTransparency);

	
	//set world to identaty matrix
	XMStoreFloat4x4(&cpWorld, XMMatrixIdentity());

	//setup raster states
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_BACK;
	cpD3dDevice->CreateRasterizerState(&wfdesc, &cpSolidFill);

	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	cpD3dDevice->CreateRasterizerState(&wfdesc, &cpSolidFillNoCull);

	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	cpD3dDevice->CreateRasterizerState(&wfdesc, &cpWireFrame);
	//set up  cameras
	for each (JsonFilePars::CameraData c in cCameraData)
	{
		XMFLOAT3 Pos = { c.Pos.x,c.Pos.y,c.Pos.z };
		XMFLOAT3 At = { c.At.x,c.At.y,c.At.z };
		XMFLOAT3 Up = { c.Up.x,c.Up.y,c.Up.z };
		CameraMode mode;
		std::string CamTypeString = c.Type;
		std::transform(CamTypeString.begin(), CamTypeString.end(), CamTypeString.begin(), ::tolower);


		if (CamTypeString == "static") {
			mode = Static;
		}
		else if (CamTypeString == "firstperson")
		{
			mode = FirstPerson;
		}
		else if (CamTypeString == "thirdperson")
		{
			mode = ThirdPerson;
		}
		else if (CamTypeString == "free")
		{
			mode = Free;
		}
		else if (CamTypeString == "look at movemnt cam")
		{
			mode = LookAtMovemntCam;
		}
		else
		{
			//defulat
			mode = Static;
		}

		cpCamera.push_back(new CameraManager(Pos, At, Up, mode, cWindowWidth, cWindowHeight, 0.01f, 100.0f));
	}
	
	//load light data
	for each (JsonFilePars::LightInfo l in cLightData)
	{
		LightData LightData;
		LightData.LightVecW = XMFLOAT3{ l.Pos.x ,l.Pos.y ,l.Pos.z };
		LightData.DiffuseLight = XMFLOAT4(l.DiffuseLight.x, l.DiffuseLight.y, l.DiffuseLight.z, 1.0f);
		LightData.AmbientLight = XMFLOAT4(l.AmbientLight.x, l.AmbientLight.y, l.AmbientLight.z, 1.0f);
		LightData.SpecularLight = XMFLOAT4(l.SpecularLight.x, l.SpecularLight.y, l.SpecularLight.z, 1.0f);
		cpLight.push_back(new LightManager(LightData));
	}
	//Load objects
	for each (JsonFilePars::ObjectData o in cObjectData)
	{
		
		if (o.FileName == "Cube") {
			cMesh = ObjectBuilder::Cube(cpD3dDevice);
			
		}
		else if(o.FileName == "Pyramid")
		{
			cMesh = ObjectBuilder::Pyramid(cpD3dDevice);
			
		}
		else
		{
			
			char* cstr = new char[o.FileName.length() + 1];
			strcpy(cstr, o.FileName.c_str());
			cMesh = OBJLoader::Load(cstr, cpD3dDevice, false);
			
		}

		XMFLOAT3 Pos = XMFLOAT3(o.Pos.x,o.Pos.y,o.Pos.z);
		XMFLOAT3 Rotation = XMFLOAT3(o.Rotation.x, o.Rotation.y, o.Rotation.z);
		XMFLOAT3 Scale = XMFLOAT3(o.Scale.x, o.Scale.y, o.Scale.z);
		GameObject* Object = new GameObject(cMesh, Pos, Rotation, Scale,Center);
		cpGameObjects.push_back(Object);

	}

	//load floor mesh
	cMesh = ObjectBuilder::HorsontalPlane(cpD3dDevice, 100, 100);
	
	cpFloor = new GameObject(cMesh, XMFLOAT3{ -50,-1,-50 }, XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 2,2,2 },Center);
	
	//load walls
	cMesh = ObjectBuilder::VerticalPlane(cpD3dDevice, 210, 2);

	cpWallList.push_back(new GameObject(cMesh, XMFLOAT3{ -50,-1,-50 }, XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 1,1,1 }, Center));
	cpWallList.push_back(new GameObject(cMesh, XMFLOAT3{ -50,-1,150 }, XMFLOAT3{ 0,1.5708f,0 }, XMFLOAT3{ 1,1,1 }, Center));
	cpWallList.push_back(new GameObject(cMesh, XMFLOAT3{ -50,-1,150 }, XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 1,1,1 }, Center));
	cpWallList.push_back(new GameObject(cMesh, XMFLOAT3{ 140,-1,150 }, XMFLOAT3{ 0,1.5708f,0 }, XMFLOAT3{ 1,1,1 }, Center));

	cMesh = OBJLoader::Load("Mesh/sphere.obj",cpD3dDevice, false);
	cpSkyBox = new GameObject(cMesh, XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 1,1,1 }, Center);

	//load Textures
	ID3D11ShaderResourceView* _pTextureRV;
	ID3D11ShaderResourceView* _pTextureRVSpec;
	ID3D11ShaderResourceView* _pTextureRVNorm;
	CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Crate_COLOR.dds", nullptr, &_pTextureRV);
	CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Crate_SPEC.dds", nullptr, &_pTextureRVSpec);
	CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Crate_NRM.dds", nullptr, &_pTextureRVNorm);

	//set textuers to ojects
	
		cpGameObjects[0]->SetTextures(_pTextureRV, _pTextureRVSpec, _pTextureRVNorm);
		cpGameObjects[2]->SetTextures(_pTextureRV, _pTextureRVSpec, NULL);
		cpGameObjects[3]->SetTextures(_pTextureRV, _pTextureRVSpec, NULL);
		cpGameObjects[4]->SetTextures(_pTextureRV, _pTextureRVSpec,NULL);
		//load tank texture
		CreateDDSTextureFromFile(cpD3dDevice, L"Textures/IS-7.dds", nullptr, &_pTextureRV);
		CreateDDSTextureFromFile(cpD3dDevice, L"Textures/IS-7_SM.dds", nullptr, &_pTextureRVSpec);
		CreateDDSTextureFromFile(cpD3dDevice, L"Textures/IS-7_NM.dds", nullptr, &_pTextureRVNorm);
		cpGameObjects[1]->SetTextures(_pTextureRV, _pTextureRVSpec, _pTextureRVNorm);
		
		//load Floor texture
		CreateDDSTextureFromFile(cpD3dDevice, L"Textures/road texture.dds",nullptr, &_pTextureRV);
		cpFloor->SetTextures(_pTextureRV, NULL, NULL);
		cpWallList[0]->SetTextures(_pTextureRV, NULL, NULL);
		cpWallList[1]->SetTextures(_pTextureRV, NULL, NULL);
		cpWallList[2]->SetTextures(_pTextureRV, NULL, NULL);
		cpWallList[3]->SetTextures(_pTextureRV, NULL, NULL);
		//load cubemap texture
		CreateDDSTextureFromFileEx(cpD3dDevice,L"Textures/skymap.dds",0,D3D11_USAGE_DEFAULT,D3D11_BIND_SHADER_RESOURCE,0, D3D11_RESOURCE_MISC_TEXTURECUBE,false,nullptr,&_pTextureRV);
		cpSkyBox->SetTextures(_pTextureRV, NULL, NULL);

}

/// <summary>
/// update the constant buffer with the data from the sceen
/// </summary>
void ScreenGameLeveL::UpdateCb(ConstantBuffer& cb)
{
	Screen::UpdateCb(cb);
}

void ScreenGameLeveL::CleanUp()
{
	delete cpFloor;
	cpFloor = nullptr;
	delete cpSkyBox;
	cpSkyBox = nullptr;

	delete cpSkyBoxShader;
	cpSkyBoxShader = nullptr;

	if (cpDSLessEqual) cpDSLessEqual->Release();
	
	for (int i = 0; i < cpWallList.size(); i++)
	{
		delete cpWallList[i];
	}
	
	cpWallList.empty();
}
