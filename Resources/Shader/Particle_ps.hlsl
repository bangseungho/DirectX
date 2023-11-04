#ifndef _PARTICLE_PS_HLSL_
#define _PARTICLE_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct GS_OUT
{
    float4 posH : SV_Position;
    float2 uv : TEXCOORD;
    uint id : SV_InstanceID;
};

float4 PS_Main(GS_OUT pin) : SV_Target
{
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    float3 fresnelR0 = matData.fresnelR0;
    float roughness = matData.roughness;
    uint diffuseMapIndex = matData.textureMapIndex;
    uint normalMapIndex = matData.normalMapIndex;
    uint roughnessMapIndex = matData.roughnessMapIndex;
    
    diffuseAlbedo = gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.uv) * diffuseAlbedo;

    return diffuseAlbedo;
}

#endif