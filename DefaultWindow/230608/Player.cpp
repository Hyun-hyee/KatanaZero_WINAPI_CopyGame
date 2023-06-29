#include "stdafx.h"
#include "Player.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "SceneManager.h"
#include "BmpMgr.h"
#include "BitMap.h"
#include "CameraComponent.h"
#include "ObjMgr.h"
#include "CollisionManager.h"
#include "ObjFactory.h"

#include "Portal.h"

#include "BasicAttack.h"
#include "InstallAttack.h"
#include "LaserAttack.h"
#include "SharkAttack.h"
#include "FollowBullet.h"
#include "Skill.h"

CPlayer::CPlayer()
{
	////////////////
	EDITMODE = false;
	////////////////

	m_Type = PLAYER;
	m_Collider_type = ELLIPSE;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize(void)
{
	CameraComponent* camera = new CameraComponent();
	AddComponent(camera);
	CCharacter::Initialize();
	
	m_tInfo = { 100.f, 900.f, 49.f, 74.f };
	m_fJump_Power = 20.f;
	m_fJump_Angle = PI / 3;
	m_iAdditionJump_MaxCount = 3;
	m_iAdditionJump_Count = m_iAdditionJump_MaxCount;

	if (EDITMODE)
		m_fSpeed = 20.f;
	else
		m_fSpeed = 10.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Idle.gif", L"Player_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Atk_1.gif", L"Player_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Move.gif", L"Player_MOVE");

	m_AnimMap.insert({ L"Player_IDLE" , 2 });
	m_AnimMap.insert({ L"Player_ATTACK" , 3 });
	m_AnimMap.insert({ L"Player_MOVE" , 4 });
	
	m_AnimKey = L"Player_MOVE";
	AnimChange(m_AnimKey);
	RatioFixByImage(m_AnimKey);


	//Pet = CObjFactory<CPortal>::Create(
	//	m_FrontCollide.left + (m_FrontCollide.right - m_FrontCollide.left)/2,
	//	m_FrontCollide.top + (m_FrontCollide.bottom - m_FrontCollide.top) / 2,
	//	40, 100);
	//dynamic_cast<CPortal*> (Pet)->Set_PortalType(NEXT);
	//CObjMgr::Get_Instance()->Add_Object(PLAYER, Pet);
}

void CPlayer::Update(void)
{
	/*Pet->Set_Pos(m_FrontCollide.left + (m_FrontCollide.right - m_FrontCollide.left) / 2,
		m_FrontCollide.top + (m_FrontCollide.bottom - m_FrontCollide.top) / 2);*/
	
	//카메라 업데이트
	CCharacter::Update();

	//키값 판단
	Key_Input();

	//EDIT모드 아닐때 중력 적용
	if (!EDITMODE)
		Jump();

	//상태 업데이트 (애니메이션 전환)
	StateUpdate();

	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
}

void CPlayer::LateUpdate(void)
{
}

void CPlayer::Render(HDC hDC)
{
	//모든 캐릭터,스킬에 필요
	CCharacter::Render(hDC);
	CObj::Render(hDC);

	//플레이어만 필요
	if (EDITMODE)
	{
		WCHAR text[100];
		_stprintf_s(text, L"[Player] x : %f \t y : %f", m_tInfo.fX, m_tInfo.fY);
		TextOutW(hDC, 0, 0, text, lstrlen(text));

		POINT	ptMouse{};

		GetCursorPos(&ptMouse);	// 마우스 위치 값을 얻어오는 함수

		ScreenToClient(g_hWnd, &ptMouse); // 스크린 상의 좌표를 우리가 생성한 창 좌표로 변환

		_stprintf_s(text, L"[Mouse] x : %f \t y : %f", (float)ptMouse.x, (float)ptMouse.y);
		TextOutW(hDC, 0, 20, text, lstrlen(text));

		//g.TranslateTransform((float)ptMouse.x - ((int)cameraPos.x - WINCX / 2), 
		//						(float)ptMouse.y - ((int)cameraPos.y - WINCY / 2));

		//g.DrawImage(pImage,
		//	tRect, 0, 0,
		//	pImage->GetWidth(), pImage->GetHeight(), //이미지 원본 사이즈
		//	Gdiplus::UnitPixel, &attr);
	}

}

void CPlayer::Release(void)
{
}



void CPlayer::Key_Input(void)
{
	int Num = 0;

	m_tInfo.fPrevX = m_tInfo.fX;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
		Set_FrontAngle(PI);

		//상태 전환
		m_State = MOVE;
	}

	 if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		Set_FrontAngle(0);

		//상태 전환
		m_State = MOVE;
	}

	 if (CKeyMgr::Get_Instance()->Key_Down('S'))
	 {
		 m_State = ATTACK;
		 LaserAttack();
	 }

	 if (CKeyMgr::Get_Instance()->Key_Up('S'))
	 {
		 m_State = IDLE;
	 }

	 if (CKeyMgr::Get_Instance()->Key_Down('A'))
	 {
		 m_State = ATTACK;
		 SharkAttack();
	 }

	 if (CKeyMgr::Get_Instance()->Key_Up('A'))
	 {
		 m_State = IDLE;
	 }
	
	 if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT))
	 {
		 //상태 전환
		 m_State = IDLE;
	 }

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN) && !m_bJump)
		{
			m_tInfo.fY -= -20.0f;
		}
		else
		{
			if (0 < m_iAdditionJump_Count--)
			{
				m_fSpeed_Vertical = 10.f;
				m_bJump = true;
			}
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SHIFT))
	{
		//상태 전환
		m_State = ATTACK;
		BasicAttack();
	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_SHIFT))
	{
		m_State = IDLE;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		//상태 전환
		m_State = ATTACK;
		InstallAttack();
	}

	if (CKeyMgr::Get_Instance()->Key_Up('Q'))
	{
		m_State = IDLE;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		//상태 전환
		m_State = ATTACK;
		FollowBullet();
	}

	if (CKeyMgr::Get_Instance()->Key_Up('E'))
	{
		m_State = IDLE;
	}
	
	


	/*******************************************************/
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
		EDITMODE = !EDITMODE;

	if (EDITMODE)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			m_tInfo.fY += m_fSpeed;
			Set_FrontAngle(PI);
		}

		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			m_tInfo.fY -= m_fSpeed;
			Set_FrontAngle(0);
		}
	}
	/*******************************************************/


}


/////////////////////////////////////////////////////////////////
void CPlayer::Jump()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (0.f < fY - m_tInfo.fY)
		m_bJump = true;

	if (m_bJump)
	{
		if (bLineCol && (fY < m_tInfo.fY - m_fSpeed_Vertical) && m_fSpeed_Vertical < 0.f)
		{
			m_bJump = false;
			m_tInfo.fY = fY;
 			m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
		}
		else
		{
			m_tInfo.fY -= m_fSpeed_Vertical;
			m_fSpeed_Vertical -= (0.034f * G);
		}
	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
		m_fSpeed_Vertical = 0.f;
	}
	else
	{
		m_tInfo.fY -= m_fSpeed_Vertical;
		m_fSpeed_Vertical -= (0.034f * G);
	}


}

/////////////////////////////////////////////////////////////////

//각 객체의 해당 애니메이션 키값 넣어주기
void CPlayer::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"Player_IDLE";
		break;

	case MOVE:
		m_AnimKey = L"Player_MOVE";
		break;

	case ATTACK:
		m_AnimKey = L"Player_ATTACK";
		break;

	//case DEAD:
	//	m_AnimKey = L"Player_DEAD";
	//	break;
	}
	AnimChange(m_AnimKey);
}

void CPlayer::FindFrontMonster(vector<CObj*>& _targetVec)
{
	list<CObj*>* pMonsterList = CObjMgr::Get_Instance()->Get_ObjList(MONSTER);
	
	for (auto& iter : (*pMonsterList))
	{
		RECT* pMonsterCollide = iter->Get_Collide();
		if (DIR_NONE != CCollisionManager::Collision_Enter_SS(&m_FrontCollide, pMonsterCollide))
		{
			_targetVec.push_back(iter);
		}
	}
}

void CPlayer::BasicAttack()
{
	vector<CObj*> pTargetVec = {};
	FindFrontMonster(pTargetVec);

	if (!(pTargetVec.empty()))
	{
		for (const auto& iter : pTargetVec)
		{
			INFO pTargetInfo = *iter->Get_Info();
			CObj* pSkill = CObjFactory<CBasicAttack>::Create(pTargetInfo.fX, pTargetInfo.fY, 200, 200);
			pSkill->SetOwner(this);
			dynamic_cast<CBasicAttack*>(pSkill)->Set_Target(iter);
			dynamic_cast<CBasicAttack*>(pSkill)->SetOwnerImage();
			CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
		}
	}

}

void CPlayer::InstallAttack()
{
	CObj* pSkill = CObjFactory<CInstallAttack>::Create(m_tInfo.fX, m_tInfo.fY - 100, 500 * SMALL, 350 * SMALL);
	pSkill->SetOwner(this);
	dynamic_cast<CInstallAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}


void CPlayer::LaserAttack()
{
	CObj* pSkill = CObjFactory<CLaserAttack>::Create(m_tInfo.fX, m_tInfo.fY - 100, 800, 550);
	pSkill->SetOwner(this);
	dynamic_cast<CLaserAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}

void CPlayer::SharkAttack()
{
	CObj* pSkill = CObjFactory<CSharkAttack>::Create(m_tInfo.fX, m_tInfo.fY - 100, 800, 550);
	pSkill->SetOwner(this);
	dynamic_cast<CSharkAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}


void CPlayer::FollowBullet()
{
	CObj* pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
	pSkill->SetOwner(this);
	dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI / 6);
	dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

	pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
	pSkill->SetOwner(this);
	dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 2 / 6);
	dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

	pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
	pSkill->SetOwner(this);
	dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 3 / 6);
	dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}






int CPlayer::OnCollision(CObj* _target, DIR _dir)
{

	return 0;
}



void CPlayer::Set_Angle()
{

}

