#ifndef _SHADOW_VS_HLSL
#define _SHADOW_VS_HLSL

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float4 clipPos : POSITION;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    float4 posW = mul(float4(vin.posL, 1.f), gObjConstants.world);
    vout.posH = mul(posW, gPassConstants.WorldViewProjTexture);
    vout.clipPos = vout.posH;
    
    return vout;
}

#endif