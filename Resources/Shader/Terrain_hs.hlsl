#ifndef _TERRAIN_HS_HLSL_
#define _TERRAIN_HS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

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

    float minDistance = gTerrainData[0].MinMaxDistance.x;
    float maxDistance = gTerrainData[0].MinMaxDistance.y;
    
    float3 edge0 = (input[1].pos + input[2].pos) / 2.f;
    float3 edge1 = (input[2].pos + input[0].pos) / 2.f;
    float3 edge2 = (input[0].pos + input[1].pos) / 2.f;
    
    edge0 = mul(float4(edge0, 1.f), gObjConstants.world).xyz;
    edge1 = mul(float4(edge1, 1.f), gObjConstants.world).xyz;
    edge2 = mul(float4(edge2, 1.f), gObjConstants.world).xyz;
    
    float edge0TessLevel = CalculateTessLevel(gPassConstants.eyePosW.xyz, edge0, minDistance, maxDistance, 4.f);
    float edge1TessLevel = CalculateTessLevel(gPassConstants.eyePosW.xyz, edge1, minDistance, maxDistance, 4.f);
    float edge2TessLevel = CalculateTessLevel(gPassConstants.eyePosW.xyz, edge2, minDistance, maxDistance, 4.f);
    
    output.edgeTess[0] = edge0TessLevel;
    output.edgeTess[1] = edge1TessLevel;
    output.edgeTess[2] = edge2TessLevel;
    output.insideTess = edge2TessLevel;

    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
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