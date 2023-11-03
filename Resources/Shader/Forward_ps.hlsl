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

float4 PS_Main(VS_OUT pin) : SV_Target
{
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    float3 fresnelR0 = matData.fresnelR0;
    float roughness = matData.roughness;
    uint diffuseMapIndex = matData.textureMapIndex;
    uint normalMapIndex = matData.normalMapIndex;
    uint roughnessMapIndex = matData.roughnessMapIndex;
    
    // ���� �������� ���� ���Ͱ� �ȵ� �� �����Ƿ� �븻����� �Ѵ�.
    pin.normalW = normalize(pin.normalW);
    
    // ���̽� �÷�
    diffuseAlbedo = gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.uv) * diffuseAlbedo;
    
    // ��� ��
    float4 normalMap = gTextureMaps[normalMapIndex].Sample(gsamAnisotropicWrap, pin.uv);
    float3 bumpedNormalW = NormalToWorldSpace(normalMap.rgb, pin.normalW, pin.tangentW);
    
    // ��ĥ�� 
    roughness *= gTextureMaps[roughnessMapIndex].Sample(gsamAnisotropicWrap, pin.uv).x;
    
    // ����Ǵ� ������ �������� ����
    float3 toEyeW = normalize(gPassConstants.eyePosW.xyz - pin.posW);
    
    // �ֺ���
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
    
    // ���� : ��ĥ���� ������ ������
    float shininess = 1.f - roughness;
    
    // ��� ������ ���Ѵٸ� �Ϲ� �븻��
    if (normalMapIndex == -1)
        bumpedNormalW = pin.normalW;
    else
        shininess *= normalMap.a;
    
    // ������ ���� ���� ��Ƽ����
    float3 shadowFactor = 1.0f;
    Material mat = { diffuseAlbedo, fresnelR0, shininess };
    
    // ���� ���
    LightColor directLight = ComputeLighting(mat, pin.posW, bumpedNormalW, toEyeW, shadowFactor);
    
    float4 resColor = ambient + (float4(directLight.diffuse, 0.f) + float4(directLight.specular, 0.f));
    
    // �л� �������� ���ĸ� �����´�.
    resColor.a = diffuseAlbedo.a;
    
    return diffuseAlbedo;
}

#endif