#ifndef _PARTICLE_PS_HLSL_
#define _PARTICLE_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct GS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
    uint id : SV_InstanceID;
};

float4 PS_Main(GS_OUT pin) : SV_Target
{
    return gTextureMaps[gMaterialData[gObjConstants.materialIndex].textureMapIndex].Sample(gsamAnisotropicWrap, pin.Uv);
}

#endif