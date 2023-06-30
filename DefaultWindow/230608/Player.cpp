#include "stdafx.h"
#include "Player.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "SceneManager.h"
#include "BmpMgr.h"
#include "BitMap.h"
#include "CameraComponent.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "ObjFactory.h"
#include "CameraComponent.h"



CPlayer::CPlayer()
{
	////////////////
	EDITMODE = false;
	CAMERAMODE = false;
	////////////////

	m_Type = PLAYER;
	m_Collider_type = RECTANGLE;
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
	m_fAccel = 0.1f;
	m_fWallSpeed = 1.f;
	m_fSpeed = 3.f;

	m_WallJump = false;
	
	for (int i = 0; i < DIR_END; ++i)
		m_DirCheck[i] = false;

	InitImage();
}

void CPlayer::Update(void)
{
	//카메라 업데이트
	CCharacter::Update();

	//키값 판단
	Key_Input();
	
	//EDIT모드 아닐때
	if (!EDITMODE)
	{
		//중력 적용 + 점프
		Jump();

		//상태 업데이트 (애니메이션 전환)
		StateUpdate();
	}
	
	//AttackAngle 업데이트
	AttackAngleUpdate();
	
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
		
		fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();
		ptMouse.x = ((float)ptMouse.x + ((float)cameraPos.x - WINCX / 2));
		ptMouse.y = ((float)ptMouse.y + ((float)cameraPos.y - WINCY / 2));


		_stprintf_s(text, L"[Mouse] x : %f \t y : %f", (float)ptMouse.x, (float)ptMouse.y);
		TextOutW(hDC, 0, 20, text, lstrlen(text));

		_stprintf_s(text, L"[AttackAngle] %f", m_fAttackAngle);
		TextOutW(hDC, 0, 40, text, lstrlen(text));
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
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 70;
	TempFrame.iFrameSizeY = 70;

	m_FrameMap.insert({ IDLE, TempFrame });

	TempFrame.AnimKey = L"Player_RUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 88;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ RUN, TempFrame });

	TempFrame.AnimKey = L"Player_IDLE_TO_RUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 84;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ IDLE_TO_RUN, TempFrame });


	TempFrame.AnimKey = L"Player_RUN_TO_IDLE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 4;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 80;
	TempFrame.iFrameSizeY = 70;

	m_FrameMap.insert({ RUN_TO_IDLE, TempFrame });

	TempFrame.AnimKey = L"Player_ATTACK";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 120;
	TempFrame.iFrameSizeY = 82;

	m_FrameMap.insert({ ATTACK, TempFrame });

	TempFrame.AnimKey = L"Player_JUMP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 60;
	TempFrame.iFrameSizeY = 84;

	m_FrameMap.insert({ JUMP, TempFrame });
	m_FrameMap.insert({ JUMP_WALL, TempFrame });

	TempFrame.AnimKey = L"Player_FALL";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 82;
	TempFrame.iFrameSizeY = 91;

	m_FrameMap.insert({ FALL, TempFrame });
	m_FrameMap.insert({ FALL_WALL, TempFrame });
	//
	TempFrame.AnimKey = L"Player_FLIP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 10;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 96;
	TempFrame.iFrameSizeY = 88;

	m_FrameMap.insert({ FLIP, TempFrame });

	TempFrame.AnimKey = L"Player_ROLL";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 94;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ ROLL, TempFrame });


	TempFrame.AnimKey = L"Player_GRAB_WALL";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 78;
	TempFrame.iFrameSizeY = 100;

	m_FrameMap.insert({ GRAB_WALL, TempFrame });


	TempFrame.AnimKey = L"Player_HURTFLY";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 100;
	TempFrame.iFrameSizeY = 66;

	m_FrameMap.insert({ HURTFLY, TempFrame });


	TempFrame.AnimKey = L"Player_HURTGROUND";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 5;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 114;
	TempFrame.iFrameSizeY = 46;

	m_FrameMap.insert({ HURTGROUND, TempFrame });


	TempFrame.AnimKey = L"Player_DOORBREAK";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 100;
	TempFrame.iFrameSizeY = 88;

	m_FrameMap.insert({ DOORBREAK, TempFrame });
}



void CPlayer::Key_Input(void)
{
	if (!EDITMODE && !CAMERAMODE)
	{
		m_tInfo.fPrevX = m_tInfo.fX;

		//if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		//{
		//	if (!m_DirCheck[LEFT])
		//	{
		//		if (m_State == IDLE)
		//			m_State = IDLE_TO_RUN;
		//		Set_FrontAngle(PI);
		//		m_tInfo.fX -= m_fSpeed;
		//		m_fSpeed += m_fAccel;
		//		if (m_fSpeed >= 8.f)
		//			m_fSpeed = 8.f;
		//	}a  da  d  ad    ad  ad  ad

		//}

		//if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		//{
		//	if (!m_DirCheck[RIGHT])
		//	{
		//		if (m_State == IDLE)
		//			m_State = IDLE_TO_RUN;
		//		Set_FrontAngle(0);
		//		m_tInfo.fX += m_fSpeed;
		//		m_fSpeed += m_fAccel;
		//		if (m_fSpeed >= 8.f)
		//			m_fSpeed = 8.f;
		//	}

		//}


		//if (CKeyMgr::Get_Instance()->Key_Down('A') || CKeyMgr::Get_Instance()->Key_Down('D'))
		//{
		//	if (m_State == IDLE || m_State == IDLE_TO_RUN)
		//		m_State = IDLE_TO_RUN;
		//	else if (m_State != JUMP && m_State != ATTACK && m_State != FALL)
		//		m_State = RUN;
		//}
		//if (!CKeyMgr::Get_Instance()->Key_Pressing('A') && !CKeyMgr::Get_Instance()->Key_Pressing('D') && !CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		//{
		//	if (m_State == RUN || m_State == RUN_TO_IDLE)
		//		m_State = RUN_TO_IDLE;
		//	else if (m_State != JUMP && m_State != ATTACK && m_State != FALL)
		//	{
		//		m_State = IDLE;
		//	}

		//}

		//if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		//{
		//	m_State = ATTACK;
		//}


		//if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		//{
		//	if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
		//	{
		//		m_tInfo.fY -= -10.0f;
		//	}
		//	else
		//	{
		//		if (m_WallJump)
		//		{
		//			if (m_fFrontAngle == PI)
		//			{
		//				m_fFrontAngle == 0;
		//				m_bJump = true;
		//				//m_State = JUMP;
		//				m_fSpeed_Vertical = 20.f;
		//				m_tInfo.fX += 100.f;
		//				m_fSpeed += m_fAccel;
		//			}
		//			else
		//			{
		//				m_fFrontAngle == PI;
		//				m_bJump = true;
		//				//m_State = JUMP;
		//				m_fSpeed_Vertical = 20.f;
		//				m_tInfo.fX -= 100.f;
		//				m_fSpeed += m_fAccel;
		//			}
		//		}
		//		

		//		if (0 < m_iAdditionJump_Count--)
		//		{
		//			m_fSpeed_Vertical = 10.f;
		//			m_bJump = true;
		//			//m_State = JUMP;
		//		}
		//	}
		//}

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT))
		{
			g_FrameLimit = 300;
		}
		else
		{
			g_FrameLimit = ORIGIN_FRAME;
		}
			

	}
	

	


	/*******************************************************/
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		EDITMODE = !EDITMODE;

		if (EDITMODE)
			m_fSpeed = 10.f;
		else
			m_fSpeed = 5.f;	
	}	
	
	if (EDITMODE)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		{
			m_tInfo.fY -= m_fSpeed;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		{
			m_tInfo.fX -= m_fSpeed;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		{
			m_tInfo.fY += m_fSpeed;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		{
			m_tInfo.fX += m_fSpeed;
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
			//m_State = RUN;
		}
		else
		{
			if (m_State != GRAB_WALL)
			{
				m_tInfo.fY -= m_fSpeed_Vertical;
				m_fSpeed_Vertical -= (0.034f * G);

				/*if (m_fSpeed_Vertical < 0)
					m_State = FALL;*/
			}
			else
			{
				if (m_fSpeed_Vertical > 0)
					m_fSpeed_Vertical = 0;
				m_tInfo.fY -= m_fSpeed_Vertical;
				m_fSpeed_Vertical -= (0.034f * G * 0.1f);
			}
		}
	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
		m_fSpeed_Vertical = 0.f;
	}
	

}

/////////////////////////////////////////////////////////////////

//각 객체의 해당 애니메이션 키값 넣어주기
void CPlayer::StateUpdate()
{

	if (m_PrevState != m_State)
	{
		m_FrameMap[m_State].iFrameStart = 0;
		m_FrameMap[m_State].dwTime = GetTickCount64();

		m_PrevState = m_State;
	}


	if (m_fFrontAngle == 0)
	{
		m_FrameMap[m_State].iMotion = 0;
	}
	else if (m_fFrontAngle == PI)
	{
		m_FrameMap[m_State].iMotion = 1;
	}
		
	switch (m_State)
	{
	case IDLE:
		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Pressing('D'))
		{
			m_State = IDLE_TO_RUN;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
			{
				m_tInfo.fY -= -10.0f;
			}
			else
			{
				if (0 < m_iAdditionJump_Count--)
				{
					m_fSpeed_Vertical = 10.f;
					m_bJump = true;
					m_State = JUMP;
				}
			}
		}
		
		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		break;

	case IDLE_TO_RUN:
		
		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}
		}
		
		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}
		}

		if (!CKeyMgr::Get_Instance()->Key_Pressing('A') && !CKeyMgr::Get_Instance()->Key_Pressing('D') && !CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
			m_State = RUN_TO_IDLE;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
		}

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			m_State = RUN;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
			{
				m_tInfo.fY -= -10.0f;
			}
			else
			{
				if (0 < m_iAdditionJump_Count--)
				{
					m_fSpeed_Vertical = 10.f;
					m_bJump = true;
					m_State = JUMP;
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;


		break;

	case RUN:

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}
		}

		if (!CKeyMgr::Get_Instance()->Key_Pressing('A') && !CKeyMgr::Get_Instance()->Key_Pressing('D') && !CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
				m_State = RUN_TO_IDLE;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
			{
				m_tInfo.fY -= -10.0f;
			}
			else
			{
				if (0 < m_iAdditionJump_Count--)
				{
					m_fSpeed_Vertical = 10.f;
					m_bJump = true;
					m_State = JUMP;
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		break;

	case RUN_TO_IDLE:

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
			{
				m_tInfo.fY -= -10.0f;
			}
			else
			{
				if (0 < m_iAdditionJump_Count--)
				{
					m_fSpeed_Vertical = 10.f;
					m_bJump = true;
					m_State = JUMP;
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;


		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			m_State = IDLE;
		
		m_fSpeed -= m_fAccel;
		if (m_fSpeed <= 5.f)
			m_fSpeed = 5.f;
		
		break;

	case ATTACK:

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			m_State = RUN;
		break;

	case JUMP:

   		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
			{
				m_tInfo.fY -= -10.0f;
			}
			else
			{
				if (0 < m_iAdditionJump_Count--)
				{
					m_fSpeed_Vertical = 10.f;
					m_bJump = true;
					m_State = JUMP;
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		if (!m_bJump)
			m_State = RUN;

		if (m_WallJump && m_fSpeed_Vertical < 8.f )
		{
			if ((CKeyMgr::Get_Instance()->Key_Pressing('D')))
			{
				if (m_fFrontAngle == 0)
				{
					m_State = GRAB_WALL;
					m_fSpeed_Vertical = 0.f;
				}
			}
			else if ((CKeyMgr::Get_Instance()->Key_Pressing('A')))
			{
				if (m_fFrontAngle == PI)
				{
					m_State = GRAB_WALL;
					m_tInfo.fX -= 34.f;
					m_fSpeed_Vertical = 0.f;
				}
			}
		}


		break;

	case FALL:

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= 8.f)
					m_fSpeed = 8.f;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('S') && !m_bJump)
			{
				m_tInfo.fY -= -10.0f;
			}
			else
			{
				if (0 < m_iAdditionJump_Count--)
				{
					m_fSpeed_Vertical = 10.f;
					m_bJump = true;
					m_State = JUMP;
				}
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
		}

		if (!m_bJump)
		{
			m_State = RUN;
		}
			
		if (m_WallJump && m_fSpeed_Vertical < 8.f)
		{
			if ((CKeyMgr::Get_Instance()->Key_Pressing('D')))
			{
				if (m_fFrontAngle == 0)
				{
					m_State = GRAB_WALL;
					m_fSpeed_Vertical = 0.f;
				}
			}
			else if ((CKeyMgr::Get_Instance()->Key_Pressing('A')))
			{
				if (m_fFrontAngle == PI)
				{
					m_State = GRAB_WALL;
					m_tInfo.fX -= 34.f;
					m_fSpeed_Vertical = 0.f;
				}
			}
		}

		break;

	case GRAB_WALL:

		if (!m_bJump)
		{
			m_State = RUN;
			if (m_fFrontAngle == PI)
				m_tInfo.fX += 34.f;
		}
		
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (m_fFrontAngle == PI)
				m_fFrontAngle = 0;
			else if (m_fFrontAngle == 0)
				m_fFrontAngle = PI;

			m_fSpeed_Vertical = 10.f;
			m_bJump = true;
			m_WallJump = false;
			m_State = JUMP_WALL;
		}

		break;

	case JUMP_WALL:

		if (!m_bJump)
		{
			m_State = RUN;
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL_WALL;


		if (m_WallJump)
		{
			m_State = GRAB_WALL;
			m_fSpeed_Vertical = 0.f;
			if (m_fFrontAngle == PI)
				m_tInfo.fX -= 34.f;
		}
		else
		{
			m_tInfo.fX += cos(m_fFrontAngle) * m_fWallSpeed;
			m_fWallSpeed += m_fAccel;
			if (m_fWallSpeed >= 8.f)
				m_fWallSpeed = 8.f;
		}

		break;
	
	case FALL_WALL:
		if (!m_bJump)
		{
			m_State = RUN;
		}

		if (m_WallJump)
		{
			m_State = GRAB_WALL;
			m_fSpeed_Vertical = 0.f;
			if (m_fFrontAngle == PI)
				m_tInfo.fX -= 34.f;
		}
		else
		{
			m_tInfo.fX += cos(m_fFrontAngle) * m_fWallSpeed;
			m_fWallSpeed += m_fAccel;
			if (m_fWallSpeed >= 8.f)
				m_fWallSpeed = 8.f;
		}

		break;

	case ROLL:
		break;

	case FLIP:
		break;

	case HURTFLY:
		break;

	case HURTGROUND:
		break;

	case DOORBREAK:
		break;
	}

	if(m_State != JUMP_WALL && m_State != FALL_WALL)
		m_fWallSpeed = 1.f;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_CONTROL))
	{
		CAMERAMODE = true;
		m_State = IDLE;
	}
	else
	{
		CAMERAMODE = false;
	}

	


}

void CPlayer::AttackAngleUpdate()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);	// 마우스 위치 값을 얻어오는 함수

	ScreenToClient(g_hWnd, &ptMouse); // 스크린 상의 좌표를 우리가 생성한 창 좌표로 변환

	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();
	ptMouse.x = ((float)ptMouse.x + ((float)cameraPos.x - WINCX / 2));
	ptMouse.y = ((float)ptMouse.y + ((float)cameraPos.y - WINCY / 2));

	m_fAttackAngle = atan2(ptMouse.y - m_tInfo.fY, ptMouse.x - m_tInfo.fX);
}









int CPlayer::OnCollision(CObj* _target, DIR _dir)
{
	if (_target->Get_Type() == WALL)
	{
		m_WallJump = true;
		m_DirCheck[_dir] = true;
		/*
		if (_dir == RIGHT)
			m_tInfo.fX -= 10.f;*/
	}
	return 0;
}

int CPlayer::OutCollision(CObj* _target)
{

	if (_target->Get_Type() == WALL)
	{
		m_WallJump = false;
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
	}
		
	return 0;
}




