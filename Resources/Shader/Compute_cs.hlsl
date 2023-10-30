#ifndef _COMPUTE_CS_HLSL_
#define _COMPUTE_CS_HLSL_

#include "Params.hlsl"

RWTexture2D<float4> gRwTex0 : register(u0);

// max : 1024 
[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.y % 2 == 0)
        gRwTex0[threadIndex.xy] = float4(1.f, 0.f, 0.f, 1.f);
    else 
        gRwTex0[threadIndex.xy] = float4(0.f, 1.f, 0.f, 1.f);
    
}

#endif