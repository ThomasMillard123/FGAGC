#pragma once
#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

#include"Structures.h"
using namespace DirectX;
using namespace std;


/// <summary>
/// class to hold apperance data of an object
/// </summary>
class Appearance
{
public:

	Appearance(Material material, Geometry geometry);
	~Appearance();

	//sets
	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _TextureRV = textureRV; }
	
	
	//gets
	Geometry GetGeometryData() const { return _Geometry; }
	Material GetMaterial() const { return _Material; }
	ID3D11ShaderResourceView* GetTextureRV() const { return _TextureRV; }

	bool HasTexture() const { return _TextureRV ? true : false; }


	void Update(float t);
	void Draw(ID3D11DeviceContext* pImmediateContext);


private:
	//data
	Geometry _Geometry;
	Material _Material;
	ID3D11ShaderResourceView* _TextureRV;


	void CleanUp();
};

