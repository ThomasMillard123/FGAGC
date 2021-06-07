//--------------------------------------------------------------------------------------
// File: NoLighting.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txSpec : register(t1);
Texture2D txNRM : register(t2);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
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
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXTCOORD0;
    float3 Tangent : TANGENT;
    
  
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    
    
};
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;


    output.Pos = mul(input.Pos, World);

    output.Pos = mul(output.Pos, View);

    output.Pos = mul(output.Pos, Projection);


    output.Tex = input.Tex;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    //texture
   float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);
   clip(textureColour.a - 0.25f);
   //final color
   float4 color;
   color.rgb = textureColour.rgb;

   return  color;





}

