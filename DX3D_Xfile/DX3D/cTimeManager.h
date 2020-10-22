#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
//public:
//	cTimeManager();
//	~cTimeManager();
private:
	Singletone(cTimeManager);
private:
	DWORD	m_dwLastUpdateTime;
	float	m_fElapsedTime;
public:
	void Update();
	float GetElapsedTime();
	float GetLastUpdateTime();
};