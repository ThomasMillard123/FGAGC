#include "JsonFilePars.h"

bool JsonFilePars::LoadFile(std::string fileName, std::vector<ObjectData>& objectData, std::vector< JsonFilePars::CameraData>& cameraData, std::vector<LightInfo>& lightData)
{
    std::string out;
    json j;
    std::ifstream t(fileName);
    //put file into json
    t >> j;

    //pullout data
    std::string v = j["version"].get<std::string>();

        //gets node
        json Menu = j["Objects"];
        //find the size of nodes (number of nodes)
        int size = Menu.size();
        ObjectData obj;
        for ( int i = 0; i < size; i++) {
            
            json jasonObject = Menu.at(i); 
            obj.FileName = jasonObject["File"];
            obj.Pos.x = jasonObject["PosX"];
            obj.Pos.y = jasonObject["PosY"];
            obj.Pos.z = jasonObject["PosZ"];
            obj.Rotation.x = jasonObject["RotationX"];
            obj.Rotation.y = jasonObject["RotationY"];
           obj.Rotation.z = jasonObject["RotationZ"];
           obj.Scale.x = jasonObject["SizeX"];
           obj.Scale.y = jasonObject["SizeY"];
           obj.Scale.z = jasonObject["SizeZ"];
           objectData.push_back(obj);
            
        }
 
        //gets node
         Menu = j["Camera"];
        //fineds the size of nodes (number of nodes)
         size = Menu.size();
         CameraData Cam;
        for ( int i = 0; i < size; i++) {
            //loopes throug the nodes in the node
            json jasonObject = Menu.at(i);
            Cam.Pos.x = jasonObject["EyeX"];
            Cam.Pos.y = jasonObject["EyeY"];
            Cam.Pos.z = jasonObject["EyeZ"];
            Cam.At.x = jasonObject["AtX"];
            Cam.At.y = jasonObject["AtY"];
            Cam.At.z = jasonObject["AtZ"];
            Cam.Up.x = jasonObject["UpX"];
            Cam.Up.y = jasonObject["UpY"];
            Cam.Up.z = jasonObject["UpZ"];
            Cam.Type = jasonObject["Type"];
            cameraData.push_back(Cam);
        }
    
        //gets node
        Menu = j["Lights"];
        //fineds the size of nodes (number of nodes)
        size = Menu.size();
        LightInfo Light;
        for ( int i = 0; i < size; i++) {
            //loopes throug the nodes in the node
            json jasonObject = Menu.at(i);
            Light.Pos.x = jasonObject["PosX"];
            Light.Pos.y = jasonObject["PosY"];
            Light.Pos.z = jasonObject["PosZ"];
            Light.DiffuseLight.x = jasonObject["DiffuseLightX"];
            Light.DiffuseLight.y = jasonObject["DiffuseLightY"];
            Light.DiffuseLight.z = jasonObject["DiffuseLightZ"];
            Light.AmbientLight.x = jasonObject["AmbientLightX"];
            Light.AmbientLight.y = jasonObject["AmbientLightY"];
            Light.AmbientLight.z = jasonObject["AmbientLightZ"];
            Light.SpecularLight.x = jasonObject["SpecularLightX"];
            Light.SpecularLight.y = jasonObject["SpecularLightY"];
            Light.SpecularLight.z = jasonObject["SpecularLightZ"];
            lightData.push_back(Light);
        }


    //if data is null return false
        if (objectData.size() == 0 || cameraData.size() == 0 || lightData.size() == 0) {
            return false;
        }
    return true;
}
