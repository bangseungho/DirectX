
struct ObjectConstants
{
    float4 offset0;
    float4 offset1;
};

struct MaterialConstants
{
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float roughness;
    float4x4 matTransform;
    float texOn;
};

ConstantBuffer<ObjectConstants> gObjConstants : register(b0);
ConstantBuffer<MaterialConstants> gMaterialConstants : register(b1);

Texture2D gDiffuseMap : register(t0);
Texture2D gDiffuseMap2 : register(t1);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

struct VS_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.f);
    output.pos += gObjConstants.offset0;
    
    output.color = input.color;
    output.color += gObjConstants.offset1;
    
    output.uv = input.uv;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = gDiffuseMap.Sample(gsamAnisotropicWrap, input.uv);
    
    return color;
}