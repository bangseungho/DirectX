#ifndef _DIRLIGHTING_PS_HLSL_
#define _DIRLIGHTING_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
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
    
    float3 posW = gTextureMaps[POSITIONMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float4 posV = mul(float4(posW, 1.f), gPassConstants.View);
    if(posV.z <= 0.f)
        clip(-1);

    float3 normalW = gTextureMaps[NORMALMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float4 diffuseAlbedo = gTextureMaps[DIFFUSEMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv);
    float3 fresnelR0 = gTextureMaps[FRESNELMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float shininess = gTextureMaps[SHININESSMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).x;
    
    float3 toEyeW = gPassConstants.eyePosW.xyz - posW;
    float distToEye = length(toEyeW);
    toEyeW /= distToEye;
    
    
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
 
    Material mat = { diffuseAlbedo, fresnelR0, shininess };
    LightColor directLight = ComputeDirectionalLight(gPassConstants.lights[gObjConstants.lightIndex], mat, normalW, toEyeW);
    
    pout.diffuse = float4(directLight.diffuse, 0.f);
    pout.specular = float4(directLight.specular, 0.f);
    
    return pout;
}


#endif