#ifndef _UTILS_HLSLI_
#define _UTILS_HLSLI_

#include "Params.hlsl"

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightColor color = (LightColor)0.f;

    float3 viewLightDir = (float3)0.f;

    float diffuseRatio = 0.f;
    float specularRatio = 0.f;
    float distanceRatio = 1.f;

    if (gPassConstants.lights[lightIndex].lightType == 0)
    {
        // Directional Light
        viewLightDir = normalize(mul(float4(gPassConstants.lights[lightIndex].direction.xyz, 0.f), gObjConstants.gMatView).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
    }
    else if (gPassConstants.lights[lightIndex].lightType == 1)
    {
        // Point Light
        float3 viewLightPos = mul(float4(gPassConstants.lights[lightIndex].position.xyz, 1.f), gObjConstants.gMatView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        float dist = distance(viewPos, viewLightPos);
        if (gPassConstants.lights[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else
            distanceRatio = saturate(1.f - pow(dist / gPassConstants.lights[lightIndex].range, 2));
    }
    else
    {
        // Spot Light
        float3 viewLightPos = mul(float4(gPassConstants.lights[lightIndex].position.xyz, 1.f), gObjConstants.gMatView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        if (gPassConstants.lights[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else
        {
            float halfAngle = gPassConstants.lights[lightIndex].angle / 2;

            float3 viewLightVec = viewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(gPassConstants.lights[lightIndex].direction.xyz, 0.f), gObjConstants.gMatView).xyz);

            float centerDist = dot(viewLightVec, viewCenterLightDir);
            distanceRatio = saturate(1.f - centerDist / gPassConstants.lights[lightIndex].range);

            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));

            if (centerDist < 0.f || centerDist > gPassConstants.lights[lightIndex].range) // �ִ� �Ÿ��� �������
                distanceRatio = 0.f;
            else if (lightAngle > halfAngle) // �ִ� �þ߰��� �������
                distanceRatio = 0.f;
            else // �Ÿ��� ���� ������ ���⸦ ����
                distanceRatio = saturate(1.f - pow(centerDist / gPassConstants.lights[lightIndex].range, 2));
        }
    }

    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    float3 eyeDir = normalize(viewPos);
    specularRatio = saturate(dot(-eyeDir, reflectionDir));
    specularRatio = pow(specularRatio, 2);

    color.diffuse = gPassConstants.lights[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
    color.ambient = gPassConstants.lights[lightIndex].color.ambient * distanceRatio;
    color.specular = gPassConstants.lights[lightIndex].color.specular * specularRatio * distanceRatio;

    return color;
}


#endif