#ifndef _TEX_VS_HLSL_
#define _TEX_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : Position;
    float2 Uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    float4 posW = mul(float4(vin.posL, 1.f), gObjConstants.world);
    vout.posH = mul(posW, gObjConstants.ViewProj);
    vout.Uv = vin.Uv;
    
    return vout;
}

#endif