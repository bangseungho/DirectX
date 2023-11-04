#ifndef _PARTICLE_VS_HLSL_
#define _PARTICLE_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : Position;
    float2 Uv : TEXCOORD;
    uint id : SV_InstanceID;
};

struct VS_OUT
{
    float3 posW : Position;
    float2 Uv : TEXCOORD;
    float id : ID;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    vout.posW = posW.xyz;
    
    float4 Uv = mul(float4(vin.Uv, 0.f, 1.f), gObjConstants.texTransform);
    vout.Uv = mul(Uv, matData.matTransform).xy;
 
    return vout;
}

#endif