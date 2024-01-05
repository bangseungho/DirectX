#ifndef _PARAMS_HLSL_
#define _PARAMS_HLSL_
#define MAX_LIGHTS 50
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
    
    float               FogStart;
	float               FogRange;
    float4              FogColor;
    
	float4              ambientLight;
    
    int		            lightCount;
	float3	            padding3;
    
	LightInfo	        lights[MAX_LIGHTS];
    
    float3              CameraRight;
    float               padding4;
    
	row_major Matrix	WorldViewProjTexture;
};

struct ObjectData
{
    row_major matrix    world;
    row_major matrix    ViewProj;
    row_major matrix    texTransform;
    uint                materialIndex;
    int                 lightIndex;
    float2              Padding;
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
    uint                heightMapIndex;
    float2	            ScoreIndex;
	float2	            Padding;
};

struct TerrainData
{
    uint	SizeX;
	uint	SizeZ;
    float   Width;
    float   Height;
	float	MaxTessellationLevel;
	float	Padding1;
	float2	Resolution;
	float2	MinMaxDistance;
	float2	Padding2;
};

struct Particle
{
    float3  WorldPos;
    float   CurTime;
    float3  WorldDir;
    float   LifeTime; 
    int     Alive;
    float2  StartEndScale;
    float   Padding;
};

struct ParticleSystemData
{
    float   DeltaTime;
    uint    AddCount;
	float   AccTime;
	uint    MaxCount;
	float   MinLifeTime;
	float   MaxLifeTime;
	float   MinSpeed;
	float   MaxSpeed;
    float   StartScale;
    float   EndScale;
    float2  Padding;
};

struct ComputeShared
{
    int     AddCount;
    float3  Padding;
};

Texture2D gTextureMaps[TEXTURE2D_COUNT] : register(t0, space0);
TextureCube gCubeMap : register(t0, space1);
StructuredBuffer<MaterialData> gMaterialData : register(t1, space1);
StructuredBuffer<Particle> gOutputParticle : register(t2, space1);
StructuredBuffer<TerrainData> gTerrainData : register(t3, space1);
StructuredBuffer<ParticleSystemData> gParticleSystemData : register(t4, space1);

ConstantBuffer<PassConstants> gPassConstants : register(b0);
ConstantBuffer<ObjectData> gObjConstants : register(b1);

RWStructuredBuffer<ComputeShared> gSharedData : register(u0, space0);
RWStructuredBuffer<Particle> gInputParticle : register(u1, space0);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerState gsamAnisotropicBorder : register(s6);

#endif