#pragma once

#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <vector>		//For storing the XMFLOAT3/2 variables


#include"Structs.h"
using namespace DirectX;

namespace ObjectBuilder {
	

	MeshData HorsontalPlane(ID3D11Device* d3dDevice,int width,int hight);
	MeshData VerticalPlane(ID3D11Device* d3dDevice, int width, int hight);

	MeshData Cube(ID3D11Device* d3dDevice);
	MeshData Pyramid(ID3D11Device* d3dDevice);
	
}

