#include "ObjectBuilder.h"

MeshData ObjectBuilder::HorsontalPlane(ID3D11Device* d3dDevice,int width,int hight)
{
    MeshData PlaneMesh;
    ID3D11Buffer* _pPlaneVertexBuffer;
    ID3D11Buffer* _pPlaneIndexBuffer;

    //vertex
    std::vector<VertexTangent>PlaneVerTex;
    //start from 0,0
    float CurrentX = 0.0f;
    float CurrentZ = 0.0f;

    int WidthV = width+1;
    int HightV = hight +1;

    const int totalV = WidthV * HightV;
    float textCordY = 1.0f;
    float textCordX = 0.0f;
    float increasX = 1.0f / width;
    float increasZ = 1.0f / hight;

    //creat vertex data
    for (int i = 0; i < HightV; i++)
    {
        for (int j = 0; j < WidthV; j++)
        {
            PlaneVerTex.push_back({ XMFLOAT3(CurrentX, 0.0f, CurrentZ), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT2(textCordX,textCordY),XMFLOAT3(1.0f, 0.0f, 1.0f) });
            CurrentX += 1.0f;
            textCordX += increasX;

        }
        textCordY -= increasZ;
        textCordX = 0;

        CurrentX = 0.0f;
        CurrentZ += 1.0f;

    }


    VertexTangent* Planevertices=new VertexTangent[PlaneVerTex.size()];
    unsigned int numMeshVertices = PlaneVerTex.size();
    for (int i = 0; i < totalV; i++)
    {
        Planevertices[i].Pos = PlaneVerTex[i].Pos;
        Planevertices[i].Normal = PlaneVerTex[i].Normal;
        Planevertices[i].TexC = PlaneVerTex[i].TexC;
        Planevertices[i].Tangent = PlaneVerTex[i].Tangent;
    }

    //buffer creation
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VertexTangent) * PlaneVerTex.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = Planevertices;

    //store vertex data in buffer
    d3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneVertexBuffer);

    //indices creation
    std::vector<WORD>PlaneIndices1;
    int i = 0;
    const int Width = width;
    const int Hight = hight;
    for (int y = 0; y < Hight; y++)
    {
        for (int x = 0; x < Width; x++)
        {

            PlaneIndices1.push_back(i);
            PlaneIndices1.push_back(i + Width + 1);
            PlaneIndices1.push_back(i + Width + 2);

            PlaneIndices1.push_back(i);
            PlaneIndices1.push_back(i + Width + 2);
            PlaneIndices1.push_back(i + 1);
            i++;
        }
        i++;
    }


    const int total = Width * Hight * 6;
    int size = (int)PlaneIndices1.size();


    WORD* PlaneIndices = new WORD[total];
  
    int planeIndCount = PlaneIndices1.size();

    for (int i = 0; i < planeIndCount; i++)
    {
        PlaneIndices[i] = PlaneIndices1[i];
    }

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * planeIndCount;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PlaneIndices;
    //creat buffer
    d3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneIndexBuffer);

    //store data in struct
    PlaneMesh.VertexBuffer = _pPlaneVertexBuffer;
    PlaneMesh.IndexBuffer = _pPlaneIndexBuffer;
    PlaneMesh.IndexCount = total;
    PlaneMesh.VBOffset = 0;
    PlaneMesh.VBStride = sizeof(VertexTangent);

   

    return PlaneMesh;
}

MeshData ObjectBuilder::VerticalPlane(ID3D11Device* d3dDevice, int width, int hight)
{
    MeshData PlaneMesh;
    ID3D11Buffer* _pPlaneVertexBuffer;
    ID3D11Buffer* _pPlaneIndexBuffer;

    //vertex
    std::vector<VertexTangent>PlaneVerTex;
    float CurrentX = 0.0f;
    float CurrentY = 0.0f;

    int WidthV = width + 1;
    int HightV = hight + 1;
    const int totalV = WidthV * HightV;
    float textCordY = 1.0f;
    float textCordX = 0.0f;
    float increasX = 1.0f / width;
    float increasY = 1.0f / hight;

    //vertex data
    for (int i = 0; i < HightV; i++)
    {
        for (int j = 0; j < WidthV; j++)
        {
            PlaneVerTex.push_back({ XMFLOAT3(CurrentX, CurrentY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2(textCordX,textCordY),XMFLOAT3(-1.0f, -1.0f, 0.0f) });
            CurrentX += 1.0f;
            textCordX += increasX;

        }
        textCordY -= increasY;
        textCordX = 0;

        CurrentX = 0.0f;
        CurrentY += 1.0f;

    }


    VertexTangent* Planevertices = new VertexTangent[PlaneVerTex.size()];
    unsigned int numMeshVertices = PlaneVerTex.size();
    for (int i = 0; i < numMeshVertices; i++)
    {
        Planevertices[i].Pos = PlaneVerTex[i].Pos;
        Planevertices[i].Normal = PlaneVerTex[i].Normal;
        Planevertices[i].TexC = PlaneVerTex[i].TexC;
        Planevertices[i].Tangent = PlaneVerTex[i].Tangent;
    }

    //buffer creation
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VertexTangent) * totalV;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = Planevertices;

    d3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneVertexBuffer);

    //indices
    std::vector<WORD>PlaneIndices1;
    int i = 0;
    int Width = width;
    int Hight = hight;
    for (int y = 0; y < Hight; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            PlaneIndices1.push_back(i);
            PlaneIndices1.push_back(i + Width + 1);
            PlaneIndices1.push_back(i + Width + 2);

            PlaneIndices1.push_back(i);
            PlaneIndices1.push_back(i + Width + 2);
            PlaneIndices1.push_back(i + 1);
            i++;
        }
        i++;
    }


    const int total = Width * Hight * 6;
    int size = (int)PlaneIndices1.size();


    WORD* PlaneIndices = new WORD[total];

    int planeIndCount = PlaneIndices1.size();

    for (int i = 0; i < planeIndCount; i++)
    {
        PlaneIndices[i] = PlaneIndices1[i];
    }

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * planeIndCount;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PlaneIndices;
    d3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneIndexBuffer);

    //store in struct
    PlaneMesh.VertexBuffer = _pPlaneVertexBuffer;
    PlaneMesh.IndexBuffer = _pPlaneIndexBuffer;
    PlaneMesh.IndexCount = planeIndCount;
    PlaneMesh.VBOffset = 0;
    PlaneMesh.VBStride = sizeof(VertexTangent);

   

    return PlaneMesh;
}

MeshData ObjectBuilder::Cube(ID3D11Device* d3dDevice)
{
    ID3D11Buffer* _pCubeVertexBuffer;
    ID3D11Buffer* _pCubeIndexBuffer;

    D3D11_BUFFER_DESC bd;
    D3D11_SUBRESOURCE_DATA InitData;
    // Create vertex buffer
    VertexTangent cubevertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 1.0f, -1.0f),XMFLOAT2(0,0),XMFLOAT3(0.0f, 0.0f, -1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(1.0f, 1.0f, -1.0f),XMFLOAT2(1,0),  XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),   XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, -1.0f, -1.0f) },

        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, -1.0f, -1.0f) },

        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f),XMFLOAT2(0,0),XMFLOAT3(1.0f, -1.0f, -1.0f) },

        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f),XMFLOAT2(1,0),XMFLOAT3(0.0f, -1.0f, -1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f),XMFLOAT2(0,1),XMFLOAT3(-1.0f, 0.0f, -1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT2(1,1),XMFLOAT3(1.0f, 0.0f, -1.0f) },

        //top
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 1.0f, -1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(1.0f, 1.0f, -1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, 1.0f, 0.0f) },

        //bot
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f),XMFLOAT2(0,0),XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT2(1,0),XMFLOAT3(0.0f, -1.0f, 0.0f) },

        //left
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 1.0f, -1.0f),XMFLOAT2(1,0),XMFLOAT3(-1.0f, 0.0f, 0.0f) },
         { XMFLOAT3(-1.0f, -1.0f, -1.0f),   XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT2(1,1),XMFLOAT3(-1.0f, 0.0f, 0.0f) },
         //right
         { XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT2(0,1),XMFLOAT3(1.0f, 0.0f, 0.0f) },
         { XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(1.0f, 1.0f, -1.0f),XMFLOAT2(0,0),XMFLOAT3(1.0f, 0.0f, 0.0f) },
         { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f),XMFLOAT2(1,0),XMFLOAT3(1.0f, 0.0f, 0.0f) },
    };
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VertexTangent) * (sizeof(cubevertices) / sizeof(cubevertices[0]));
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = cubevertices;

    d3dDevice->CreateBuffer(&bd, &InitData, &_pCubeVertexBuffer);

    WORD cubeIndices[] =
    {
        0, 1, 2,    // front
        2, 1, 3,

        4, 12, 13,    // left
        13, 6, 4,

        5,4,6,// back
        6,7,5,

        14, 15, 7,    // right
        7, 15, 16,

        4, 5, 8,    // top
        8, 5, 9,

        10, 2, 3,    // bot
        3, 11, 10,

    };
   int cubeIndCount = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * cubeIndCount;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;


    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = cubeIndices;
   d3dDevice->CreateBuffer(&bd, &InitData, &_pCubeIndexBuffer);

   //store data
   MeshData CubeMesh;
   CubeMesh.VertexBuffer= _pCubeVertexBuffer;
   CubeMesh.IndexBuffer= _pCubeIndexBuffer;
   CubeMesh.IndexCount= cubeIndCount;
   CubeMesh.VBOffset = 0;
   CubeMesh.VBStride = sizeof(VertexTangent);

 
 

    return CubeMesh;
}

MeshData ObjectBuilder::Pyramid(ID3D11Device* d3dDevice)
{
    ID3D11Buffer* _pPyrmidVertexBuffer;
    ID3D11Buffer* _pPyrmidIndexBuffer;

    D3D11_BUFFER_DESC bd;
    D3D11_SUBRESOURCE_DATA InitData;

    VertexTangent Pyramidvertices[] =
    {
        //bot
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f),XMFLOAT2(0,0),XMFLOAT3(0.0f, 0.0f, 0.0f) },
         { XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT2(1,0),XMFLOAT3(-1.0f, -1.0f, 0.0f) },
         { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
         { XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },

         //top
        { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT2(0.5,0),XMFLOAT3(0.0f, 0.0f, 0.0f) },

        //back
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },

        //front
        { XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
        //right
        { XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT3(1.0f, -1.0f, 1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
         { XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT3(1.0f, -1.0f, -1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },

         //left
         { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT2(1,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
         { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f),XMFLOAT2(0,1),XMFLOAT3(0.0f, 0.0f, 0.0f) },
    };

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 13;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = Pyramidvertices;

   d3dDevice->CreateBuffer(&bd, &InitData, &_pPyrmidVertexBuffer);


   WORD pyrimidIndices[] =
   {
       0, 2, 1,    // bot
       1, 2, 3,

       5, 6, 4,    // front

       9, 10, 4,    // right

       7, 8, 4,    // back

       11, 12, 4,    // left




   };
    int  pyrmidIndCount = sizeof(pyrimidIndices) / sizeof(pyrimidIndices[0]);

   ZeroMemory(&bd, sizeof(bd));
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof(WORD) * pyrmidIndCount;
   bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
   bd.CPUAccessFlags = 0;


   ZeroMemory(&InitData, sizeof(InitData));
   InitData.pSysMem = pyrimidIndices;
   d3dDevice->CreateBuffer(&bd, &InitData, &_pPyrmidIndexBuffer);

   MeshData PyramidMesh;

   PyramidMesh.VertexBuffer = _pPyrmidVertexBuffer;
   PyramidMesh.IndexBuffer = _pPyrmidIndexBuffer;
   PyramidMesh.IndexCount = pyrmidIndCount;
   PyramidMesh.VBOffset = 0;
   PyramidMesh.VBStride = sizeof(VertexTangent);


 

    return PyramidMesh;
}






