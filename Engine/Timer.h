#pragma once

class Timer
{
	DECLARE_SINGLE(Timer)

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
	double mSecondsPerCount;
	double mDeltaTime;
	int64 mBaseTime;
	int64 mPausedTime;
	int64 mStopTime;
	int64 mPrevTime;
	int64 mCurrTime;
	float mFps;
	float mSpf;
	bool mStopped;

	WindowInfo mInfo;
};

