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

struct PS_OUT
{
    float4  position : SV_Target0;
    //float4 normal : SV_Target1;
    //float4 diffuseAlbedo : SV_Target2;
    //float4 fresnelR0 : SV_Target3;
    //float shininess : SV_Target4;
};

PS_OUT PS_Main(VS_OUT pin)
{
    // 재질 정보 색인화
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
    
    // 광택 : 거칠수록 광택이 떨어짐
    float shininess = 1.f - roughness;
    
    // 노멀 매핑을 안한다면 일반 노말로
    if (normalMapIndex == -1)
        bumpedNormalW = pin.normalW;
    else
        shininess *= normalMap.a;

    PS_OUT pout = (PS_OUT)0;
    pout.position = float4(pin.posW, 0.f);
    //pout.normal = float4(bumpedNormalW, 0.f);
    //pout.diffuseAlbedo = diffuseAlbedo;
    //pout.shininess = shininess;
    //pout.fresnelR0 = float4(fresnelR0, 0.f);
    
    return pout;
}

#endif