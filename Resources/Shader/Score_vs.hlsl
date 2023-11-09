#ifndef _SCORE_VS_HLSL_
#define _SCORE_VS_HLSL_

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
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    float4 posW = mul(float4(vin.posL, 1.f), gObjConstants.world);
    vout.posH = mul(posW, gObjConstants.ViewProj);
    
    float4 Uv = mul(float4(vin.Uv, 0.f, 1.f), gObjConstants.texTransform);
    vout.Uv = mul(Uv, matData.matTransform).xy;
    
    float2 scoreIndex = matData.ScoreIndex;
    
    vout.Uv.x += 0.2f * scoreIndex.x;
    vout.Uv.y += 0.5f * scoreIndex.y;
    
    return vout;
}

#endif