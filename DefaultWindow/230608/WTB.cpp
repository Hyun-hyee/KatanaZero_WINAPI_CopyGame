#include "stdafx.h"
#include "WTB.h"
#include "Thorn.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ShootingStar.h"
#include "CollisionManager.h"

CWTB::CWTB():m_Random(CHARACTER_STATE_END), m_bOneTime(true), m_dwTime(GetTickCount64()), m_Ignore(false), m_WTState(WT_BEFORE),
m_Start(false), m_Life(1000)
{
	ZeroMemory(&m_pTarget, sizeof(m_pTarget));
	ZeroMemory(&m_First, sizeof(m_First));

	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
}

CWTB::~CWTB()
{
	Release();
}

void CWTB::Initialize()
{
	m_fSpeed = 10.f;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/등장전.gif", L"WT_BEFORE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/기본.gif", L"WT_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/투사체.gif", L"WT_SHOOTING_STAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/불꽃.gif", L"WT_FIREWORKS");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/가시.gif", L"WT_THORNY_VINE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/죽음.gif", L"WT_DEAD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/내려찍기.gif", L"WT_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스/가시순간이동.gif", L"WT_MOVE");
	
	m_AnimMap.insert({ L"WT_BEFORE" , 12 });
	m_AnimMap.insert({ L"WT_IDLE" , 10 });
	m_AnimMap.insert({ L"WT_SHOOTING_STAR" , 11 });
	m_AnimMap.insert({ L"WT_FIREWORKS" , 3 });
	m_AnimMap.insert({ L"WT_THORNY_VINE" , 11 });
	m_AnimMap.insert({ L"WT_DEAD" , 9 });
	m_AnimMap.insert({ L"WT_ATTACK" , 16 });
	m_AnimMap.insert({ L"WT_MOVE" , 6 });

	AnimChange(L"WT_BEFORE");
	RatioFixByImage(m_AnimKey);

	m_First = { 1000.f,850.f };
}

void CWTB::Update()
{
	if(!m_Ignore)
		Gravity();

	if (CCollisionManager::Collision_Enter_SS(m_pTarget->Get_Collide(), &m_Collide) != DIR_NONE&&!m_Start) {
		m_WTState = WT_IDLE;
		if (m_dwTime + 3000 < GetTickCount64()) {
			m_Start = true;
			m_dwTime = GetTickCount64();
		}
	}

	StateUpdate();

	if (m_Start) {

		if (m_PatternList.empty()) {
			m_tInfo.fX = m_First.x;
			m_tInfo.fY = m_First.y;
			//Attack();
			if (m_dwTime + 1000 < GetTickCount64()) {
				
				m_Random = rand() % 2 + 1;
				//m_Random = 2;
				m_bOneTime = true;
				m_Ignore = false;
				m_dwTime = GetTickCount64();
			}
		}

		Pattern();

		
	}
	
	

	CObj::Update_Rect();
}

void CWTB::LateUpdate(void)
{
	for (auto iter = m_PatternList.begin(); iter != m_PatternList.end();) {
		if ((*iter)->Get_State() == DEAD) {
			iter = m_PatternList.erase(iter);
		}
		else {
			iter++;
		}
	}

	if (m_pTarget->Get_Info()->fX < m_tInfo.fX) {
		Set_FrontAngle(PI);
	}
	else {
		Set_FrontAngle(0);
	}

	if (m_Life <= 0) {
		m_WTState = BE_DEAD;
	}
}

void CWTB::Render(HDC hdc)
{
	TCHAR m_szFPS[32] = L"";
	swprintf_s(m_szFPS, L"Life : %d", m_Life);
	SetWindowText(g_hWnd, m_szFPS);

	CObj::Render(hdc);
}

void CWTB::Release(void)
{
	m_PatternList.clear();
	
}

int CWTB::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL)
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER) {
			--m_Life;
		}
	}

	return OBJ_NOEVENT;
}

void CWTB::Pattern()
{
	switch (m_Random)
	{
	case THORNY_VINE:
		Create_Pattern(THORNY_VINE);
		break;
	case SHOOTING_STAR:
		Create_Pattern(SHOOTING_STAR);
		break;
	case FIREWORKS:
		Create_Pattern(FIREWORKS);
		break;
	}
}

void CWTB::Create_Pattern(WT_STATE _pattern)
{
	switch (_pattern)
	{
	case THORNY_VINE:
		if (m_bOneTime) {
			m_tInfo.fX = rand() % WINCX + 1;
			
			m_WTState = THORNY_VINE;
			for (int i = 0; i < 10; i++) {
				
				m_PatternList.push_back(CObjFactory<CThorn>::Create((float)(i * 200 + 100), 730.f, 320 * SMALL, 600 * SMALL));
				dynamic_cast<CThorn*>(m_PatternList.back())->Set_Rect();
				m_PatternList.back()->SetOwner(this);
				CObjMgr::Get_Instance()->Add_Object(MONSTER, m_PatternList.back());
			}
			m_bOneTime = false;
		}
		break;
	case SHOOTING_STAR:
		if (m_bOneTime) {
			m_tInfo.fY = rand() % WINCY + WINCY/3;
			m_WTState = SHOOTING_STAR;
			m_Ignore = true;
			for (int i = 0; i < 9; i++) {
				float fx = m_tInfo.fX + (float)(i - 4) * 100.f;
				float fy = m_tInfo.fY - m_tInfo.fCY * 0.5f;
				m_PatternList.push_back(CObjFactory<CShootingStar>::Create(fx, fy, 180, 100));
				//dynamic_cast<CShootingStar*>(m_PatternList.back())->Set_TargetPos(m_pTarget->Get_Info()->fX, m_pTarget->Get_Info()->fY);
				
				//dynamic_cast<CShootingStar*>(m_PatternList.back())->Set_Time((i * 10) + 100);
				dynamic_cast<CShootingStar*>(m_PatternList.back())->Set_Count(i * 50);
				dynamic_cast<CShootingStar*>(m_PatternList.back())->Set_Target(m_pTarget);
				//dynamic_cast<CShootingStar*>(m_PatternList.back())->Find_Angle();
				CObjMgr::Get_Instance()->Add_Object(MONSTER, m_PatternList.back());
			}
			
			m_bOneTime = false;
		}
		break;
	case FIREWORKS:
		if (m_bOneTime) {

			m_bOneTime = false;
		}
		break;
	}
}

void CWTB::Attack()
{
	

	if (m_dwTime + 2000 < GetTickCount64()) {

		m_WTState = WT_MOVE    ;  
 	     	  float   fx = CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fX;

		  m_tInfo.fX = m_pTarget->Get_Info()->fX;

		m_WTState = WT_ATTACK;

		m_dwTime = GetTickCount();
	}
}

void CWTB::Gravity()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (bLineCol)
	{
		m_tInfo.fY = fY - 50;
	}
	else
	{
		m_tInfo.fY -= m_fSpeed_Vertical;
		m_fSpeed_Vertical -= (0.034f * G);
	}
}

void CWTB::StateUpdate()
{
	switch (m_WTState)
	{
	case WT_BEFORE:
		m_AnimKey = L"WT_BEFORE";
		break;
	case WT_IDLE:
		m_AnimKey = L"WT_IDLE";
		break;
	case WT_ATTACK:
		m_AnimKey = L"WT_ATTACK";
		break;
	case THORNY_VINE:
		m_AnimKey = L"WT_THORNY_VINE";
		break;
	case SHOOTING_STAR:
		m_AnimKey = L"WT_SHOOTING_STAR";
		break;
	case FIREWORKS:
		m_AnimKey = L"WT_FIREWORKS";
		break;
	case WT_MOVE:
		m_AnimKey = L"WT_MOVE";
		break;
	case BE_DEAD:
		m_AnimKey = L"WT_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
			m_State = DEAD;
		break;
	}
	m_AnimFrame = m_AnimMap[m_AnimKey];

	

}
