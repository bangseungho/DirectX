#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    // 세계 공간으로 변환
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    vout.posW = posW.xyz;
    
    // 비균등 비례가 없을 경우 법선 변환 있다면 역전치 행렬을 사용
    vout.normalW = mul(vin.normalL, (float3x3)gObjConstants.world);
    
    // 동차 절단 공간으로 변환
    vout.posH = mul(posW, gPassConstants.viewProj);

    float4 uv = mul(float4(vin.uv, 0.f, 1.f), gMaterialConstants.matTransform);
    vout.uv = uv.xy;
 
    return vout;
}

float4 PS_Main(VS_OUT pin) : SV_Target
{
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.uv) * gMaterialConstants.diffuseAlbedo;
    
    clip(diffuseAlbedo.a - 0.1f);
    
    return diffuseAlbedo;
}

#endif