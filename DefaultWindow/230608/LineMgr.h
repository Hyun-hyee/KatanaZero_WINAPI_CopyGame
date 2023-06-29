#pragma once
#include "Line.h"
#include "Define.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	static CLineMgr* Get_Instance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new CLineMgr;
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

private:
	static CLineMgr* m_pInstance;
	list<CLine*> m_LineList;

public:
	void Add_Line(LINEPOINT _Lpoint, LINEPOINT _Rpoint);
	bool CollisionLine(INFO& _rInfo, float* _pY);

	void Change_Scene();

};

