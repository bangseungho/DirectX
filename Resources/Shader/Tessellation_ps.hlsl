#ifndef _TESSELLATION_PS_HLSL_
#define _TESSELLATION_PS_HLSL_

#include "Params.hlsl"

struct DS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 PS_Main(DS_OUT input) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}

#endif