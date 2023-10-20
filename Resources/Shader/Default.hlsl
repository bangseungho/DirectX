#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

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
    vout.posH = mul(posW, gPassConstants.viewProj);

    float4 uv = mul(float4(vin.uv, 0.f, 1.f), matData.matTransform);
    vout.uv = uv.xy;
 
    return vout;
}

float4 PS_Main(VS_OUT pin) : SV_Target
{
    // ���� �������� ���� ���Ͱ� �ȵ� �� �����Ƿ� �븻����� �Ѵ�.
    pin.normalW = normalize(pin.normalW);
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];

    // ��� ��
    //float4 normalMap = gNormalMap.Sample(gsamAnisotropicWrap, pin.uv);
    //float3 bumpedNormalW = NormalToWorldSpace(normalMap.rgb, pin.normalW, pin.tangentW);
    
    // ��ĥ�� 
    //float roughness = gRoughnessMap.Sample(gsamAnisotropicWrap, pin.uv).x;

    
    // ���̽� �÷�
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.uv) * matData.diffuseAlbedo;
    clip(diffuseAlbedo.a - 0.1f);
    
    // ����Ǵ� ������ �������� ����
    float3 toEyeW = normalize(gPassConstants.eyePosW.xyz - pin.posW);
    
    // �ֺ���
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    // ���� : ��ĥ���� ������ ������
    //float shininess = 1.0f - roughness;
    float shininess = 1.f - matData.roughness;
    
    //if (gMaterialConstants.normalMapping == 1.f)
    //    shininess *= normalMap.a;
    //else 
	   // bumpedNormalW = pin.normalW;
    
    // ������ ���� ���� ��Ƽ����
    float3 shadowFactor = 1.0f;
    Material mat = { diffuseAlbedo, matData.fresnelR0, shininess };
    
    // ���� ���
    float4 directLight = ComputeLighting(mat, pin.posW, pin.normalW, toEyeW, shadowFactor);
    
    float4 resColor = ambient + directLight;
    
    // �л� �������� ���ĸ� �����´�.
    resColor.a = diffuseAlbedo.a;
    
    return resColor;
}

#endif