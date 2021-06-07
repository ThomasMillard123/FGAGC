#include "Terrain.h"
#include"DDSTextureLoader.h"
Terrain::Terrain(string hightMapFile, Transform* transfrom,  ID3D11Device* d3dDevice, ID3D11DeviceContext* pImmediateContext)
{

	HightMapFile = hightMapFile;
	_Transfrom=transfrom;
	

    _HeightmapWidth = 513;
	_HeightmapHeight = 513;
    _HeightScale = 8;

    //creat shader for terrain
    TerrainShader = new Shader_VS_PS(d3dDevice, pImmediateContext, L"Terrain.fx");

    LoadHightMap();
    CreatTerrain(d3dDevice);
}

Terrain::~Terrain()
{
}

void Terrain::Draw(ID3D11DeviceContext* pImmediateContext)
{
	_Appreance->Draw(pImmediateContext);
}
//load hight map data
void Terrain::LoadHightMap()
{
    // A height for each vertex 
    std::vector<unsigned char> in(_HeightmapWidth * _HeightmapHeight);

    // Open the file.
    std::ifstream inFile;
    inFile.open(HightMapFile.c_str(), std::ios_base::binary);

    if (inFile)
    {
        // Read the RAW bytes.
        inFile.read((char*)&in[0], (std::streamsize)in.size());
        // Done with file.
    
            inFile.close();
    }

    // Copy the array data into a float array and scale it. mHeightmap.resize(heightmapHeight * heightmapWidth, 0);
    _HightmapData.resize(_HeightmapHeight * _HeightmapWidth);
    for (UINT i = 0; i < _HeightmapHeight * _HeightmapWidth; ++i)
    {
        _HightmapData[i] = (in[i] / 255.0f) * _HeightScale;
    }

   
}

void Terrain::CreatTerrain(ID3D11Device* d3dDevice)
{


    //creat  plane with hight data
    int cols = _HeightmapWidth;
    int rows = _HeightmapHeight;
    int NumVertices = rows * cols;
    int NumFaces = (rows - 1) * (cols - 1) * 2;
    float width = 50.0f;
    float hight = 150.0f;
    //vertex
    std::vector<SimpleVertex>v;

    float dx = width / (cols - 1);
    float dz = hight / (rows - 1);
    float textCordY = 1.0f;
    float textCordX = 0.0f;
    float increasX = 1.0f / cols;
    float increasZ = 1.0f / rows;
    //creat vertex data
    for (UINT i = 0; i < rows; i++)
    {
        for (UINT j = 0; j < cols; j++)
        {
            v.push_back({ XMFLOAT3{ (float)(-0* width + j * dx),_HightmapData[i * cols + j]-2, (float)(0* hight - i * dz) },XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT2(textCordX,textCordY) });
            textCordX += increasX;
        }
        textCordY -= increasZ;
        textCordX = 0;
    }



    //indices creation

    std::vector<WORD>indices;
    for (UINT i = 0; i < rows-1 ; i++)
    {
        for (UINT j = 0; j < cols-1 ; j++)
        {

            indices.push_back(i * cols + j);

            indices.push_back(i * cols + (j + 1));

            indices.push_back((i + 1) * cols + j);


            indices.push_back((i + 1) * cols + j);

            indices.push_back(i * cols + (j + 1));

            indices.push_back((i + 1) * cols + (j + 1));



        }


    }

    ID3D11Buffer* _pPlaneVertexBuffer;
    ID3D11Buffer* _pPlaneIndexBuffer;

    //buffer creation
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * v.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &v[0];

    //store vertex data in buffer
    d3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneVertexBuffer);

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * indices.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &indices[0];
    //creat buffer
    d3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneIndexBuffer);


    Geometry planeGeometry;
    planeGeometry.indexBuffer = _pPlaneIndexBuffer;
    planeGeometry.vertexBuffer = _pPlaneVertexBuffer;
    planeGeometry.numberOfIndices = indices.size();
    planeGeometry.vertexBufferOffset = 0;
    planeGeometry.vertexBufferStride = sizeof(SimpleVertex);

    Material noSpecMaterial;
    noSpecMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    noSpecMaterial.specularPower = 0.0f;

    _Appreance = new Appearance(noSpecMaterial, planeGeometry);

    //load textures
    _pGroundTextureRV.resize(4);
    CreateDDSTextureFromFile(d3dDevice, L"TerrainAssets\\grass.dds", nullptr, &_pGroundTextureRV[0]);
    CreateDDSTextureFromFile(d3dDevice, L"TerrainAssets\\stone.dds", nullptr, &_pGroundTextureRV[1]);
    CreateDDSTextureFromFile(d3dDevice, L"TerrainAssets\\snow.dds", nullptr, &_pGroundTextureRV[2]);
    _Appreance->SetTextureRV(_pGroundTextureRV[0]);
}

void Terrain::CleanUp()
{
    for (UINT i = 0; i < _pGroundTextureRV.size(); i++)
    {
        if (_pGroundTextureRV[i]) {
            _pGroundTextureRV[i]->Release();
        }
    }

    _pGroundTextureRV.clear();

    delete _Appreance;
    _Appreance = nullptr;
    delete _Transfrom;
    _Transfrom = nullptr;
}


