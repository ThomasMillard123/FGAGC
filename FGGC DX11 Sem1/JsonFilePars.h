#pragma once
#include<iostream>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
namespace JsonFilePars
{
	struct Vector3D
	{
		float x;
		float y;
		float z;
	};

	struct ObjectData
	{
		std::string FileName;
		Vector3D Pos;
		Vector3D Rotation;
		Vector3D Scale;

	}; 

	struct CameraData
	{
		Vector3D Pos;
		Vector3D At;
		Vector3D Up;
		std::string Type;
	};

	struct LightInfo
	{
		Vector3D Pos;
		Vector3D DiffuseLight;
		Vector3D AmbientLight;
		Vector3D SpecularLight;

	};
	
	bool LoadFile(std::string fileName, std::vector<ObjectData> &objectData, std::vector<JsonFilePars::CameraData> &cameraData, std::vector<LightInfo>&lightData);
	

};

