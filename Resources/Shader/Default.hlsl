#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangentU : TANGENT;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    // 세계 공간으로 변환
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    vout.posW = posW.xyz;
    
    // 비균등 비례가 없을 경우 법선 변환 있다면 역전치 행렬을 사용
    vout.normalW = mul(vin.normalL, (float3x3)gObjConstants.world);
    
    vout.tangentW = mul(vin.tangentU, (float3x3)gObjConstants.world);

    // 동차 절단 공간으로 변환
    vout.posH = mul(posW, gPassConstants.viewProj);

    float4 uv = mul(float4(vin.uv, 0.f, 1.f), matData.matTransform);
    vout.uv = uv.xy;
 
    return vout;
}

float4 PS_Main(VS_OUT pin) : SV_Target
{
    // 보간 과정에서 단위 벡터가 안될 수 있으므로 노말라이즈를 한다.
    pin.normalW = normalize(pin.normalW);
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];

    // 노멀 맵
    //float4 normalMap = gNormalMap.Sample(gsamAnisotropicWrap, pin.uv);
    //float3 bumpedNormalW = NormalToWorldSpace(normalMap.rgb, pin.normalW, pin.tangentW);
    
    // 거칠기 
    //float roughness = gRoughnessMap.Sample(gsamAnisotropicWrap, pin.uv).x;

    
    // 베이스 컬러
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.uv) * matData.diffuseAlbedo;
    clip(diffuseAlbedo.a - 0.1f);
    
    // 조명되는 점에서 눈으로의 벡터
    float3 toEyeW = normalize(gPassConstants.eyePosW.xyz - pin.posW);
    
    // 주변광
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    // 광택 : 거칠수록 광택이 떨어짐
    //float shininess = 1.0f - roughness;
    float shininess = 1.f - matData.roughness;
    
    //if (gMaterialConstants.normalMapping == 1.f)
    //    shininess *= normalMap.a;
    //else 
	   // bumpedNormalW = pin.normalW;
    
    // 조명을 입힐 최종 머티리얼
    float3 shadowFactor = 1.0f;
    Material mat = { diffuseAlbedo, matData.fresnelR0, shininess };
    
    // 조명 계산
    float4 directLight = ComputeLighting(mat, pin.posW, pin.normalW, toEyeW, shadowFactor);
    
    float4 resColor = ambient + directLight;
    
    // 분산 재질에서 알파를 가져온다.
    resColor.a = diffuseAlbedo.a;
    
    return resColor;
}

#endif