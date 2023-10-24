#ifndef _TEX_VS_HLSL_
#define _TEX_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    // 세계 공간으로 변환
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    
    // 동차 절단 공간으로 변환
    vout.posH = mul(posW, gObjConstants.viewProj);

    float4 uv = mul(float4(vin.uv, 0.f, 1.f), gObjConstants.texTransform);
    vout.uv = mul(uv, matData.matTransform).xy;
 
    return vout;
}
#endif