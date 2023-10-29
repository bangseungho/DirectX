#ifndef _DIRLIGHTING_VS_HLSL_
#define _DIRLIGHTING_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    vout.pos = float4(vin.pos * 2.f, 1.f);
    vout.uv = vin.uv;
    
    return vout;
}

#endif