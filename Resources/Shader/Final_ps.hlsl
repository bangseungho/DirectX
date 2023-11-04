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
    
    
    pout = diffuseLight * diffuseAlbedo + specular;

    return pout;
}

#endif