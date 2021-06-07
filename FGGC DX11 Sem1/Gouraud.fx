//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
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
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXTCOORD0;
    float3 Tangent : TANGENT;


};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR0;
    float2 Tex : TEXCOORD0;
  

};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.Pos = mul(input.Pos, World);

    // Compute the vector from the vertex to the eye position.
    float3 toEye = normalize(EyePosW - input.Pos.xyz);
    
    output.Pos = mul(output.Pos, View);

    output.Pos = mul(output.Pos, Projection);

    // Convert from local space to world space 
    // W component of vector is 0 as vectors cannot be translated
    float3 normalW = mul(float4(input.Normal, 0.0f), World).xyz;
     normalW = normalize(normalW);
  
    float3 r = reflect(-LightVecW, normalW);
        
    float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
    
        float3 ambient = AmbientMaterial * AmbientLight;
        float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
        float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
 
        float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
      
        output.Color.rgb = ambient + diffuse+specular;
        output.Color.a = DiffuseMtrl.a;

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

   
    return input.Color+ textureColour;





}
