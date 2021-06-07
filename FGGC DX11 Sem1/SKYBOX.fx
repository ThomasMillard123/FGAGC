//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t3);

SamplerState samLinear : register(s0);
TextureCube gCubeMap : register(t0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
    matrix World;
    matrix View;
    matrix Projection;
    float gTime;
    float3 LightVecW;
    float4 DiffuseMtrl;
    float4 DiffuseLight;
    float4 AmbientMaterial;
    float4 AmbientLight;

    float4 SpecularMtrl;
    float4 SpecularLight;
    float SpecularPower;
    float3 EyePosW;
    int NRMEnabled;
    int SpecularEnabled;

}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXTCOORD0;
    float3 Tangent : TANGENT;


};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD0;
   

};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

   //set it so it wil always disyply on far plane
    output.Pos = mul(float4(input.Pos,1.0f), WorldViewProjection).xyww;
   
    output.Tex = input.Pos;

   

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    //texture at pos
   float4 textureColour1 = gCubeMap.Sample(samLinear, input.Tex);

   

  

    return  textureColour1;





}
