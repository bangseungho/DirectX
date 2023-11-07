#ifndef _TESSELLATION_HS_HLSL_
#define _TESSELLATION_HS_HLSL_

#include "Params.hlsl"

struct VS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

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

PatchTess ConstantHS(InputPatch<VS_OUT, 3> input, int patchID : SV_PrimitiveID)
{
    PatchTess output = (PatchTess)0.f;

    output.edgeTess[0] = 3;
    output.edgeTess[1] = 3;
    output.edgeTess[2] = 3;
    output.insideTess = 3;

    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)] 
[patchconstantfunc("ConstantHS")]
HS_OUT HS_Main(InputPatch<VS_OUT, 3> input, int vertexIdx : SV_OutputControlPointID, int patchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT)0.f;

    output.pos = input[vertexIdx].pos;
    output.uv = input[vertexIdx].uv;

    return output;
}

#endif