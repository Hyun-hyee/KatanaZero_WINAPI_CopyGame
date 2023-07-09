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
#include "UIMgr.h"
#include "Boss.h"
#include "Laser.h"

CMementoMgr* CMementoMgr::m_pInstance = nullptr;

CMementoMgr::CMementoMgr()
{
	m_ReverseOn = false;
	m_ReverseSpeed = 0;
	m_EnemyChange = false;
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
	m_pObjList[BOSS] = CObjMgr::Get_Instance()->Get_ObjList(BOSS);
	m_pObjList[LASER] = CObjMgr::Get_Instance()->Get_ObjList(LASER);
	
}

void CMementoMgr::Render(HDC hDC)
{
	WCHAR text[100];
	_stprintf_s(text, L"[Memento] %d ", m_ReverseOn);
	TextOutW(hDC, 0, 80, text, lstrlen(text));
	if (!m_pMementoList[PLAYER].empty())
	{
		_stprintf_s(text, L"[Memento] %f ", m_pMementoList[PLAYER].back()->Get_Info()->fX);
		TextOutW(hDC, 0, 100, text, lstrlen(text));
	}

}

void CMementoMgr::Release()
{
	for (int i = 0; i < OBJ_TYPE_END; ++i)
	{
		for (auto& iter : m_pMementoList[i])
		{
			if (iter != nullptr)
			{
				if (typeid(*iter) == typeid(CPlayer))
					dynamic_cast<CPlayer*>(iter)->CameraReMake();
				Safe_Delete<CObj*>(iter);
			}
		}
		m_pMementoList[i].clear();
	}
}

void CMementoMgr::Update()
{
	if(CKeyMgr::Get_Instance()->Key_Down('L'))
		m_ReverseOn = !m_ReverseOn;

	if (m_ReverseOn)
		g_ReverseOn = -10;
	else
		g_ReverseOn = 0;

	if (!m_pObjList[BOSS]->empty())
	{
		if (dynamic_cast<CBoss*>(m_pObjList[BOSS]->front())->Get_Phase() == 1)
			m_EnemyChange = true;
		else
			m_EnemyChange = false;
	}
}

void CMementoMgr::LateUpdate()
{
	++m_ReverseSpeed;
	if (m_ReverseSpeed == 1)
	{
		if (m_ReverseOn)
			RestoreMemento();
		else
			SaveMemento();
	}
	else if (m_ReverseSpeed == 3)
		m_ReverseSpeed = 0;
}

void CMementoMgr::SaveMemento()
{
	//ī�޶�
	m_CameraList.push_back(CSceneManager::Get_Instance()->GetCameraPos());
	
	//�÷��̾�
	CPlayer* pTempP = new CPlayer;
	*pTempP = *(dynamic_cast<CPlayer*> (m_pObjList[PLAYER]->front()));
	m_pMementoList[PLAYER].push_back(pTempP);
	
	//��
	if (m_EnemyChange)
		m_EnemySize.push_back(m_pObjList[ENEMY]->size());
	for (auto& iter : *m_pObjList[ENEMY])
	{
		if (typeid(*iter) == typeid(CArmEnemy))
		{
			CArmEnemy* pTempEA = new CArmEnemy;
			*pTempEA = *( dynamic_cast<CArmEnemy*> (iter));
			m_pMementoList[ENEMY].push_back(pTempEA);
		}
		else if (typeid(*iter) == typeid(CGunEnemy))
		{
			CGunEnemy* pTempEG = new CGunEnemy;
			*pTempEG = *(dynamic_cast<CGunEnemy*> (iter));
			m_pMementoList[ENEMY].push_back(pTempEG);
		}
	}

	//����
	for (auto& iter : *m_pObjList[BOSS])
	{
		if (typeid(*iter) == typeid(CBoss))
		{
			CBoss* pTempEB = new CBoss;
			*pTempEB = *(dynamic_cast<CBoss*> (iter));
			m_pMementoList[BOSS].push_back(pTempEB);
		}
	}

	//�Ѿ�
	m_BulletSize.push_back(m_pObjList[BULLET]->size());
	if (!m_pObjList[BULLET]->empty())
	{
		for (auto& iter : *m_pObjList[BULLET])
		{
			CBullet* pTempB = new CBullet;
			*pTempB = *(dynamic_cast<CBullet*> (iter));
			m_pMementoList[BULLET].push_back(pTempB);
		}
	}

	//������
	m_LaserSize.push_back(m_pObjList[LASER]->size());
	if (!m_pObjList[LASER]->empty())
	{
		for (auto& iter : *m_pObjList[LASER])
		{
			CLaser* pTempL = new CLaser;
			*pTempL = *(dynamic_cast<CLaser*> (iter));
			m_pMementoList[LASER].push_back(pTempL);
		}
	}

	//������
	 m_ItemSize.push_back(m_pObjList[ITEM]->size());
	if (!m_pObjList[ITEM]->empty())
	{
		for (auto& iter : *m_pObjList[ITEM])
		{
			CItem* pTempI = new CItem;
			*pTempI = *(dynamic_cast<CItem*> (iter));
			m_pMementoList[ITEM].push_back(pTempI);
		}
	}
	
	pTemp = nullptr;
}

void CMementoMgr::RestoreMemento()
{
	//�÷��̾� ����
	if (!m_pMementoList[PLAYER].empty())
	{
		//�÷��̾�
		CPlayer* PlayerTemp = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		* PlayerTemp = (*dynamic_cast<CPlayer*>(m_pMementoList[PLAYER].back()));
		dynamic_cast<CPlayer*>(m_pMementoList[PLAYER].back())->CameraReMake();
		CUIMgr::Get_Instance()->SetBattery(PlayerTemp->GetBatteryCount());
		CUIMgr::Get_Instance()->SetInven(PlayerTemp->GetItemState());
		Safe_Delete<CObj*>(m_pMementoList[PLAYER].back());
		m_pMementoList[PLAYER].pop_back();

		//�� 
		if (!m_EnemyChange)
		{
			if (!m_pMementoList[ENEMY].empty())
			{
				list<CObj*>::reverse_iterator riter((*m_pObjList[ENEMY]).rbegin());
				for (; riter != (*m_pObjList[ENEMY]).rend(); ++riter)
				{
					if (typeid(**riter) == typeid(CArmEnemy))
					{
						*dynamic_cast<CArmEnemy*>(*(riter)) = (*dynamic_cast<CArmEnemy*>(m_pMementoList[ENEMY].back()));
						Safe_Delete<CObj*>(m_pMementoList[ENEMY].back());
						m_pMementoList[ENEMY].pop_back();
					}
					else if (typeid(**riter) == typeid(CGunEnemy))
					{
						*dynamic_cast<CGunEnemy*>(*(riter)) = (*dynamic_cast<CGunEnemy*>(m_pMementoList[ENEMY].back()));
						Safe_Delete<CObj*>(m_pMementoList[ENEMY].back());
						m_pMementoList[ENEMY].pop_back();
					}
				}
			}
		}
		else
		{
			if (!m_EnemySize.empty())
			{
				if (!m_pObjList[ENEMY]->empty())
				{
					for (auto& iter : *m_pObjList[ENEMY])
						Safe_Delete<CObj*>(iter);
					m_pObjList[ENEMY]->clear();
				}
				if (m_EnemySize.back() != 0)
				{
					for (int i = 0; i < m_EnemySize.back(); ++i)
					{
						m_pObjList[ENEMY]->push_back((m_pMementoList[ENEMY].back()));
						m_pMementoList[ENEMY].pop_back();
					}
				}
				m_EnemySize.pop_back();
			}
		}
		
		
		
		

		//����
		if (!m_pMementoList[BOSS].empty())
		{
			list<CObj*>::reverse_iterator riter((*m_pObjList[BOSS]).rbegin());
			for (; riter != (*m_pObjList[BOSS]).rend(); ++riter)
			{
				if (typeid(**riter) == typeid(CBoss))
				{
					*dynamic_cast<CBoss*>(*(riter)) = (*dynamic_cast<CBoss*>(m_pMementoList[BOSS].back()));
					Safe_Delete<CObj*>(m_pMementoList[BOSS].back());
					m_pMementoList[BOSS].pop_back();
				}
			}
		}

		//�Ѿ�
		//m_BulletSize�� �����°� ��� ������ ���ư����̹Ƿ� ���� ����
			if (!m_BulletSize.empty())
			{
				if (!m_pObjList[BULLET]->empty())
				{
					for (auto& iter : *m_pObjList[BULLET])
						Safe_Delete<CObj*>(iter);
					m_pObjList[BULLET]->clear();
				}
				if (m_BulletSize.back() != 0)
				{
					for (int i = 0; i < m_BulletSize.back(); ++i)
					{					
						m_pObjList[BULLET]->push_back((m_pMementoList[BULLET].back()));
						m_pMementoList[BULLET].pop_back();
					}
				}
				m_BulletSize.pop_back();
			}
		
		//������
		//m_LaserSize�� �����°� ��� ������ ���ư����̹Ƿ� ���� ����
		if (!m_LaserSize.empty())
		{
			if (!m_pObjList[LASER]->empty())
			{
				for (auto& iter : *m_pObjList[LASER])
					Safe_Delete<CObj*>(iter);
				m_pObjList[LASER]->clear();
			}
			if (m_LaserSize.back() != 0)
			{
				for (int i = 0; i < m_LaserSize.back(); ++i)
				{
					m_pObjList[LASER]->push_back((m_pMementoList[LASER].back()));
					m_pMementoList[LASER].pop_back();
				}
			}
			m_LaserSize.pop_back();
		}


		//������
		//m_ItemSize�� �����°� ��� ������ ���ư����̹Ƿ� ���� ����
			if (!m_ItemSize.empty())
			{
				if (!m_pObjList[ITEM]->empty())
				{
					for (auto& iter : *m_pObjList[ITEM])
						Safe_Delete<CObj*>(iter);
					m_pObjList[ITEM]->clear();
				}
				if (m_ItemSize.back() != 0)
				{					
					for (int i = 0; i < m_ItemSize.back(); ++i)
					{
						m_pObjList[ITEM]->push_back((m_pMementoList[ITEM].back()));
						m_pMementoList[ITEM].pop_back();
					}
				}				
				m_ItemSize.pop_back();
			}
	
		


		//ī�޶�
		if (!m_CameraList.empty())
		{
			CSceneManager::Get_Instance()->SetCameraPos(m_CameraList.back());
			m_CameraList.pop_back();
		}
		
	}
	else
		m_ReverseOn = false; //�÷��̾� ��� ������ ī��Ʈ -> �÷��̾� ������ �ٳ���
	
	
}

void CMementoMgr::ChangeScene()
{
	Release();
}
