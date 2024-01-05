#ifndef _TERRAIN_PS_HLSL_
#define _TERRAIN_PS_HLSL_

#include "Utils.hlsl"
#include "Params.hlsl"

struct DS_OUT
{
    float4 posH : SV_Position;
    float3 posW : Position;
    float3 normalW : Normal;
	float3 tangentW : Tangent;
    float2 Uv : TEXCOORD;
    float4 ProjTex : TEXCOORD1;
};

struct PS_OUT
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;
    float4 diffuseAlbedo : SV_Target2;
    float4 fresnelR0 : SV_Target3;
    float shininess : SV_Target4;
};

PS_OUT PS_Main(DS_OUT pin)
{
    // 재질 정보 색인화
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    float3 fresnelR0 = matData.fresnelR0;
    float roughness = matData.roughness;
    uint diffuseMapIndex = matData.textureMapIndex;
    uint normalMapIndex = matData.normalMapIndex;
    uint roughnessMapIndex = matData.roughnessMapIndex;
    uint heightMapIndex = matData.heightMapIndex;
    
    // 보간 과정에서 단위 벡터가 안될 수 있으므로 노말라이즈를 한다.
    pin.normalW = normalize(pin.normalW);
    
    pin.ProjTex.xyz /= pin.ProjTex.w;
    pin.ProjTex.x = pin.ProjTex.x * 0.5f + 0.5f;
    pin.ProjTex.y = pin.ProjTex.y * 0.5f + 0.5f;
    float4 projTexMap = gTextureMaps[8].Sample(gsamAnisotropicBorder, pin.ProjTex.xy);
    
    // 베이스 컬러
    if (diffuseMapIndex != -1)
        diffuseAlbedo = gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.Uv) * diffuseAlbedo * projTexMap;
    
    clip(diffuseAlbedo.a - 0.1f);
    
    // 노멀 맵
    float4 normalMap = float4(pin.normalW, 0.f);
    if (normalMapIndex != -1)
        normalMap = gTextureMaps[normalMapIndex].Sample(gsamAnisotropicWrap, pin.Uv);
    
    float3 bumpedNormalW = NormalToWorldSpace(normalMap.rgb, pin.normalW, pin.tangentW);
    
    // 거칠기 
    if (roughness != -1)
        roughness *= gTextureMaps[roughnessMapIndex].Sample(gsamAnisotropicWrap, pin.Uv).x;
    
    // 광택 : 거칠수록 광택이 떨어짐
    float shininess = 1.f - roughness;
    
    // 노멀 매핑을 안한다면 일반 노말로
    if (normalMapIndex == -1)
        bumpedNormalW = pin.normalW;
    else
        shininess *= normalMap.a;

    PS_OUT pout = (PS_OUT)0;
    pout.Position = float4(pin.posW, 0.f);
    pout.Normal = float4(bumpedNormalW, 0.f);
    pout.diffuseAlbedo = diffuseAlbedo;
    pout.fresnelR0 = float4(fresnelR0, 0.f);
    pout.shininess = shininess;
    
    return pout;
}

#endif