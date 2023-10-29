#ifndef _POINTLIGHTING_PS_HLSL_
#define _POINTLIGHTING_PS_HLSL_

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
    
    float2 uv = float2(pin.posH.x / gPassConstants.width, pin.posH.y / gPassConstants.height);
    float3 posW = gTextureMaps[POSITIONMAP_INDEX].Sample(gsamAnisotropicWrap, uv).xyz;
    float4 posV = mul(float4(posW, 1.f), gPassConstants.view);
    if(posV.z <= 0.f)
        clip(-1);
    
    float3 toEyeW = normalize(gPassConstants.eyePosW.xyz - posW);
    float distance = length(toEyeW);
    if (distance > gPassConstants.lights[matData.lightIndex].fallOffEnd)
        clip(-1);
    
    float3 normalW = gTextureMaps[NORMALMAP_INDEX].Sample(gsamAnisotropicWrap, uv).xyz;
    float4 diffuseAlbedo = gTextureMaps[DIFFUSEMAP_INDEX].Sample(gsamAnisotropicWrap, uv);
    float3 fresnelR0 = gTextureMaps[FRESNELMAP_INDEX].Sample(gsamAnisotropicWrap, uv).xyz;
    float shininess = gTextureMaps[SHININESSMAP_INDEX].Sample(gsamAnisotropicWrap, uv).x;
    
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    Material mat = { diffuseAlbedo, fresnelR0, shininess };
    LightColor directLight = ComputePointLight(gPassConstants.lights[matData.lightIndex], mat, posW, normalW, toEyeW);
    
    pout.diffuse = float4(directLight.diffuse, 0.f);
    pout.specular = float4(directLight.specular, 0.f);
    
    return pout;
}


#endif