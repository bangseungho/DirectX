#ifndef _LIGHTING_PS_HLSL_
#define _LIGHTING_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 uv : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

PS_OUT PS_Main(VS_OUT pin)
{
    PS_OUT pout = (PS_OUT)0;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    float3 posW = gTextureMaps[0].Sample(gsamAnisotropicWrap, pin.uv).xyz;
    if (posW.z <= 0.f)
        clip(-1);
    
    float3 normalW = gTextureMaps[1].Sample(gsamAnisotropicWrap, pin.uv).xyz;
    float4 diffuseAlbedo = gTextureMaps[2].Sample(gsamAnisotropicWrap, pin.uv);
    float3 fresnelR0 = gTextureMaps[3].Sample(gsamAnisotropicWrap, pin.uv).xyz;
    float shininess = gTextureMaps[4].Sample(gsamAnisotropicWrap, pin.uv).x;
    
    float3 toEyeW = normalize(gPassConstants.eyePosW.xyz - posW);
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    float3 shadowFactor = 1.0f;
    Material mat = { diffuseAlbedo, fresnelR0, shininess };
    float4 directLight = ComputeLighting(mat, posW, normalW, toEyeW, shadowFactor);
    
    pout.diffuse = ambient;
    pout.specular = directLight;
    
    return pout;
}


#endif