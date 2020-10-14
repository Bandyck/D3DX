#pragma once
class cObject
{
public:
	cObject();
	//~cObject();
protected:
	ULONG m_ulRefCount; 
public :
	virtual ~cObject(void); 

	virtual	void AddRef(); 
	virtual void Release(); 
};

