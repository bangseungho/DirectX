#ifndef _TERRAIN_VS_HLSL_
#define _TERRAIN_VS_HLSL_

#include "Utils.hlsl"
#include "Params.hlsl"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = input;

    return output;
}

#endif