#ifndef _SKY_HLSL_
#define _SKY_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 Uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posL : Position;
    float3 posFog : FogPosition;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    vout.posL = vin.posL;

    float4 posW = mul(float4(vout.posL, 1.f), gObjConstants.world);
    
    float scale = 0;
    if(gTerrainData[0].Width >= gTerrainData[0].Height)
        scale = gTerrainData[0].Width;
    else 
        scale = gTerrainData[0].Height;
    
    Matrix matScale =
    {
        scale, 0.f,    0.f,    0.f,
        0.f,    scale, 0.f,    0.f,
        0.f,    0.f,    scale, 0.f,
        0.f,    0.f,    0.f,    1.f,
    };
    
    vout.posFog = mul(posW, matScale);
    
    posW.xyz += gPassConstants.eyePosW;
    
    vout.posH = mul(posW, gObjConstants.ViewProj).xyww;
    
    return vout;
}

#endif