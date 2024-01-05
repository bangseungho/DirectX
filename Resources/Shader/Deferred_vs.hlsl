#ifndef _FORWARD_VS_HLSL_
#define _FORWARD_VS_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 Uv : TEXCOORD;
    float3 tangentU : Tangent;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posW : Position;
    float3 normalW : Normal;
	float3 tangentW : Tangent;
    float2 Uv : TEXCOORD0;
    float4 ProjTex : TEXCOORD1;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    
    // ���� �������� ��ȯ
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    vout.posW = posW.xyz;
    
    // ��յ� ��ʰ� ���� ��� ���� ��ȯ �ִٸ� ����ġ ����� ���
    vout.normalW = mul(vin.normalL, (float3x3)gObjConstants.world);
    
    vout.tangentW = mul(vin.tangentU, (float3x3)gObjConstants.world);

    // ���� ���� �������� ��ȯ
    vout.posH = mul(posW, gObjConstants.ViewProj);
    vout.ProjTex = mul(posW, gPassConstants.WorldViewProjTexture);
    
    float4 Uv = mul(float4(vin.Uv, 0.f, 1.f), matData.matTransform);
    vout.Uv = Uv.xy;
    
    
    return vout;
}

#endif