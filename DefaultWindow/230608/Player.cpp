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
#include "CameraComponent.h"



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
	
	m_tInfo = { 600.f, 900.f, 49.f, 74.f };
	m_fJump_Power = 20.f;
	m_fJump_Angle = PI / 3;
	m_iAdditionJump_MaxCount = 3;
	m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
	m_fAccel = 0.f;
	m_fAccelTime = 0.f;

	if (EDITMODE)
		m_fSpeed = 20.f;
	else
		m_fSpeed = 5.f;

	InitImage();
}

void CPlayer::Update(void)
{
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

	CObj::Move_Frame();
}

void CPlayer::LateUpdate(void)
{
}

void CPlayer::Render(HDC hDC)
{
	//모든 캐릭터,스킬에 필요
	CCharacter::Render(hDC);
	CObj::FrameRender(hDC);

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
	}

}

void CPlayer::Release(void)
{
}

void CPlayer::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_idle_11x2.bmp", L"Player_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_idle_to_run_4x2.bmp", L"Player_IDLE_TO_RUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_run_10x2.bmp", L"Player_RUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_run_to_idle_5x2.bmp", L"Player_RUN_TO_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_attack_7x2.bmp", L"Player_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_jump_4x2.bmp", L"Player_JUMP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_roll_7x2.bmp", L"Player_ROLL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_flip_11x2.bmp", L"Player_FLIP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_fall_4x2.bmp", L"Player_FALL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_grab_wall_4x2.bmp", L"Player_GRAB_WALL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_hurtfly_4x2.bmp", L"Player_HURTFLY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_hurtground_6x2.bmp", L"Player_HURTGROUND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_doorbreak_10x2.bmp", L"Player_DOORBREAK");

	FRAME TempFrame;
	TempFrame.AnimKey = L"Player_IDLE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 10;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 200;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 70;
	TempFrame.iFrameSizeY = 70;

	m_FrameMap.insert({ IDLE, TempFrame });

	TempFrame.AnimKey = L"Player_RUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 200;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 88;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ RUN, TempFrame });

	TempFrame.AnimKey = L"Player_IDLE_TO_RUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 200;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 84;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ IDLE_TO_RUN, TempFrame });


	TempFrame.AnimKey = L"Player_RUN_TO_IDLE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 4;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 200;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 80;
	TempFrame.iFrameSizeY = 70;

	m_FrameMap.insert({ RUN_TO_IDLE, TempFrame });
}



void CPlayer::Key_Input(void)
{
	int Num = 0;

	m_tInfo.fPrevX = m_tInfo.fX;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		Set_FrontAngle(PI);
		m_tInfo.fX -= m_fSpeed + 0.5 * m_fAccel * m_fAccelTime;
	}

	 if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT) || CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		Set_FrontAngle(0);
		m_tInfo.fX += m_fSpeed + 0.5 * m_fAccel * m_fAccelTime;
	}


	 if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	 {
		 if (m_State == IDLE || m_State == IDLE_TO_RUN)
			 m_State = IDLE_TO_RUN;
		 else
			 m_State = RUN;
	 }
	 if (!CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) && !CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	 {
		 if (m_State == RUN || m_State == RUN_TO_IDLE)
			 m_State = RUN_TO_IDLE;
		 else
		 {
			 m_State = IDLE;
		 }
		
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
	if (m_PrevState != m_State)
	{
		m_FrameMap[m_State].iFrameStart = 0;
		m_FrameMap[m_State].dwSpeed = 200;
		m_FrameMap[m_State].dwTime = GetTickCount64();

		m_PrevState = m_State;
	}

	if (m_fFrontAngle == 0)
		m_FrameMap[m_State].iMotion = 0;
	else if (m_fFrontAngle == PI)
		m_FrameMap[m_State].iMotion = 1;

	switch (m_State)
	{
	case IDLE:
		m_fAccelTime = 0.f;
		break;

	case IDLE_TO_RUN:
			if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
				m_State = RUN;
			m_fAccel = 1.f;
			m_fAccelTime += 0.3f;
		break;

	case RUN:
		m_fAccel = 1.f;
		break;

	case RUN_TO_IDLE:
			if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
				m_State = IDLE;
			m_fAccel = 1.f;
			m_fAccelTime -= 0.3f;
		break;

	case ATTACK:
		break;

	case JUMP:
		break;

	case ROLL:
		break;

	case FLIP:
		break;

	case FALL:
		break;

	case GRAB_WALL:
		break;

	case HURTFLY:
		break;

	case HURTGROUND:
		break;

	case DOORBREAK:
		break;
	}


}








int CPlayer::OnCollision(CObj* _target, DIR _dir)
{

	return 0;
}



void CPlayer::Set_Angle()
{

}

