#pragma once
#include "Define.h"
#include "Enum.h"
#include "Struct.h"

class CObj;
class CPlayer;
class CArmEnemy;
class CGunEnemy;
class CBullet;
class CItem;

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

	deque <CObj*>		m_pMementoList[OBJ_TYPE_END];

	deque <fPOINT>	 m_CameraList;
	deque  <int>	m_BulletSize;
	deque  <int>	m_ItemSize;
	deque  <int>	m_LaserSize;
	deque  <int>	m_EnemySize;
	deque  <int>	m_EffectSize;
	bool			m_EnemyChange;

	bool		m_ReverseOn;
	int			m_ReverseSpeed;
public:
	void		SaveMemento();
	void		RestoreMemento();
	void		ClearReverse();//리플레이

	bool		GetReverseOn() { return m_ReverseOn; }
	
	void		ChangeScene();

};

