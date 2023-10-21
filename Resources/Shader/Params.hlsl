#ifndef _PARAMS_HLSL_
#define _PARAMS_HLSL_
#define MAX_LIGHTS 200
#define TEXTURE2D_COUNT 8

struct LightInfo
{
    float3      strength;
    float       fallOffStart; 
    float3      direction;    
    float       fallOffEnd;   
    float3      position;     
    float       spotPower;    
    int         lightType;
    float3      padding;
};

struct Material
{
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float shininess;
};

struct PassConstants
{
    row_major matrix    view;
    row_major matrix    proj;
    row_major matrix    viewProj;
    float4              eyePosW;
	float               nearZ;
	float               farZ;
	float               totalTime;
	float               deltaTime;
	float4              ambientLight;
    int		            lightCount;
	float3	            padding;
	LightInfo	        lights[MAX_LIGHTS];
};

struct ObjectConstants
{
    row_major matrix    world;
    uint                materialIndex;
    float3              padding;
};

struct MaterialData
{
    float4              diffuseAlbedo;
    float3              fresnelR0;
    float               roughness;
    row_major float4x4  matTransform;
    uint                textureMapIndex;
    uint                normalMapIndex;
    uint                roughnessMapIndex;
    uint                padding;
};

ConstantBuffer<PassConstants> gPassConstants : register(b0);
ConstantBuffer<ObjectConstants> gObjConstants : register(b1);
StructuredBuffer<MaterialData> gMaterialData : register(t0, space1);

TextureCube gCubeMap : register(t0);
Texture2D gTextureMaps[TEXTURE2D_COUNT] : register(t1);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

#endif