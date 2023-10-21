#include "pch.h"
#include "Timer.h"

DECLARE_SINGLE(Timer)

void Timer::Init(const WindowInfo info)
{
	int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;

	mInfo = info;
	Reset();
}

void Timer::Update()
{
	if (!mStopped)
		CalculateFrameStats();

	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}

	int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
		mDeltaTime = 0.0;
}

void Timer::Reset()
{
	int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Timer::Start()
{
	int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped) {
		mPausedTime += (startTime - mStopTime);
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void Timer::Stop()
{
	if (!mStopped)
	{
		int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

float Timer::TotalTime()const
{
	if (mStopped)
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	else
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
}

float Timer::DeltaTime() const
{
	return (float)mDeltaTime;
}

float Timer::GetFps() const
{
	return mFps;
}

void Timer::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((TotalTime() - timeElapsed) >= 1.0f)
	{
		mFps = (float)frameCnt;
		mSpf = 1000.0f / mFps;

		wstring fpsStr = to_wstring(mFps);
		wstring mspfStr = to_wstring(mSpf);
		wstring wndCaption = L"DirectX Game";
		wstring windowText = wndCaption + L"    fps: " + fpsStr + L"   mspf: " + mspfStr;

		SetWindowText(mInfo.hwnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}



