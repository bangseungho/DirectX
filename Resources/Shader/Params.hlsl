#ifndef _PARAMS_HLSL_
#define _PARAMS_HLSL_
#define MAX_LIGHTS 200
#define TEXTURE2D_COUNT 11

#define POSITIONMAP_INDEX 0
#define NORMALMAP_INDEX 1
#define DIFFUSEMAP_INDEX 2
#define FRESNELMAP_INDEX 3
#define SHININESSMAP_INDEX 4
#define DIFFUSELIGHT_INDEX 5
#define SPECULARLIGHT_INDEX 6

struct LightInfo
{
    float3      Strength;
    float       FallOffStart; 
    float3      Direction;    
    float       FallOffEnd;   
    float3      Position;     
    float       SpotPower;    
    int         LightType;
    float3      Padding;
};

struct Material
{
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float shininess;
};

struct LightColor
{
    float3 diffuse;
    float3 specular;
};

struct PassConstants
{
    row_major matrix    View;
    row_major matrix    Proj;
    row_major matrix    ViewProj;
    float4              eyePosW;
	float               nearZ;
	float               farZ;
    float               Width;
    float               Height;
	float               totalTime;
	float               deltaTime;
	float2              padding2;
	float4              ambientLight;
    int		            lightCount;
	float3	            padding3;
	LightInfo	        lights[MAX_LIGHTS];
    float3              CameraRight;
    float               padding4;
};

struct ObjectData
{
    row_major matrix    world;
    row_major matrix    ViewProj;
    row_major matrix    texTransform;
    uint                materialIndex;
    float3              Padding;
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
    int                 lightIndex;
};

struct Particle
{
    float3  WorldPos;
    float   CurTime;
    float3  WorldDir;
    float   LifeTime; 
    int     Alive;
    float3  Padding;
};

ConstantBuffer<PassConstants> gPassConstants : register(b0);
ConstantBuffer<ObjectData> gObjConstants : register(b1);
StructuredBuffer<MaterialData> gMaterialData : register(t0, space1);

//StructuredBuffer<Particle> gInputParticleData : register(t9, space1);
//RWStructuredBuffer<Particle> gOutputParticleData : register(u0);

TextureCube gCubeMap : register(t0);
Texture2D gTextureMaps[TEXTURE2D_COUNT] : register(t1);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

#endif