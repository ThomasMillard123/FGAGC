#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

class Model
{
public:

	int positionX, positionY, positionZ;
	int rotation;

	Model();
	~Model();

	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	void Update();
	void Draw();

private:



};

