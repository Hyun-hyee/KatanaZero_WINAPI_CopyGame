#include "stdafx.h"
#include "MementoMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "KeyMgr.h"
#include "SceneManager.h"
#include "ArmEnemy.h"
#include "GunEnemy.h"
#include "Bullet.h"
#include "Item.h"

CMementoMgr* CMementoMgr::m_pInstance = nullptr;

CMementoMgr::CMementoMgr()
{
	m_ReverseOn = false;
}

CMementoMgr::~CMementoMgr()
{
	Release();
}

void CMementoMgr::Initialize()
{
	m_pObjList[PLAYER] = CObjMgr::Get_Instance()->Get_ObjList(PLAYER);
	m_pObjList[BULLET] = CObjMgr::Get_Instance()->Get_ObjList(BULLET);
	m_pObjList[EFFECT] = CObjMgr::Get_Instance()->Get_ObjList(EFFECT);
	m_pObjList[ENEMY] = CObjMgr::Get_Instance()->Get_ObjList(ENEMY);
	m_pObjList[ITEM] = CObjMgr::Get_Instance()->Get_ObjList(ITEM);
	
}

void CMementoMgr::Render(HDC hDC)
{
	WCHAR text[100];
	_stprintf_s(text, L"[Memento] %d ", m_ReverseOn);
	TextOutW(hDC, 0, 60, text, lstrlen(text));
	if (!m_pMementoList[PLAYER].empty())
	{
		_stprintf_s(text, L"[Memento] %f ", m_pMementoList[PLAYER].back()->Get_Info()->fX);
		TextOutW(hDC, 0, 80, text, lstrlen(text));
	}

}

void CMementoMgr::Release()
{
	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		for (auto& iter : m_pMementoList[i])
			Safe_Delete<CObj*>(iter);
		m_pMementoList[i].clear();
	}
}

void CMementoMgr::Update()
{
	if(CKeyMgr::Get_Instance()->Key_Down('L'))
		m_ReverseOn = !m_ReverseOn;
}

void CMementoMgr::LateUpdate()
{
	if (m_ReverseOn)
		RestoreMemento();
	else
		SaveMemento();
}

void CMementoMgr::SaveMemento()
{
	//카메라
	m_CameraList.push_back(CSceneManager::Get_Instance()->GetCameraPos());
	
	//플레이어
	pTemp = new CPlayer;
	*pTemp = *m_pObjList[PLAYER]->front();
	m_pMementoList[PLAYER].push_back(pTemp);
	
	//적

	for (auto& iter : *m_pObjList[ENEMY])
	{
		if (typeid(iter) == typeid(CArmEnemy))
			pTemp = new CArmEnemy;
		else
			pTemp = new CGunEnemy;
		
		*pTemp = *iter;
		m_pMementoList[ENEMY].push_back(pTemp);
	}

	//총알
	m_BulletSize.push_back(m_pObjList[BULLET]->size());
	if (!m_pObjList[BULLET]->empty())
	{
		for (auto& iter : *m_pObjList[BULLET])
		{
			pTemp = new CBullet;
			*pTemp = *iter;
			m_pMementoList[BULLET].push_back(pTemp);
		}
	}

	//아이템
	 m_ItemSize.push_back(m_pObjList[ITEM]->size());
	if (!m_pObjList[ITEM]->empty())
	{
		for (auto& iter : *m_pObjList[ITEM])
		{
			pTemp = new CItem;
			*pTemp = *iter;
			m_pMementoList[ITEM].push_back(pTemp);
		}
	}
	
	pTemp = nullptr;
}

void CMementoMgr::RestoreMemento()
{
	//플레이어 기준
	if (!m_pMementoList[PLAYER].empty())
	{
		//플레이어
		(*CObjMgr::Get_Instance()->Get_Player()) = (*m_pMementoList[PLAYER].back());		
		Safe_Delete<CObj*>(m_pMementoList[PLAYER].back());
		m_pMementoList[PLAYER].pop_back();

		//적 
		if (!m_pMementoList[ENEMY].empty())
		{
			list<CObj*>::reverse_iterator riter((*m_pObjList[ENEMY]).rbegin());
			for (; riter != (*m_pObjList[ENEMY]).rend(); ++riter)
			{
				**(riter) = (*m_pMementoList[ENEMY].back());
				Safe_Delete<CObj*>(m_pMementoList[ENEMY].back());
				m_pMementoList[ENEMY].pop_back();
			}
		}

		//총알
		if (!m_pMementoList[BULLET].empty())
		{
			if (!m_BulletSize.empty())
			{
				if (m_BulletSize.back() != 0)
				{
					for (int i = 0; i < m_BulletSize.back(); ++i)
					{
						if (!m_pObjList[BULLET]->empty())
						{
							for (auto& iter : *m_pObjList[BULLET])
								Safe_Delete<CObj*>(iter);
							m_pObjList[BULLET]->clear();
						}

						m_pObjList[BULLET]->push_back((m_pMementoList[BULLET].back()));
						m_pMementoList[BULLET].pop_back();
						m_BulletSize.pop_back();
					}
				}
				else
					m_BulletSize.pop_back();
			}
		
		}

		//아이템
		/*if (!m_pMementoList[ITEM].empty())
		{
			if (!m_ItemSize.empty())
			{
				if (m_ItemSize.back() != 0)
				{
					for (int i = 0; i < m_ItemSize.back(); ++i)
					{
						if (!m_pObjList[ITEM]->empty())
						{
							for (auto& iter : *m_pObjList[ITEM])
								Safe_Delete<CObj*>(iter);
							m_pObjList[ITEM]->clear();
						}

						m_pObjList[ITEM]->push_back((m_pMementoList[ITEM].back()));
						m_pMementoList[ITEM].pop_back();
						m_ItemSize.pop_back();
					}
				}
				else
					m_ItemSize.pop_back();
			}

		}*/
			
		

		//카메라
		if (!m_CameraList.empty())
		{
			CSceneManager::Get_Instance()->SetCameraPos(m_CameraList.back());
			m_CameraList.pop_back();
		}
		
	}
	else
		m_ReverseOn = false; //플레이어 모든 프레임 카운트 -> 플레이어 끝나면 다끝남
	
	
}
