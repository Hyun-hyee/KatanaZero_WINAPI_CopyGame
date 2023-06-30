#pragma once

#include "Define.h"
#include "Enum.h"

class CObj;

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

	static CObjMgr* m_pInstance;

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;
		}

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


private:
	list<CObj*>		m_ObjList[OBJ_TYPE_END];

	multimap <OBJ_TYPE, OBJ_TYPE> m_CheckPairMap;

public:
	void 		Update();
	void		LateUpdate();
	void		Render(HDC hDC);
	void		Release();


public:
	CObj* Get_Player() { return m_ObjList[PLAYER].front(); }
	list<CObj*>* Get_ObjList(OBJID eID) { return &m_ObjList[eID]; }
	void  Add_Object(OBJID eID, CObj* pInstance);

	void Change_Scene();


};

