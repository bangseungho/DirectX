#ifndef _UTILS_HLSLI_
#define _UTILS_HLSLI_

#include "Params.hlsl"

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return saturate((falloffEnd-d) / (falloffEnd - falloffStart));
}

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
    const float m = mat.shininess * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f)*pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(mat.fresnelR0, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor*roughnessFactor;

    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (mat.diffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(LightInfo L, Material mat, float3 normal, float3 toEye)
{
    // �� ������ �ݴ� ���� ����
    float3 lightVec = -L.direction;

    // ���� ���� ��ֿ� ������Ʈ �ڻ��� ��Ģ�� ����
    float ndotl = max(dot(lightVec, normal), 0.0f);
    
    // ndotl�� 0~1�̹Ƿ� ������ ���������� ���� ���Ⱑ ��������.
    float3 lightStrength = L.strength * ndotl;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 ComputePointLight(LightInfo L, Material mat, float3 pos, float3 normal, float3 toEye)
{
    // ������ ���������� ����
    float3 lightVec = L.position - pos;

    // ������ ���������� �Ÿ�
    float d = length(lightVec);

    // ������ ���������� �Ÿ��� �������� ũ�� 0����
    if(d > L.fallOffEnd)
        return 0.0f;

    // ������ ���������� ���� ����ȭ
    lightVec /= d;

    // ������ ���������� ���Ϳ� ���� ��ֿ� ������Ʈ �ڻ��� ��Ģ�� ����
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.strength * ndotl;

    // ���� ���� �Լ��� �Ÿ��� ���� ��������.
    float att = CalcAttenuation(d, L.fallOffStart, L.fallOffEnd);
    lightStrength *= att;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 ComputeSpotLight(LightInfo L, Material mat, float3 pos, float3 normal, float3 toEye)
{
    // ������ ���������� ����
    float3 lightVec = L.position - pos;

    // ������ ���������� �Ÿ�
    float d = length(lightVec);

    // ������ ���������� �Ÿ��� �������� ũ�� 0����
    if(d > L.fallOffEnd)
        return 0.0f;

    // ������ ���������� ���� ����ȭ
    lightVec /= d;

    // ������ ���������� ���Ϳ� ���� ��ֿ� ������Ʈ �ڻ��� ��Ģ�� ����
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.strength * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(d, L.fallOffStart, L.fallOffEnd);
    lightStrength *= att;

    // Scale by spotlight
    float spotFactor = pow(max(dot(-lightVec, L.direction), 0.0f), L.spotPower);
    lightStrength *= spotFactor;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 NormalToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// [0, 1]���� [-1, 1]�� ���
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	// ���� ���� ������ ����
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

    // ���������� ���� �������� ��ȯ
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

float4 ComputeLighting(Material mat, float3 pos, float3 normal, float3 toEye, float3 shadowFactor)
{
    float3 result = 0.0f;
    
    for(int i = 0; i < gPassConstants.lightCount; ++i)
    {
        if (gPassConstants.lights[i].lightType == 0)
            result += shadowFactor[i] * ComputeDirectionalLight(gPassConstants.lights[i], mat, normal, toEye);
        if (gPassConstants.lights[i].lightType == 1)
            result += ComputePointLight(gPassConstants.lights[i], mat, pos, normal, toEye);
        if (gPassConstants.lights[i].lightType == 2)
            result += ComputeSpotLight(gPassConstants.lights[i], mat, pos, normal, toEye);
    }

    return float4(result, 0.0f);
}

#endif