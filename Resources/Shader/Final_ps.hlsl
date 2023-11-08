#ifndef _FINAL_PS_HLSL_
#define _FINAL_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
};

float4 PS_Main(VS_OUT pin) : SV_Target
{
    float4 pout = (float4)0;

    float4 diffuseAlbedo = gTextureMaps[DIFFUSEMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv);
    float4 diffuseLight = gTextureMaps[DIFFUSELIGHT_INDEX].Sample(gsamAnisotropicWrap, pin.Uv);
    float4 specular = gTextureMaps[SPECULARLIGHT_INDEX].Sample(gsamAnisotropicWrap, pin.Uv);
    
    float3 posW = gTextureMaps[POSITIONMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float3 toEyeW = gPassConstants.eyePosW.xyz - posW;
    float distToEye = length(toEyeW);
    toEyeW /= distToEye;
    
    pout = (diffuseLight + gPassConstants.ambientLight) * diffuseAlbedo + specular;

    
    // Fog
    float fogAmount = clamp(((distToEye - gPassConstants.FogStart) / gPassConstants.FogRange), 0.f, 0.9f);
    pout = lerp(pout, gPassConstants.FogColor, fogAmount); 
    
    return pout;
}

#endif