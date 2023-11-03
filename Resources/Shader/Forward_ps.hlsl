#ifndef _FORWARD_PS_HLSL_
#define _FORWARD_PS_HLSL_

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

float4 PS_Main(VS_OUT pin) : SV_Target
{
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    float3 fresnelR0 = matData.fresnelR0;
    float roughness = matData.roughness;
    uint diffuseMapIndex = matData.textureMapIndex;
    uint normalMapIndex = matData.normalMapIndex;
    uint roughnessMapIndex = matData.roughnessMapIndex;
    
    // 보간 과정에서 단위 벡터가 안될 수 있으므로 노말라이즈를 한다.
    pin.normalW = normalize(pin.normalW);
    
    // 베이스 컬러
    diffuseAlbedo = gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.uv) * diffuseAlbedo;
    
    // 노멀 맵
    float4 normalMap = gTextureMaps[normalMapIndex].Sample(gsamAnisotropicWrap, pin.uv);
    float3 bumpedNormalW = NormalToWorldSpace(normalMap.rgb, pin.normalW, pin.tangentW);
    
    // 거칠기 
    roughness *= gTextureMaps[roughnessMapIndex].Sample(gsamAnisotropicWrap, pin.uv).x;
    
    // 조명되는 점에서 눈으로의 벡터
    float3 toEyeW = normalize(gPassConstants.eyePosW.xyz - pin.posW);
    
    // 주변광
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    // 광택 : 거칠수록 광택이 떨어짐
    float shininess = 1.f - roughness;
    
    // 노멀 매핑을 안한다면 일반 노말로
    if (normalMapIndex == -1)
        bumpedNormalW = pin.normalW;
    else
        shininess *= normalMap.a;
    
    // 조명을 입힐 최종 머티리얼
    float3 shadowFactor = 1.0f;
    Material mat = { diffuseAlbedo, fresnelR0, shininess };
    
    // 조명 계산
    LightColor directLight = ComputeLighting(mat, pin.posW, bumpedNormalW, toEyeW, shadowFactor);
    
    float4 resColor = ambient + (float4(directLight.diffuse, 0.f) + float4(directLight.specular, 0.f));
    
    // 분산 재질에서 알파를 가져온다.
    resColor.a = diffuseAlbedo.a;
    
    return diffuseAlbedo;
}

#endif