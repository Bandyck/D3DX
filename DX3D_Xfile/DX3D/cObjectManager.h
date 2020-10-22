#pragma once

#define		g_pObjectManager cObjectManager::GetInstance()

class cObjectManager
{
//public:
//	cObjectManager();
//	~cObjectManager();

private : 
	Singletone(cObjectManager); 

	std::set<cObject*> m_setObject; 

public :
	void AddObject(cObject* pObject); 
	void RemoveObject(cObject* pObject); 
	void Destroy(); 

};

