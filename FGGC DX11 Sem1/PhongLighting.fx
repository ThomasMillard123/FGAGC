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
    float3 PosW : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Tangent : TANGENT;

};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;


    output.Pos = mul(input.Pos, World);

    // Compute the vector from the vertex to the eye position.
    output.PosW = EyePosW - input.Pos.xyz;

    output.Pos = mul(output.Pos, View);

    output.Pos = mul(output.Pos, Projection);

    // Convert from local space to world space 
    // W component of vector is 0 as vectors cannot be translated
    output.Normal = mul(float4(input.Normal, 0.0f), World).xyz;

    output.Tex = input.Tex;

    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.Tangent = normalize(mul(input.Tangent, World));

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

   //lighting
   float3 toEye = normalize(input.PosW);
   float3 normalW = normalize(input.Normal);

    int NormalMap = NRMEnabled;
    if (NormalMap == 1) {

        float4 bumpMap = txNRM.Sample(samLinear, input.Tex);
        //bump mamping
         // Expand the range of the normal value from (0, +1) to (-1, +1).
        bumpMap = (bumpMap * 2.0f) - 1.0f;


        float3 biTangent = normalize(cross(input.Normal, input.Tangent));

        // Calculate the normal from the data in the bump map.
        float3x3 texSpace = float3x3(input.Tangent, biTangent, input.Normal);

        // Normalize the resulting bump normal.
        float3 BumpNorm = normalize(mul(bumpMap, texSpace));
        normalW.xyz = BumpNorm.xyz;
    }

    //phong shading

    //find the relection vector
    float3 r = reflect(-LightVecW, normalW);
    //how much specular light that will enter the eye
    float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);

    //ambient and diffuse light caualation
    float3 ambient = AmbientMaterial * AmbientLight;
    float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
    float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight * textureColour).rgb;

    //specular calualtion
    float3 specular;
    int SpecMapEnabled = SpecularEnabled;
    if (SpecMapEnabled == 1) {
        float4 textureSpec = txSpec.Sample(samLinear, input.Tex);
        specular = specularAmount * (textureSpec * SpecularMtrl * SpecularLight).rgb;
    }
    else {
        specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
    }

    //final color
    float4 color;
    color.rgb = ambient + diffuse + specular;
    color.a = DiffuseMtrl.a;

    return  color;





}
