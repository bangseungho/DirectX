#ifndef _PARTICLE_CS_HLSL_
#define _PARTICLE_CS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

//StructuredBuffer<Particle> gData : register(t0, space3);

// Require
// DeltaTime / AccTime
// Particle Max Count
// AddCount
// MinLifeTime / MaxLifeTime / MinSpeed / MaxSpeed
[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    //ParticleSystemData particleSystem = gParticleSystemData[0];
    
    //gOutputParticleData[threadIndex.x].WorldPos.x += 1;
    
    //if(threadIndex.x >= particleSystem.MaxCount)
    //    return;
    
    //gShared[0].AddCount = particle.AddCount; // AddCount는 공유 자원임
    //GroupMemoryBarrierWithGroupSync();
    
    //if (gParticle[threadIndex.x].alive == 0)
    //{
    //    while (true)
    //    {
    //        int remaining = gShared[0].AddCount;
    //        if (remaining <= 0) // 파티클 생성 개수가 남아있지 않다면 업데이트 하지 않는다.
    //            break;
            
    //        int expected = remaining;
    //        int desired = remaining - 1;
    //        int originalValue;
    //        // AddCount와 expected 값이 같다면 desired를 AddCount에 넣어준다.
    //        // 생성된 AddCount 결과를 originalValue에 넣어준다.
    //        InterlockedCompareExchange(gShared[0].AddCount, expected, desired, originalValue); 
            
    //        if(expected == originalValue)
    //        {
    //            gParticle[threadIndex.x].alive = 1;
    //            break;
    //        }
    //    }
        
    //    if (gParticle[threadIndex.x].alive == 1)
    //    {
    //        float x = ((float)threadIndex.x / (float)particle.MaxCount) + particle.AccTime;
    //        float r1 = Rand(float2(x, particle.AccTime));
    //        float r2 = Rand(float2(x * particle.AccTime, particle.AccTime));
    //        float r3 = Rand(float2(x * particle.AccTime * particle.AccTime, particle.AccTime * particle.AccTime));
            
    //        float3 noise =
    //        {
    //            2 * r1 - 1,
    //            2 * r2 - 1,
    //            2 * r3 - 1
    //        };
            
    //        float3 dir = (noise - 0.5f) * 2.f;
            
    //        gParticle[threadIndex.x].worldDir = normalize(dir);
    //        gParticle[threadIndex.x].worldPos = (noise.xyz - 0.5f) * 25;
    //        gParticle[threadIndex.x].lifeTime = ((particle.MaxLifeTime - particle.MinLifeTime) * noise.x) + particle.MinLifeTime;
    //        gParticle[threadIndex.x].curTime = 0.f;
    //    }
    //}
    //else
    //{
    //    gParticle[threadIndex.x].curTime += gPassConstants.deltaTime;
    //    if (gParticle[threadIndex.x].lifeTime < gParticle[threadIndex.x].curTime)
    //    {
    //        gParticle[threadIndex.x].alive = 0;
    //        return;
    //    }
        
    //    float ratio = gParticle[threadIndex.x].curTime / gParticle[threadIndex.x].lifeTime;
    //    float speed = (particle.MaxSpeed - particle.MinSpeed) * ratio + particle.MinSpeed;
    //    gParticle[threadIndex.x].worldPos += gParticle[threadIndex.x].worldDir * speed * gPassConstants.deltaTime;
    //}
}

#endif