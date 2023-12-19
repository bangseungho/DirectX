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

float4 PS_Main(VS_OUT pin) : SV_Target
{
	float4 pout = gCubeMap.Sample(gsamLinearWrap, pin.posL);
    
    float3 toEyeW = gPassConstants.eyePosW.xyz - pin.posFog;
    float distToEye = length(toEyeW);
    
    float range = 0;
    if(gTerrainData[0].Width >= gTerrainData[0].Height)
        range = gTerrainData[0].Width / 2.f;
    else 
        range = gTerrainData[0].Height / 2.f;
    
    float fogAmount = clamp(((distToEye - gPassConstants.FogStart) / (range - 800)), 0.f, 0.9f);
    
    //pout = lerp(pout, gPassConstants.FogColor, fogAmount); 
    
    return pout;
}

#endif