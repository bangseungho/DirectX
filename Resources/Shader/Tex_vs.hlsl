#ifndef _TEX_VS_HLSL_
#define _TEX_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    float4 posW = mul(float4(vin.posL, 1.f), gObjConstants.world);
    vout.posH = mul(posW, gObjConstants.viewProj);
    vout.uv = vin.uv;
    
    return vout;
}

#endif