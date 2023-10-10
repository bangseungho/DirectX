#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT)0.f;
    
    // ���� �������� ��ȯ
    float4 posW = mul(float4(vin.posL, 1.0f), gObjConstants.world);
    vout.posW = posW.xyz;
    
    // ��յ� ��ʰ� ���� ��� ���� ��ȯ �ִٸ� ����ġ ����� ���
    vout.normalW = mul(vin.normalL, (float3x3)gObjConstants.world);
    
    // ���� ���� �������� ��ȯ
    vout.posH = mul(posW, gPassConstants.viewProj);

    float4 uv = mul(float4(vin.uv, 0.f, 1.f), gMaterialConstants.matTransform);
    vout.uv = uv.xy;
 
    return vout;
}

float4 PS_Main(VS_OUT pin) : SV_Target
{
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.uv) * gMaterialConstants.diffuseAlbedo;
    
    // ���� �������� ���� ���Ͱ� �ȵ� �� �����Ƿ� �븻����� �Ѵ�.
    pin.normalW = normalize(pin.normalW);

    // ����Ǵ� ������ �������� ����
	float3 toEyeW = gPassConstants.eyePosW.xyz - pin.posW;
	float distToEye = length(toEyeW);
	toEyeW /= distToEye; // �븻������

    // �ֺ���
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    // ���� : ��ĥ���� ������ ������
    const float shininess = 1.0f - gMaterialConstants.roughness;
    
    // ������ ���� ���� ��Ƽ����
    float3 shadowFactor = 1.0f;
    Material mat = { diffuseAlbedo, gMaterialConstants.fresnelR0, shininess };
    float4 directLight = ComputeLighting(gPassConstants.lights, mat, pin.posW, pin.normalW, toEyeW, shadowFactor);
    
    float4 resColor = ambient + directLight;
    
    // �л� �������� ���ĸ� �����´�.
    resColor.a = diffuseAlbedo.a;
    
    return resColor;
}

#endif