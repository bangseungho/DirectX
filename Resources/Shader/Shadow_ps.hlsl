#ifndef _SHADOW_PS_HLSL
#define _SHADOW_PS_HLSL

#include "Params.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float4 clipPos : POSITION;
};

float4 PS_Main(VS_OUT vout) : SV_Target
{
    float res = vout.clipPos.z / vout.clipPos.w;
    return float4(res, 0.f, 0.f, 0.f);
}
#endif