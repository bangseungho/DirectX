#ifndef _PARTICLE_VS_HLSL_
#define _PARTICLE_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : Position;
    float2 uv : TEXCOORD;
    uint id : SV_InstanceID;
};

struct VS_OUT
{
    float3 posW : Position;
    float2 uv : TEXCOORD;
    float id : ID;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    vout.posW = posW.xyz;
    vout.posW += gOutputParticle[vin.id].WorldPos;
    vout.id = vin.id;
    
    return vout;
}

#endif