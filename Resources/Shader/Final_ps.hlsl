#ifndef _FINAL_PS_HLSL_
#define _FINAL_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 uv : TEXCOORD;
};

float4 PS_Main(VS_OUT pin) : SV_Target
{
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    uint diffuseMapIndex = matData.textureMapIndex;

    return gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.uv) * diffuseAlbedo;
}

#endif