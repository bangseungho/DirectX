#ifndef _PARAMS_HLSL_
#define _PARAMS_HLSL_

struct LightColor
{
    float4      diffuse;
    float4      ambient;
    float4      specular;
};

struct LightInfo
{
    LightColor  color;
    float4	    position;
    float4	    direction; 
    int		    lightType;
    float	    range;
    float	    angle;
    int  	    padding;
};

struct PassConstants
{
    int		    lightCount;
	float3	    padding;
	LightInfo	lights[50];
};

struct ObjectConstants
{
     row_major matrix gMatWorld;
	 row_major matrix gMatView;
	 row_major matrix gMatProjection;
	 row_major matrix gMatWV;
	 row_major matrix gMatWVP;
};

struct MaterialConstants
{
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float roughness;
    float4x4 matTransform;
    float texOn;
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