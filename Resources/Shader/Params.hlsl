#ifndef _PARAMS_HLSL_
#define _PARAMS_HLSL_
#define MaxLights 50

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
	LightInfo	        lights[50];
};

struct ObjectConstants
{
     row_major matrix world;
};

struct MaterialConstants
{
    float4              diffuseAlbedo;
    float3              fresnelR0;
    float               roughness;
    row_major float4x4  matTransform;
};

ConstantBuffer<PassConstants> gPassConstants : register(b0);
ConstantBuffer<ObjectConstants> gObjConstants : register(b1);
ConstantBuffer<MaterialConstants> gMaterialConstants : register(b2);

Texture2D gDiffuseMap : register(t0);
Texture2D gDiffuseMap2 : register(t1);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

#endif