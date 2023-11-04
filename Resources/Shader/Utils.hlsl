#ifndef _UTILS_HLSL_
#define _UTILS_HLSL_

#include "Params.hlsl"

float CalcAttenuation(float d, float FallOffStart, float FallOffEnd)
{
    return saturate((FallOffEnd-d) / (FallOffEnd - FallOffStart));
}

float3 SchlickFresnel(float3 R0, float3 Normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(Normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

    return reflectPercent;
}

LightColor BlinnPhong(float3 lightStrength, float3 lightVec, float3 Normal, float3 toEye, Material mat)
{
    LightColor color = (LightColor)0.f;
    
    const float m = mat.shininess * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f)*pow(max(dot(halfVec, Normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(mat.fresnelR0, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor*roughnessFactor;

    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    color.diffuse =  lightStrength;
    color.specular = specAlbedo * lightStrength;
    
    return color;
}

LightColor ComputeDirectionalLight(LightInfo L, Material mat, float3 Normal, float3 toEye)
{
    // �� ������ �ݴ� ���� ����
    float3 lightVec = -L.Direction;

    // ���� ���� ��ֿ� ������Ʈ �ڻ��� ��Ģ�� ����
    float ndotl = max(dot(lightVec, Normal), 0.0f);
    
    // ndotl�� 0~1�̹Ƿ� ������ ���������� ���� ���Ⱑ ��������.
    float3 lightStrength = L.Strength * ndotl;

    return BlinnPhong(lightStrength, lightVec, Normal, toEye, mat);
}

LightColor ComputePointLight(LightInfo L, Material mat, float3 Pos, float3 Normal, float3 toEye)
{
    // ������ ���������� ����
    float3 lightVec = L.Position - Pos;

    // ������ ���������� �Ÿ�
    float d = length(lightVec);

    // ������ ���������� �Ÿ��� �������� ũ�� 0����
    if(d > L.FallOffEnd)
        return (LightColor)0.f;

    // ������ ���������� ���� ����ȭ
    lightVec /= d;

    // ������ ���������� ���Ϳ� ���� ��ֿ� ������Ʈ �ڻ��� ��Ģ�� ����
    float ndotl = max(dot(lightVec, Normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

    // ���� ���� �Լ��� �Ÿ��� ���� ��������.
    float att = CalcAttenuation(d, L.FallOffStart, L.FallOffEnd);
    lightStrength *= att;

    return BlinnPhong(lightStrength, lightVec, Normal, toEye, mat);
}

LightColor ComputeSpotLight(LightInfo L, Material mat, float3 Pos, float3 Normal, float3 toEye)
{
    // ������ ���������� ����
    float3 lightVec = L.Position - Pos;

    // ������ ���������� �Ÿ�
    float d = length(lightVec);

    // ������ ���������� �Ÿ��� �������� ũ�� 0����
    if(d > L.FallOffEnd)
        return (LightColor)0.f;

    // ������ ���������� ���� ����ȭ
    lightVec /= d;

    // ������ ���������� ���Ϳ� ���� ��ֿ� ������Ʈ �ڻ��� ��Ģ�� ����
    float ndotl = max(dot(lightVec, Normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(d, L.FallOffStart, L.FallOffEnd);
    lightStrength *= att;

    // Scale by spotlight
    float spotFactor = pow(max(dot(-lightVec, L.Direction), 0.0f), L.SpotPower);
    lightStrength *= spotFactor;

    return BlinnPhong(lightStrength, lightVec, Normal, toEye, mat);
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

LightColor ComputeLighting(Material mat, float3 Pos, float3 Normal, float3 toEye, float3 shadowFactor)
{
    LightColor result = (LightColor)0.f;
    
    for(int i = 0; i < gPassConstants.lightCount; ++i)
    {
        if(gPassConstants.lights[i].LightType == 0)
        {
            LightColor color = ComputeDirectionalLight(gPassConstants.lights[i], mat, Normal, toEye);
            result.diffuse += shadowFactor * color.diffuse;
            result.specular += shadowFactor * color.specular;
        }
        if(gPassConstants.lights[i].LightType == 1)
        {
            LightColor color = ComputePointLight(gPassConstants.lights[i], mat, Pos, Normal, toEye);
            result.diffuse += color.diffuse;
            result.specular += color.specular;
        }
        if(gPassConstants.lights[i].LightType == 2)
        {
            LightColor color = ComputeSpotLight(gPassConstants.lights[i], mat, Pos, Normal, toEye);
            result.diffuse += color.diffuse;
            result.specular += color.specular;
        }
    }

    return result;
}

float Rand(float2 co)
{
    return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}
#endif