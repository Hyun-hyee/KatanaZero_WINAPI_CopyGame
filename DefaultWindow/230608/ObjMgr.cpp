#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "ObjFactory.h"
#include "Obj.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
	m_CheckPairMap.insert({ PLAYER, SKILL });
	m_CheckPairMap.insert({ PLAYER, ENEMY });
	m_CheckPairMap.insert({ PLAYER, WALL });
	m_CheckPairMap.insert({ ENEMY, SKILL });
	m_CheckPairMap.insert({ ITEM, WALL });
	m_CheckPairMap.insert({ PLAYER, ITEM });
}

CObjMgr::~CObjMgr()
{
	Release();
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

