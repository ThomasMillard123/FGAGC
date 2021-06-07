#pragma once


#include <iostream>
#include<string>
#include <fstream>
#include<vector>

#include"Transform.h"
#include"Appearance.h"
#include "Structures.h"

#include "Shader_VS_PS.h"
using namespace std;
/// <summary>
/// class to hold trrain data 
/// it holds the hight map,textures and object data
/// </summary>
class Terrain
{
	//funct
public:
	Terrain(string hightMapFile, Transform* transfrom, ID3D11Device* d3dDevice, ID3D11DeviceContext* pImmediateContext);
	~Terrain();


	void Draw(ID3D11DeviceContext* pImmediateContext);
	
	//get/sets
	void SetHightMapFile(string File) { HightMapFile = File; }
	string GetHightMapFile() const { return HightMapFile; }

	Transform* GetTransform() const { return _Transfrom; }

	Appearance* GetAppreance() const { return _Appreance; }
	Shader_VS_PS* GetTerrainShgader() { return TerrainShader; }
	vector<ID3D11ShaderResourceView*> GetTextureList() { return _pGroundTextureRV; }


private:

	void LoadHightMap();
	void CreatTerrain(ID3D11Device* d3dDevice);
	void CleanUp();
	//vars
private:
	string HightMapFile;
	
	vector<float> _HightmapData;


	int _HeightmapWidth, _HeightmapHeight;
	float _HeightScale;

	Transform* _Transfrom;
	Appearance*_Appreance;
	vector<ID3D11ShaderResourceView*> _pGroundTextureRV;

	Shader_VS_PS* TerrainShader;
};

