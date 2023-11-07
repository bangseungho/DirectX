#ifndef _TESSELLATION_DS_HLSL_
#define _TESSELLATION_DS_HLSL_

#include "Params.hlsl"

struct PatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

struct HS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct DS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, 3> input, float3 location : SV_DomainLocation, PatchTess patch)
{
    DS_OUT output = (DS_OUT)0.f;

    float3 localPos = input[0].pos * location[0] + input[1].pos * location[1] + input[2].pos * location[2];
    float2 uv = input[0].uv * location[0] + input[1].uv * location[1] + input[2].uv * location[2];

    float4x4 matWVP = mul(gObjConstants.world, gPassConstants.ViewProj);
    
    output.pos = mul(float4(localPos, 1.f), matWVP);
    output.uv = uv;

    return output;
}

#endif