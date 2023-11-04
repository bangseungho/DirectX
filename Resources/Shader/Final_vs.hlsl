#ifndef _FINAL_VS_HLSL_
#define _FINAL_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 Pos : Position;
    float2 Uv : TEXCOORD;
};

struct VS_OUT
{
    float4 Pos : SV_Position;
    float2 Uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    vout.Pos = float4(vin.Pos * 2.f, 1.f);
    vout.Uv = vin.Uv;
    
    return vout;
}

#endif