#pragma once
#include "Define.h"
#include "Enum.h"
#include "Struct.h"

class CObj;

class CMementoMgr
{
private:
	CMementoMgr();
	~CMementoMgr();
	static CMementoMgr* m_pInstance;
	CObj* pTemp;
public:
	static CMementoMgr* Get_Instance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new CMementoMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void		Initialize();
	void		Render(HDC hDC);
	void		Release();
	void		Update();
	void		LateUpdate();

private:
	list<CObj*>*	 m_pObjList[OBJ_TYPE_END];
	vector<CObj*>    m_pMementoList[OBJ_TYPE_END];
	
	vector<fPOINT>	 m_CameraList;
	vector <int>	m_BulletSize;
	vector <int>	m_ItemSize;

	bool		m_ReverseOn;

public:
	void		SaveMemento();
	void		RestoreMemento();
	bool		GetReverseOn() { return m_ReverseOn; }

};

