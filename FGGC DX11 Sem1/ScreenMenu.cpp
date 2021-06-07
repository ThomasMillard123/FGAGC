#include "ScreenMenu.h"
#include<DirectXCollision.h>

ScreenMenu::ScreenMenu(ID3D11Device* pd3dDevice, ID3D11DeviceContext* ImmediateContext, ID3D11Buffer* _pConstantBuffer, UINT WindowHeight, UINT WindowWidth):Screen(pd3dDevice,ImmediateContext, _pConstantBuffer,  WindowHeight,  WindowWidth) {

	
	SetUp();
}

ScreenMenu::~ScreenMenu()
{
	CleanUp();
}

void ScreenMenu::SetUp()
{
	//read in settings from file
	JsonFilePars::LoadFile("SetupFile.json", cObjectData, cCameraData, cLightData);
	

	//setup vertex/pixle shaders
	cpShader = new ShaderManager(cpImmediateContext,cpD3dDevice, GourandLighting);

	//set up belnding
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
	wfdesc.FillMode = D3D11_FILL_SOLID;//fill
	wfdesc.CullMode = D3D11_CULL_BACK;//cull back
	cpD3dDevice->CreateRasterizerState(&wfdesc, &cpSolidFill);

	//setup camrea
	for each (JsonFilePars::CameraData c in cCameraData)
	{
		XMFLOAT3 Pos = { c.Pos.x,c.Pos.y,c.Pos.z };
		XMFLOAT3 At= { c.At.x,c.At.y,c.At.z };
		XMFLOAT3 Up= { c.Up.x,c.Up.y,c.Up.z };
		CameraMode mode;
		std::string CamTypeString=c.Type;
		std::transform(CamTypeString.begin(), CamTypeString.end(), CamTypeString.begin(), ::tolower);

		
		if (CamTypeString == "static") {
			mode = Static;
		}
		else if(CamTypeString =="firstPerson")
		{
			mode = FirstPerson;
		}
		else if (CamTypeString == "thirdPerson")
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

		cpCamera.push_back(new CameraManager(Pos, At, Up, mode, cWindowWidth, cWindowHeight, 0.01f, 1000.0f));
	}


	//Set upLights
	for each (JsonFilePars::LightInfo l in cLightData)
	{
		LightData LightData;
		LightData.LightVecW = XMFLOAT3{ l.Pos.x ,l.Pos.y ,l.Pos.z };
		LightData.DiffuseLight = XMFLOAT4(l.DiffuseLight .x, l.DiffuseLight.y, l.DiffuseLight.z, 1.0f);
		LightData.AmbientLight = XMFLOAT4(l.AmbientLight.x, l.AmbientLight.y, l.AmbientLight.z, 1.0f);
		LightData.SpecularLight = XMFLOAT4(l.SpecularLight.x, l.SpecularLight.y, l.SpecularLight.z, 1.0f);
		cpLight.push_back(new LightManager(LightData));
	}


	//setup objects

	for each (JsonFilePars::ObjectData o in cObjectData)
	{

		if (o.FileName == "Cube") {
			cMesh = ObjectBuilder::Cube(cpD3dDevice);
			
		}
		else if (o.FileName == "Pyramid")
		{
			cMesh = ObjectBuilder::Pyramid(cpD3dDevice);
			
		}
		else
		{

			char* cstr = new char[o.FileName.length() + 1];
			strcpy(cstr, o.FileName.c_str());
			cMesh = OBJLoader::Load(cstr, cpD3dDevice, false);
			
		}

		XMFLOAT3 Pos = XMFLOAT3(o.Pos.x, o.Pos.y, o.Pos.z);
		XMFLOAT3 Rotation = XMFLOAT3(o.Rotation.x, o.Rotation.y, o.Rotation.z);
		XMFLOAT3 Scale = XMFLOAT3(o.Scale.x, o.Scale.y, o.Scale.z);
		GameObject* Object = new GameObject(cMesh, Pos, Rotation, Scale,Shiny);
		cpGameObjects.push_back(Object);

	}

	//load Textures
	ID3D11ShaderResourceView* _pTextureRV;
	ID3D11ShaderResourceView* _pTextureRVSpec;
	ID3D11ShaderResourceView* _pTextureRVNorm;
	CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Crate_COLOR.dds", nullptr, &_pTextureRV);
	CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Crate_SPEC.dds", nullptr, &_pTextureRVSpec);
	CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Crate_NRM.dds", nullptr, &_pTextureRVNorm);

	//set textuers to ojects
	
		cpGameObjects[2]->SetTextures(_pTextureRV, _pTextureRVSpec, _pTextureRVNorm);
		CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Play.dds", nullptr, &_pTextureRV);
		cpGameObjects[0]->SetTextures(_pTextureRV, NULL, NULL); 
		CreateDDSTextureFromFile(cpD3dDevice, L"Textures/Exit.dds", nullptr, &_pTextureRV);
		cpGameObjects[1]->SetTextures(_pTextureRV, NULL, NULL);
	
	
	
}

void ScreenMenu::UpdateCb(ConstantBuffer& cb)
{
	Screen::UpdateCb(cb);
}

/// <summary>
/// Picking on objects in envroment
/// </summary>
void ScreenMenu::Picking()
{
	
	//NDC
	float NDC_X = (2.0f * cMousePos.x) / cWindowWidth - 1.0f;
	float NDC_Y= 1.0f-(2.0f * cMousePos.y) / cWindowHeight ;

	 
	XMVECTOR rayOrigin = XMVectorSet(NDC_X, NDC_Y, 0.0f, 0.0f);
	
	//invert projection and View
	XMMATRIX P = XMLoadFloat4x4( &cpCamera[0]->GetProjectionMatrix());
	XMMATRIX InvertP = XMMatrixInverse(nullptr,P);
	XMMATRIX V = XMLoadFloat4x4(&cpCamera[0]->GetViewMatrix());
	XMMATRIX InvertV = XMMatrixInverse(nullptr, V);
	XMVECTOR eyePos = XMLoadFloat3(&cpCamera[0]->GetEye());

	//covert ray to world
	rayOrigin = XMVector3TransformCoord(rayOrigin, InvertP);
	rayOrigin = XMVector3TransformCoord(rayOrigin, InvertV);
	//get the ray direction
	XMVECTOR rayDir = rayOrigin - eyePos;
	rayDir = XMVector3Normalize(rayDir);

	
	//find what was was hit using bounding sphere
	float distance = 1;
	
	
		if (cpGameObjects[0]->getBoundingSphere().Intersects(rayOrigin, rayDir, distance)) {
		//hit
			//Play
			cNextScreen = Level;
			cIsChangeScreen = true;
		}
		else if (cpGameObjects[1]->getBoundingSphere().Intersects(rayOrigin, rayDir, distance)) {
			//hit
				//exit
			cNextScreen = Exit;
			cIsChangeScreen = true;
		}
		
}

void ScreenMenu::Update(float t) {
	Screen::Update(t);


	if (GetAsyncKeyState(VK_NUMPAD0)) {
		cCurrentLight = 0;
	}
	else if (GetAsyncKeyState(VK_NUMPAD1)) {
		cCurrentLight = 1;
	}
	else if (GetAsyncKeyState(VK_NUMPAD2)) {
		cCurrentLight = 0;
	}
	else if (GetAsyncKeyState(VK_NUMPAD3)) {
		cCurrentLight = 1;
	}
	else if (GetAsyncKeyState(VK_NUMPAD4)) {
		cCurrentLight = 0;
	}



	//start picking
	if (GetAsyncKeyState(VK_LBUTTON)) {
		Picking();
	}


	
	//camra update
	cpCamera[cCurrentCamera]->Update(t);

	
	//update objects
	for (int i = 0; i < cpGameObjects.size(); i++)
	{
		cpGameObjects[i]->Update(0, Parent);
		
	}

	
	char sz[1024] = { 0 };
	sprintf_s(sz, "the number is %f \n", 1.0f/t);
	OutputDebugStringA(sz);

	
	
}

//cb untill vertex shaders set
void ScreenMenu::Draw(ConstantBuffer cb) {

	
	//update the shader settings
	cpImmediateContext->VSSetShader(cpShader->GetVertexShader(), nullptr, 0);
	cpImmediateContext->VSSetConstantBuffers(0, 1, &cpConstantBuffer);
	cpImmediateContext->PSSetConstantBuffers(0, 1, &cpConstantBuffer);
	cpImmediateContext->PSSetShader(cpShader->GetPixelShader(), nullptr, 0);


	// "fine-tune" the blending equation
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };

	// Set the default blend state (no blending) for opaque objects
	cpImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	for (int i = 0; i < cpGameObjects.size(); i++)
	{
		cpGameObjects[i]->UpdateConstantBuffer(cb, cpWorld);
		cpImmediateContext->UpdateSubresource(cpConstantBuffer, 0, nullptr, &cb, 0, 0);
		cpGameObjects[i]->Draw(cpImmediateContext);
	}


	

}