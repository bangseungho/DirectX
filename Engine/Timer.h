#pragma once

class Timer
{
	SINGLETON(Timer)

public:
	void Init(const WindowInfo info);
	void Update();
	void Reset();
	void Start(); 
	void Stop();  

public:
	float TotalTime() const;
	float DeltaTime() const;
	float GetFps() const;

private:
	void CalculateFrameStats();

private:
	double mSecondsPerCount = 0.0;
	double mDeltaTime = -1.0;
	int64 mBaseTime = 0;
	int64 mPausedTime = 0;
	int64 mStopTime = 0;
	int64 mPrevTime = 0;
	int64 mCurrTime = 0;
	float mFps = 0.f;
	float mSpf = 0.f;
	bool mStopped = false;

	WindowInfo mInfo = {};
};

#define DELTA_TIME GET_SINGLE(Timer)->DeltaTime()
#define TOTAL_TIME GET_SINGLE(Timer)->TotalTime()