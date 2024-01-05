#ifndef _DIRLIGHTING_PS_HLSL_
#define _DIRLIGHTING_PS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

PS_OUT PS_Main(VS_OUT pin)
{
    PS_OUT pout = (PS_OUT)0;
    
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    uint shadowMapIndex = matData.ShadowMapIndex;
    
    float3 posW = gTextureMaps[POSITIONMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float4 posV = mul(float4(posW, 1.f), gPassConstants.View);
    if(posV.z <= 0.f)
        clip(-1);

    float3 normalW = gTextureMaps[NORMALMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float4 diffuseAlbedo = gTextureMaps[DIFFUSEMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv);
    float3 fresnelR0 = gTextureMaps[FRESNELMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).xyz;
    float shininess = gTextureMaps[SHININESSMAP_INDEX].Sample(gsamAnisotropicWrap, pin.Uv).x;
    
    float3 toEyeW = gPassConstants.eyePosW.xyz - posW;
    float distToEye = length(toEyeW);
    toEyeW /= distToEye;
    
    float4 ambient = gPassConstants.ambientLight * diffuseAlbedo;
 
    Material mat = { diffuseAlbedo, fresnelR0, shininess };
    LightColor directLight = ComputeDirectionalLight(gPassConstants.lights[gObjConstants.lightIndex], mat, normalW, toEyeW);
    
    // Shadow
    if (length(directLight.diffuse) != 0) // 검은 영역이라 굳이 그림자가 필요 없는 부분
    {
        matrix shadowCamVP = gPassConstants.WorldViewProjTexture; // 광원 위치의 카메라 뷰 프로젝션 행렬
        
        float4 shadowClipPosH = mul(float4(posW, 1.f), shadowCamVP);
        
        float3 ndc = shadowClipPosH.xyz / shadowClipPosH.w; // 프로젝션 행렬 후 원근 투영 나누기를 통해 ndc를 구함
        float depth = ndc.z;
        float2 uv = ndc.xy;
        uv.y *= -1;                 // ndc 공간의 좌표는 y[1 ~ -1] 이기 때문에 반대로 변경
        uv = uv * 0.5f + 0.5;       // [0 ~ 1]로 매핑
        
        if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
        {
            // 텍스처에 렌더링한 쉐도우 맵을 샘플링한다.
            float shadowMapDepth = gTextureMaps[0].Sample(gsamAnisotropicWrap, uv).x;
            
            // 현재 픽셀의 깊이 값이 쉐도우 맵의 깊이 값보다 큰 경우에 그림자에 해당 이 부분은 어둡게 처리
            if (shadowMapDepth > 0 && depth > shadowMapDepth + 0.00001f /* 바이어스 */) 
            {
                directLight.diffuse *= 0.3f;
                directLight.specular = (float4)0.f;
            }
        }
    }
    
    pout.diffuse = float4(directLight.diffuse, 0.f);
    pout.specular = float4(directLight.specular, 0.f);
    
    return pout;
}


#endif