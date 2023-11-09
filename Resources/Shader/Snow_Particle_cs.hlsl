#ifndef _SNOW_PARTICLE_CS_HLSL_
#define _SNOW_PARTICLE_CS_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

[numthreads(512, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    ParticleSystemData particleSystem = gParticleSystemData[0];
    gInputParticle[threadIndex.x].StartEndScale = float2(particleSystem.StartScale, particleSystem.EndScale);

    if(threadIndex.x >= particleSystem.MaxCount)
        return;
    
    gSharedData[0].AddCount = particleSystem.AddCount; // AddCount�� ���� �ڿ���
    GroupMemoryBarrierWithGroupSync();
    
    if(gInputParticle[threadIndex.x].Alive == 0)
    {
        while(true)
        {
            int remaining = gSharedData[0].AddCount;
            if(remaining <= 0) // ��ƼŬ ���� ������ �������� �ʴٸ� ������Ʈ ���� �ʴ´�.
                break;
            
            int expected = remaining;
            int desired = remaining - 1;
            int originalValue;
            // AddCount�� expected ���� ���ٸ� desired�� AddCount�� �־��ش�.
            // ������ AddCount ����� originalValue�� �־��ش�.
            InterlockedCompareExchange(gSharedData[0].AddCount, expected, desired, originalValue);
            
            if(expected == originalValue)
            {
                gInputParticle[threadIndex.x].Alive = 1;
                break;
            }
        }
        
        if(gInputParticle[threadIndex.x].Alive == 1)
        {
            float x = ((float)threadIndex.x / (float)particleSystem.MaxCount) + particleSystem.AccTime;
            float r1 = Rand(float2(x, particleSystem.AccTime));
            float r2 = Rand(float2(x * particleSystem.AccTime, particleSystem.AccTime));
            float r3 = Rand(float2(x * particleSystem.AccTime * particleSystem.AccTime, particleSystem.AccTime * particleSystem.AccTime));
            
            float3 noise =
            {
                2 * r1 - 1,
                2 * r2 - 1,
                2 * r3 - 1
            };
            
            float3 dir = (noise - 0.5f) * 2.f;
            
            gInputParticle[threadIndex.x].WorldDir = float3(0.f, -1.f, 0.f);
            gInputParticle[threadIndex.x].WorldPos = (noise.xyz - 0.5f) * 25;
            gInputParticle[threadIndex.x].LifeTime = ((particleSystem.MaxLifeTime - particleSystem.MinLifeTime) * noise.x) + particleSystem.MinLifeTime;
            gInputParticle[threadIndex.x].CurTime = 0.f;
        }
    }
    else
    {
        gInputParticle[threadIndex.x].CurTime += particleSystem.DeltaTime;
        if(gInputParticle[threadIndex.x].LifeTime < gInputParticle[threadIndex.x].CurTime)
        {
            gInputParticle[threadIndex.x].Alive = 0;
            return;
        }
        
        float ratio = gInputParticle[threadIndex.x].CurTime / gInputParticle[threadIndex.x].LifeTime;
        float speed = (particleSystem.MaxSpeed - particleSystem.MinSpeed) * ratio + particleSystem.MinSpeed;
        gInputParticle[threadIndex.x].WorldPos += gInputParticle[threadIndex.x].WorldDir * speed * particleSystem.DeltaTime;
    }
}

#endif