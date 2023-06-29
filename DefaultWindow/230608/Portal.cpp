#include "stdafx.h"
#include "Portal.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjMgr.h"
#include "KeyMgr.h"

CPortal::CPortal()
{
}

CPortal::~CPortal()
{
}

void CPortal::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Portal/Portal_White.gif", L"Portal");

	m_AnimMap.insert({ L"Portal" , 8 });

	m_AnimKey = L"Portal";
	AnimChange(m_AnimKey);
	RatioFixByImage(m_AnimKey);

	m_Collider_type = RECTANGLE;
}

void CPortal::Update(void)
{
	__super::Update_Rect();
}

void CPortal::LateUpdate(void)
{
}

void CPortal::Render(HDC hDC)
{

	CObj::Render(hDC);

}

void CPortal::Release(void)
{
}

int CPortal::OnCollision(CObj* _target, DIR _dir)
{
	if (_target->Get_Type() == PLAYER)
	{

		if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
		{
			if (m_PortalType == NEXT)
			{
				CSceneManager::Get_Instance()->ToNextScene();
			}
			else if (m_PortalType == PREV)
			{
				CSceneManager::Get_Instance()->ToPrevScene();
			}
		}

	}
	return OBJ_NOEVENT;
}
