#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "ObjFactory.h"
#include "Obj.h"
#include "MementoMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Initialize()
{
	m_CheckPairMap.insert({ PLAYER, BULLET });
	m_CheckPairMap.insert({ PLAYER, ENEMY });
	m_CheckPairMap.insert({ PLAYER, WALL });
	m_CheckPairMap.insert({ ENEMY, ITEM });
	m_CheckPairMap.insert({ ENEMY, WALL });
	m_CheckPairMap.insert({ ENEMY, BULLET });
	m_CheckPairMap.insert({ ITEM, WALL });
	m_CheckPairMap.insert({ BULLET, WALL });
	m_CheckPairMap.insert({ PLAYER, ITEM });

}

void CObjMgr::Update()
{

	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		for (int r = 0; r < OBJ_TYPE_END; ++r)\
		{
			for (const auto iter : m_CheckPairMap)
			{
				if ((i == iter.first && r == iter.second) || (i == iter.second && r == iter.first))
					CCollisionMgr::Get_Instance()->Update(m_ObjList[i], m_ObjList[r]);
			}
		}
			
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Update();
		}
	}
}

void CObjMgr::LateUpdate()
{
	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
		{
			(*iter)->LateUpdate();

			if ((*iter)->Get_State() == DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Render(HDC hDC)
{
	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDC);
	}

	/*WCHAR text[100];
	_stprintf_s(text, L"[Bullet] %d ", m_ObjList[BULLET].size());
	TextOutW(hDC, 0, 60, text, lstrlen(text));*/
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			Safe_Delete<CObj*>(iter);
		m_ObjList[i].clear();
	}

	//DeleteDC()
}

void CObjMgr::Add_Object(OBJID eID, CObj* pInstance)
{
	if (eID >= OBJ_TYPE_END || pInstance == nullptr)
		return;

	m_ObjList[eID].push_back(pInstance);
}

void CObjMgr::Change_Scene()
{
	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		if (i != PLAYER)
		{
			for (auto& iter : m_ObjList[i])
				Safe_Delete<CObj*>(iter);
			m_ObjList[i].clear();
		}
	}
}

