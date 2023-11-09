#ifndef _SCORE_PS_HLSL_
#define _SCORE_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
};

float4 PS_Main(VS_OUT pin) : SV_Target
{
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    uint diffuseMapIndex = matData.textureMapIndex;

    diffuseAlbedo *= gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.Uv);
    
    clip(diffuseAlbedo.a - 0.1);
    
    return  diffuseAlbedo;
}

#endif