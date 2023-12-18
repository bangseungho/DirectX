#ifndef _FORWARD_PS_HLSL_
#define _FORWARD_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

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
    float2 Uv : TEXCOORD;
};

struct PS_OUT
{
    float4  Position : SV_Target0;
    float4 Normal : SV_Target1;
    float4 diffuseAlbedo : SV_Target2;
    float4 fresnelR0 : SV_Target3;
    float shininess : SV_Target4;
};

PS_OUT PS_Main(VS_OUT pin)
{
    // ���� ���� ����ȭ
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    float3 fresnelR0 = matData.fresnelR0;
    float roughness = matData.roughness;
    uint diffuseMapIndex = matData.textureMapIndex;
    uint normalMapIndex = matData.normalMapIndex;
    uint roughnessMapIndex = matData.roughnessMapIndex;
    uint isCubeMapping = matData.IsCubeMapping;
    
    // ���� �������� ���� ���Ͱ� �ȵ� �� �����Ƿ� �븻����� �Ѵ�.
    pin.normalW = normalize(pin.normalW);
    
    // ���̽� �÷�
    if (diffuseMapIndex != -1)
        diffuseAlbedo = gTextureMaps[diffuseMapIndex].Sample(gsamAnisotropicWrap, pin.Uv) * diffuseAlbedo;
    
    clip(diffuseAlbedo.a - 0.1f);
    
    // ��� ��
    float4 normalMap = float4(pin.normalW, 0.f);
    if (normalMapIndex != -1)
        normalMap = gTextureMaps[normalMapIndex].Sample(gsamAnisotropicWrap, pin.Uv);
    
    float3 bumpedNormalW = NormalToWorldSpace(normalMap.rgb, pin.normalW, pin.tangentW);
    
    // ��ĥ�� 
    if (roughness != -1)
        roughness *= gTextureMaps[roughnessMapIndex].Sample(gsamAnisotropicWrap, pin.Uv).x;
    
    // ���� : ��ĥ���� ������ ������
    float shininess = 1.f - roughness;
    
    // ��� ������ ���Ѵٸ� �Ϲ� �븻��
    if (normalMapIndex == -1)
        bumpedNormalW = pin.normalW;
    else
        shininess *= normalMap.a;

    // CubeMapping
    if(isCubeMapping == 1)
    {
        float3 toEyeW = gPassConstants.eyePosW.xyz - pin.posW;
        float distToEye = length(toEyeW);
        toEyeW /= distToEye;
        
        float3 r = reflect(-toEyeW, bumpedNormalW);
        float4 reflectionColor = gCubeMap.Sample(gsamLinearWrap, r);
        float3 fresnelFactor = SchlickFresnel(fresnelR0, bumpedNormalW, r);
        diffuseAlbedo.rgb += fresnelFactor * reflectionColor.rgb;
    }
    
    PS_OUT pout = (PS_OUT)0;
    pout.Position = float4(pin.posW, 0.f);
    pout.Normal = float4(bumpedNormalW, 0.f);
    pout.diffuseAlbedo = diffuseAlbedo;
    pout.fresnelR0 = float4(fresnelR0, 0.f);
    pout.shininess = shininess;
    
    return pout;
}

#endif