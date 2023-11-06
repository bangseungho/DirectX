#ifndef _PARTICLE_GS_HLSL_
#define _PARTICLE_GS_HLSL_

#include "Params.hlsl"

struct VS_OUT
{
    float3 posW : Position;
    float2 Uv : TEXCOORD;
    float id : ID;
};

struct GS_OUT
{
    float4 posH : SV_Position;
    float2 Uv : TEXCOORD;
    uint id : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Main(point VS_OUT gin[1], inout TriangleStream<GS_OUT> triStream)
{
    GS_OUT gout[4] =
    {
        (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f
    };
    
    uint id = (uint)gin[0].id;
    if(0 == gOutputParticle[id].Alive)
        return;
    
    float ratio = gOutputParticle[id].CurTime / gOutputParticle[id].LifeTime;
    float scale = ((gOutputParticle[id].StartEndScale.y - gOutputParticle[id].StartEndScale.x) *
                    ratio + gOutputParticle[id].StartEndScale.x) / 2.f;

    //float3 up = float3(0.f, 1.f, 0.f);
    float3 look = gPassConstants.eyePosW.xyz - gin[0].posW;
    look = normalize(look);
    float3 right = -gPassConstants.CameraRight;
    float3 up = cross(right, look);
    //float3 right = cross(up, look);    
    
    float halfWidth = scale;
    float halfHeight = scale;
    
    float4 posW[4];
    posW[0] = float4(gin[0].posW + halfWidth * right - halfHeight * up, 1.f);
    posW[1] = float4(gin[0].posW + halfWidth * right + halfHeight * up, 1.f);
    posW[2] = float4(gin[0].posW - halfWidth * right - halfHeight * up, 1.f);
    posW[3] = float4(gin[0].posW - halfWidth * right + halfHeight * up, 1.f);
    
    gout[0].posH = mul(posW[0], gPassConstants.ViewProj);
    gout[1].posH = mul(posW[1], gPassConstants.ViewProj);
    gout[2].posH = mul(posW[2], gPassConstants.ViewProj);
    gout[3].posH = mul(posW[3], gPassConstants.ViewProj);
    
    gout[0].Uv = float2(0.f, 1.f);
    gout[1].Uv = float2(0.f, 0.f);
    gout[2].Uv = float2(1.f, 1.f);
    gout[3].Uv = float2(1.f, 0.f);
    
    gout[0].id = gin[0].id;
    gout[1].id = gin[0].id;
    gout[2].id = gin[0].id;
    gout[3].id = gin[0].id;
    
    triStream.Append(gout[0]);
    triStream.Append(gout[1]);
    triStream.Append(gout[2]);
    
    triStream.Append(gout[1]);
    triStream.Append(gout[2]);
    triStream.Append(gout[3]);
}

#endif