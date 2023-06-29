#include "stdafx.h"
#include "Thorn.h"
#include "ObjMgr.h"
#include "BmpMgr.h"

CThorn::CThorn():m_dwTime(GetTickCount64()), m_bStart(true)
{
	ZeroMemory(&m_Warning, sizeof(m_Warning));
}

CThorn::~CThorn()
{
	Release();
}

void CThorn::Initialize()
{
	m_fSpeed = 20.f;

	

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스 패턴/가시/Thorn.gif", L"Thorn_IDLE");

	m_AnimMap.insert({ L"Thorn_IDLE" , 11 });

	AnimChange(L"Thorn_IDLE");
	RatioFixByImage(m_AnimKey);

}

void CThorn::Update()
{
	

	if (m_tRect.top <= 10) {
		m_fSpeed = -1 * m_fSpeed;
	}
	else if (m_tRect.top >= 1000) {
		m_State = DEAD;
	}

	if (!m_bStart) {
		if (m_dwTime + 10 < GetTickCount()) {

			m_tRect.top -= m_fSpeed;

			m_dwTime = GetTickCount();
		}
	}

	//CObj::Update_Rect();
}

void CThorn::LateUpdate(void)
{
	if (m_bStart) {
		if (m_dwTime + 800 < GetTickCount()) {
			m_bStart = false;
			m_dwTime = GetTickCount();
		}
	}
}

void CThorn::Render(HDC hdc)
{
	
	CObj::Render(hdc);
}

void CThorn::Release(void)
{
}

int CThorn::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}

