#ifndef _TERRAIN_DS_HLSL_
#define _TERRAIN_DS_HLSL_

#include "Params.hlsl"

struct PatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

struct HS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct DS_OUT
{
    float4 posH : SV_Position;
    float3 posW : Position;
    float3 normalW : Normal;
	float3 tangentW : Tangent;
    float2 Uv : TEXCOORD;
    float4 ProjTex : TEXCOORD1;
};

[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, 3> input, float3 location : SV_DomainLocation, PatchTess patch)
{
    MaterialData matData = gMaterialData[gObjConstants.materialIndex];
    float4 diffuseAlbedo = matData.diffuseAlbedo;
    float3 fresnelR0 = matData.fresnelR0;
    float roughness = matData.roughness;
    uint diffuseMapIndex = matData.textureMapIndex;
    uint normalMapIndex = matData.normalMapIndex;
    uint roughnessMapIndex = matData.roughnessMapIndex;
    uint heightMapIndex = matData.heightMapIndex;
    
    DS_OUT output = (DS_OUT)0.f;

    float3 localPos = input[0].pos * location[0] + input[1].pos * location[1] + input[2].pos * location[2];
    float2 uv = input[0].uv * location[0] + input[1].uv * location[1] + input[2].uv * location[2];
    
    int tileCountX = gTerrainData[0].SizeX;
    int tileCountZ = gTerrainData[0].SizeZ;
    int mapWidth = gTerrainData[0].Resolution.x;
    int mapHeight = gTerrainData[0].Resolution.y;
    
    float2 fullUV = float2(uv.x / (float)tileCountX, uv.y / (float)tileCountZ);
    float height = gTextureMaps[heightMapIndex].SampleLevel(gsamLinearWrap, fullUV, 0).x;

    localPos.y = height;
    
    float2 deltaUV = float2(1.f / mapWidth, 1.f / mapHeight);
    float2 deltaPos = float2(tileCountX * deltaUV.x, tileCountZ * deltaUV.y);
    
    float upHeight = gTextureMaps[heightMapIndex].SampleLevel(gsamLinearWrap, float2(fullUV.x, fullUV.y - deltaUV.y), 0).x;
    float downHeight = gTextureMaps[heightMapIndex].SampleLevel(gsamLinearWrap, float2(fullUV.x, fullUV.y + deltaUV.y), 0).x;
    float rightHeight = gTextureMaps[heightMapIndex].SampleLevel(gsamLinearWrap, float2(fullUV.x + deltaUV.x, fullUV.y), 0).x;
    float leftHeight = gTextureMaps[heightMapIndex].SampleLevel(gsamLinearWrap, float2(fullUV.x - deltaUV.x, fullUV.y), 0).x;
    
    float3 localTangent = float3(localPos.x + deltaPos.x, rightHeight, localPos.z) - float3(localPos.x - deltaPos.x, leftHeight, localPos.z);
    float3 localBinormal = float3(localPos.x, upHeight, localPos.z + deltaPos.y) - float3(localPos.x, downHeight, localPos.z - deltaPos.y);

    output.posW = mul(float4(localPos, 1.f), gObjConstants.world).xyz;
    output.posH = mul(float4(output.posW, 1.f), gObjConstants.ViewProj);
    output.ProjTex = mul(float4(output.posW, 1.f), gPassConstants.WorldViewProjTexture);
    
    output.tangentW = normalize(mul(float4(localTangent, 0.f), gObjConstants.world)).xyz;
    float3 binormalW = normalize(mul(float4(localBinormal, 0.f), gObjConstants.world)).xyz;
    output.normalW = normalize(cross(binormalW, output.tangentW));

    output.Uv = uv;

    return output;
}

#endif