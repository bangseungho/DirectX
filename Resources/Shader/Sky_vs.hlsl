#ifndef _SKY_HLSL_
#define _SKY_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posL : POSITION;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    vout.posL = vin.posL;

    float4 posW = mul(float4(vout.posL, 1.f), gObjConstants.world);
    
    posW.xyz += gPassConstants.eyePosW;
    
    vout.posH = mul(posW, gObjConstants.viewProj).xyww;
 
    return vout;
}

#endif