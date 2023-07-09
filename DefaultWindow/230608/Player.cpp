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
#include "Item.h"
#include "UIMgr.h"
#include "SoundMgr.h"
#include "Bullet.h"
#include "Fan.h"
#include "PlayerEffect.h"
#include "PlayerShadow.h"
#include "BloodEffect.h"

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
	
	m_tInfo = { 600.f, 900.f, 49.f * SMALL, 74.f * SMALL };
	m_fJump_Power = 20.f;
	m_fJump_Angle = PI / 3;
	m_iAdditionJump_MaxCount = 3;
	m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
	m_fAccel = 0.2f;
	m_fWallSpeed = 1.f;
	m_fSpeed = 7.f;
	m_fFixAttackAngle = m_fAttackAngle;
	m_WallJump = false;
	m_PlayerSoundCh = 0;
	m_SlowRun = 100;
	m_BatteryCount = 9;
	m_BatteryTime = 0;
	m_HurtOn = false;
	m_Levitation = false;
	m_DustEffectOn = false;
	
	m_AttackCInfo = { m_tInfo.fX, m_tInfo.fY, 70.f, 70.f };
	m_AttackCDistance = 50.f;

	m_ItemState = ITEM_NONE;
	

	InitImage();
}

void CPlayer::Update(void)
{
	//카메라 업데이트
	CCharacter::Update();

	//키값 판단
	Key_Input();
	
	SlowMotionUpdate();

	//EDIT모드 아닐때
	if (!EDITMODE)
	{
		//중력 적용 + 점프
		Jump();
		BatteryChange();

		//상태 업데이트 (애니메이션 전환)
		StateUpdate();
		ShadowEffect();
	}
	
	//AttackAngle 업데이트
	AttackAngleUpdate();
	
	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
	CObj::UpdateAttackCollide();

	CObj::Move_Frame();
}

void CPlayer::LateUpdate(void)
{
}

void CPlayer::Render(HDC hDC)
{
	//모든 캐릭터,스킬에 필요
	CCharacter::Render(hDC);

	if (g_BossDead)
		CObj::FrameRenderToBlackWhite(hDC);
	else
		CObj::FrameRender(hDC);
	
	CObj::CollideRender(hDC);

	//플레이어만 필요
	if (EDITMODE)
	{
		WCHAR text[100];
		_stprintf_s(text, L"[Player] x : %f \t y : %f", m_tInfo.fX, m_tInfo.fY);
		TextOutW(hDC, 0, 40, text, lstrlen(text));

		POINT	ptMouse{};

		GetCursorPos(&ptMouse);	// 마우스 위치 값을 얻어오는 함수

		ScreenToClient(g_hWnd, &ptMouse); // 스크린 상의 좌표를 우리가 생성한 창 좌표로 변환
		
		fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();
		ptMouse.x = ((float)ptMouse.x + ((float)cameraPos.x - WINCX / 2));
		ptMouse.y = ((float)ptMouse.y + ((float)cameraPos.y - WINCY / 2));


		_stprintf_s(text, L"[Mouse] x : %f \t y : %f", (float)ptMouse.x, (float)ptMouse.y);
		TextOutW(hDC, 0, 60, text, lstrlen(text));

		_stprintf_s(text, L"[AttackAngle] %f", m_fAttackAngle);
		TextOutW(hDC, 0, 80, text, lstrlen(text));
	}
}

void CPlayer::Release(void)
{
	//Safe_Delete< CameraComponent*>(camera);
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/player_rollStart_1x2.png", L"Player_ROLLSTART");
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
	TempFrame.dwSpeed = 30;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 120;
	TempFrame.iFrameSizeY = 82;

	m_FrameMap.insert({ ATTACK, TempFrame });
	m_FrameMap.insert({ ATTACK_WALL, TempFrame });

	TempFrame.AnimKey = L"Player_JUMP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 60;
	TempFrame.iFrameSizeY = 84;

	m_FrameMap.insert({ JUMP, TempFrame });

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
	m_FrameMap.insert({ FALL_ATTACK, TempFrame });
	//
	TempFrame.AnimKey = L"Player_FLIP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 10;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 96;
	TempFrame.iFrameSizeY = 88;

	//m_FrameMap.insert({ FLIP, TempFrame });
	m_FrameMap.insert({ JUMP_WALL, TempFrame });

	TempFrame.AnimKey = L"Player_ROLL";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 94;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ ROLL, TempFrame });

	TempFrame.AnimKey = L"Player_ROLLSTART";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 94;
	TempFrame.iFrameSizeY = 64;

	m_FrameMap.insert({ ROLLSTART, TempFrame });


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

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SHIFT))
		{
			g_SlowMotion = true;
			CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Enter.wav", SOUND_SLOW, SOUND_VOL3);
			CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_FOOTSTEP, 0.5f);
			CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_PLAYER, 0.5f);
			CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_PLAYER2, 0.5f);
			m_SlowRun =400;
		}
		else if (CKeyMgr::Get_Instance()->Key_Up(VK_SHIFT))
		{
			if (g_SlowMotion || g_BossDead)
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW);
				CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Exit.wav", SOUND_SLOW, SOUND_VOL2);
			}
			g_SlowMotion = false;
			/*CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_FOOTSTEP, 2.f);
			CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_PLAYER, 2.f);
			CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_PLAYER2, 2.f);*/
			m_SlowRun = 100;
		}
	}
	
	if (m_ItemState != ITEM_NONE)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			UseItem();
			PlayerPlaySound(L"player_throw.wav");
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

	if (0.f < fY - m_tInfo.fY && !m_Levitation)
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
			if (g_SlowMotion || g_BossDead)
			{
				if (g_SlowJumpTime + 80 < GetTickCount64())
				{
					if (m_State != GRAB_WALL)
					{
						m_tInfo.fY -= m_fSpeed_Vertical;

						if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 1.6f;
						else
							m_fSpeed_Vertical -= (0.034f * G) * 2.0f;
					}
					else
					{
						if (m_fSpeed_Vertical < -5.f)
							m_fSpeed_Vertical = -5.f;
						m_tInfo.fY -= m_fSpeed_Vertical;

						if (-1.f < m_fSpeed_Vertical && 1.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 0.2f;
						else if (-4.f < m_fSpeed_Vertical && 4.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 0.4f;
						else
							m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
					}
					g_SlowJumpTime = GetTickCount64();
				}
				
			}
			else
			{
				if (m_State != GRAB_WALL)
				{
					m_tInfo.fY -= m_fSpeed_Vertical;

					if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
						m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
					else
						m_fSpeed_Vertical -= (0.034f * G) * 1.2f;
				}
				else
				{
					if (m_fSpeed_Vertical < -5.f)
						m_fSpeed_Vertical = -5.f;
					m_tInfo.fY -= m_fSpeed_Vertical;

					if (-1.f < m_fSpeed_Vertical && 1.f > m_fSpeed_Vertical)
						m_fSpeed_Vertical -= (0.034f * G) * 0.05f;
					else
						m_fSpeed_Vertical -= (0.034f * G) * 0.8f;
				}
			}
			
		}
	}
	else if (bLineCol && !m_Levitation)
	{
		m_tInfo.fY = fY;
		m_fSpeed_Vertical = 0.f;
	}
	

}

/////////////////////////////////////////////////////////////////

//각 객체의 해당 애니메이션 키값 넣어주기
void CPlayer::StateUpdate()
{
	
	if (m_fFrontAngle == 0)
	{
		m_FrameMap[m_State].iMotion = 0;
	}
	else if (m_fFrontAngle == PI)
	{
		m_FrameMap[m_State].iMotion = 1;
	}

	if (m_PrevState != m_State)
	{
		m_FrameMap[m_State].iFrameStart = 0;
		m_FrameMap[m_State].dwTime = GetTickCount64();
		
		//이펙트 생성
		StateChangeEffect();
		m_PrevFrame = -1;

		m_PrevState = m_State;
	}

	switch (m_State)
	{
	case IDLE:
		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Pressing('D'))
		{
			m_State = IDLE_TO_RUN;

			PlayerPlaySound(L"player_prerun.wav");
			m_DustEffectOn = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			m_State = ROLLSTART;


		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			//JumpEffect
			{
				CObj* Temp = CObjFactory<CPlayerEffect>::Create();
				Temp->Set_State(PLAER_EFFECT_JUMPDUST);
				Temp->Set_AttackAngle(0);
				Temp->Set_FrontAngle(m_fFrontAngle);
				Temp->Set_Pos(m_tInfo.fX, m_tInfo.fY - 25.f);
				CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			}

			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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
					PlayerPlaySound(L"player_jump.wav");
				}
			}
		}
		
		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}

		break;

	case IDLE_TO_RUN:
		
		//RunEffect
		if(m_FrameMap[m_State].iFrameStart == 0 && !m_DustEffectOn)
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAYER_EFFECT_RUNDUST);
			Temp->Set_AttackAngle(0);
			Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 20.f, m_tInfo.fY + 5.f);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			m_DustEffectOn = true;
		}
		else if (m_FrameMap[m_State].iFrameStart == 1 && m_DustEffectOn)
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAYER_EFFECT_RUNDUST);
			Temp->Set_AttackAngle(0);
			Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 10.f, m_tInfo.fY + 10.f);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			m_DustEffectOn = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}
		}
		
		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			m_State = ROLLSTART;

		if (!CKeyMgr::Get_Instance()->Key_Pressing('A') && !CKeyMgr::Get_Instance()->Key_Pressing('D') && !CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
			m_State = RUN_TO_IDLE;
		}

		

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			//JumpEffect
			{
				CObj* Temp = CObjFactory<CPlayerEffect>::Create();
				Temp->Set_State(PLAER_EFFECT_JUMPDUST);
				Temp->Set_AttackAngle(0);
				Temp->Set_FrontAngle(m_fFrontAngle);
				Temp->Set_Pos(m_tInfo.fX, m_tInfo.fY - 25.f);
				CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			}


			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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

					PlayerPlaySound(L"player_jump.wav");
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}

		break;

	case RUN:
		
		if (m_RunningSoundTime + m_SlowRun < GetTickCount64())
		{
			CSoundMgr::Get_Instance()->PlaySound(L"player_running_4.wav", SOUND_FOOTSTEP, SOUND_VOL3);
			m_RunningSoundTime = GetTickCount64();
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			m_State = ROLLSTART;

		if (!CKeyMgr::Get_Instance()->Key_Pressing('A') && !CKeyMgr::Get_Instance()->Key_Pressing('D') && !CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
				m_State = RUN_TO_IDLE;
		}


		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			//JumpEffect
			{
				CObj* Temp = CObjFactory<CPlayerEffect>::Create();
				Temp->Set_State(PLAER_EFFECT_JUMPDUST);
				Temp->Set_AttackAngle(0);
				Temp->Set_FrontAngle(m_fFrontAngle);
				Temp->Set_Pos(m_tInfo.fX, m_tInfo.fY - 25.f);
				CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			}


			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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


					PlayerPlaySound(L"player_jump.wav");
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}

		break;

	case RUN_TO_IDLE:

		
		if (m_fSpeed_Vertical < 0)
			m_State = FALL;


		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			m_State = IDLE;

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{  
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}
		}


		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			m_State = ROLLSTART;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			//JumpEffect
			{
				CObj* Temp = CObjFactory<CPlayerEffect>::Create();
				Temp->Set_State(PLAER_EFFECT_JUMPDUST);
				Temp->Set_AttackAngle(0);
				Temp->Set_FrontAngle(m_fFrontAngle);
				Temp->Set_Pos(m_tInfo.fX, m_tInfo.fY - 25.f);
				CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			}

			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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

					PlayerPlaySound(L"player_jump.wav");
				}
			}
		}

		
		m_fSpeed -= m_fAccel;
		if (m_fSpeed <= 5.f)
			m_fSpeed = 5.f;
		
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();
			
			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}

		break;

	case ATTACK:

		Parring();
		Attack();
		
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
			{
				m_fFixAttackAngle = m_fAttackAngle;
				if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
				{
					m_fFrontAngle = 0;
					if (!m_DirCheck[RIGHT])
						m_tInfo.fX += 50.f;
				}
				else
				{
					m_fFrontAngle = PI;
					if (!m_DirCheck[LEFT])
						m_tInfo.fX -= 10.f;
				}
				m_fSpeed_Vertical = 2.f;
				m_FrameMap[m_State].iFrameStart = 0;
				m_FrameMap[m_State].dwTime = GetTickCount64();
			}
			else
				m_State = FALL_ATTACK;
		}

		if (!m_WallJump)
		{
			if (m_fFrontAngle == PI) {
				if (!m_DirCheck[LEFT])
				{
					m_tInfo.fX += (m_fSpeed + 2.5f) * cos(m_fFixAttackAngle);
					m_fSpeed += m_fAccel;
					if (m_fSpeed >= SPEED_LIMIT)
						m_fSpeed = SPEED_LIMIT;
				}
			}
			else
			{
				if (!m_DirCheck[RIGHT])
				{
					m_tInfo.fX += (m_fSpeed + 2.5f) * cos(m_fFixAttackAngle);
					m_fSpeed += m_fAccel;
					if (m_fSpeed >= SPEED_LIMIT)
						m_fSpeed = SPEED_LIMIT;
				}
			}
				
		}

		
		break;

	case ATTACK_WALL:
		
		if (m_WallJump)
		{
			if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			{
				if ((CKeyMgr::Get_Instance()->Key_Pressing('D')))
				{
					if (m_fFrontAngle == 0)
					{
						if (m_DirCheck[RIGHT])
						{
							m_State = GRAB_WALL;
							if (m_fSpeed_Vertical < -1.f)
								m_fSpeed_Vertical = 0.f;
						}

					}
				}
				else if ((CKeyMgr::Get_Instance()->Key_Pressing('A')))
				{
					if (m_fFrontAngle == PI)
					{
						if (m_DirCheck[LEFT])
						{
							m_State = GRAB_WALL; 
							if (m_fSpeed_Vertical < - 1.f)
								m_fSpeed_Vertical = 0.f;
						}
					}
				}
				else
					m_State = FALL;
			}
		}
		else
		{
			if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
				m_State = FALL;
		}
		break;

	case JUMP:
		
   		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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

					PlayerPlaySound(L"player_jump.wav");
				}
			}			
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		if (!m_bJump)
		{
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;
		}

		if (m_WallJump && m_fSpeed_Vertical < 8.f )
		{
			if ((CKeyMgr::Get_Instance()->Key_Pressing('D')))
			{
				if (m_fFrontAngle == 0)
				{
					if (!m_DirCheck[LEFT])
					{
						m_State = GRAB_WALL;
						m_fSpeed_Vertical -= 3.f;

						if (m_fSpeed_Vertical < -1.f)
							m_fSpeed_Vertical = 0.f;
					}
				}
			}
			else if ((CKeyMgr::Get_Instance()->Key_Pressing('A')))
			{
				if (m_fFrontAngle == PI)
				{
					if (!m_DirCheck[RIGHT])
					{
						m_State = GRAB_WALL;
						m_tInfo.fX -= 34.f * SMALL;
						m_fSpeed_Vertical -= 3.f;

						if (m_fSpeed_Vertical < -1.f)
							m_fSpeed_Vertical = 0.f;
					}
				}
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}


		break;

	case FALL_ATTACK:

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 2.f;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
			break;
		}
		
		if (!m_WallJump)
		{
			if (m_fFrontAngle == PI) {
				if (!m_DirCheck[LEFT])
				{
					m_tInfo.fX += (2.f) * cos(m_fFixAttackAngle);
				}
			}
			else
			{
				if (!m_DirCheck[RIGHT])
				{
					m_tInfo.fX += (2.f) * cos(m_fFixAttackAngle);
				}
			}
			
		}
		

	case FALL:

		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (!m_DirCheck[LEFT])
			{
				Set_FrontAngle(PI);
				m_tInfo.fX -= m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_DirCheck[RIGHT])
			{
				Set_FrontAngle(0);
				m_tInfo.fX += m_fSpeed;
				m_fSpeed += m_fAccel;
				if (m_fSpeed >= SPEED_LIMIT)
					m_fSpeed = SPEED_LIMIT;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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

					PlayerPlaySound(L"player_jump.wav");
				}
			}
		}

		if (!m_bJump)
		{
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;
		}
			
		if (m_WallJump && m_fSpeed_Vertical < 8.f)
		{
			if ((CKeyMgr::Get_Instance()->Key_Pressing('D')))
			{
				if (m_fFrontAngle == 0)
				{
					if (m_DirCheck[RIGHT])
					{
						m_State = GRAB_WALL;
						m_fSpeed_Vertical -= 3.f;

						if (m_fSpeed_Vertical < -1.f)
							m_fSpeed_Vertical = 0.f;
					}
				}
			}
			else if ((CKeyMgr::Get_Instance()->Key_Pressing('A')))
			{
				if (m_fFrontAngle == PI)
				{
					if (m_DirCheck[LEFT])
					{
						m_State = GRAB_WALL;
						m_tInfo.fX -= 34.f * SMALL;
						m_fSpeed_Vertical -= 3.f;

						if (m_fSpeed_Vertical < -1.f)
							m_fSpeed_Vertical = 0.f;
					}
				}
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}

		break;



	case GRAB_WALL:

		
		CSoundMgr::Get_Instance()->PlaySound(L"wallslide.wav", SOUND_FOOTSTEP, SOUND_VOL3);
		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_FOOTSTEP,abs( 0.3f * m_fSpeed_Vertical ));
		
		//RunEffect
		if (m_FrameMap[m_State].iFrameStart == 0 && !m_DustEffectOn && abs(m_fSpeed_Vertical) >= 0.8f)
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAYER_EFFECT_RUNDUST);
			Temp->Set_AttackAngle(0);
			Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) - 20.f, m_tInfo.fY + 10.f);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			m_DustEffectOn = true;
		}
		else if (m_FrameMap[m_State].iFrameStart == 3 && m_DustEffectOn && abs(m_fSpeed_Vertical) >= 0.8f)
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAYER_EFFECT_RUNDUST);
			Temp->Set_AttackAngle(0);
			Temp->Set_Pos(m_tInfo.fX - cos(m_fFrontAngle) * 10.f, m_tInfo.fY + 15.f);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			m_DustEffectOn = false;
		}

		if (!m_bJump)
		{
			
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;

			CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
			
			if (m_fFrontAngle == PI)
				m_tInfo.fX += 34.f * SMALL;
		}
		
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

			if (m_fFrontAngle == PI)
				m_fFrontAngle = 0;
			else if (m_fFrontAngle == 0)
				m_fFrontAngle = PI;

			m_fSpeed_Vertical = 8.f;
			m_bJump = true;
			m_WallJump = false;
			m_State = JUMP_WALL;

			CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
			PlayerPlaySound(L"player_roll.wav");
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				if (!m_DirCheck[RIGHT])
				{
					m_State = ATTACK;
					AttackSound();
					Parring();

					m_tInfo.fX += 50.f;
					m_fSpeed_Vertical = 10.f * sin(m_fAttackAngle);

					CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
				}
				else
				{
					m_State = ATTACK_WALL;
					AttackSound();

					m_fSpeed_Vertical = 10.f * sin(m_fAttackAngle);

					CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
				}
				m_fFrontAngle = 0;
			}
			else
			{
				if (!m_DirCheck[LEFT])
				{
					m_State = ATTACK;
					AttackSound();
					Parring();

					m_tInfo.fX -= 10.f;
					m_fSpeed_Vertical = 10.f * sin(m_fAttackAngle);

					CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
				}
				else
				{
					m_State = ATTACK_WALL;
					AttackSound();

					m_fSpeed_Vertical = 10.f * sin(m_fAttackAngle);

					CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
				}
				m_fFrontAngle = PI;
			}
		}

		break;

	case JUMP_WALL:

		if (!m_bJump)
		{
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL_WALL;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}
		else if (m_WallJump)
		{
			m_State = GRAB_WALL;
			m_fSpeed_Vertical -= 3.f;
			if (m_fFrontAngle == PI)
				m_tInfo.fX -= 34.f * SMALL;
			if (m_fSpeed_Vertical < -1.f)
				m_fSpeed_Vertical = 0.f;
		}
		else
		{
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[RIGHT])
				{
					m_tInfo.fX += cos(m_fFrontAngle) * m_fWallSpeed;
					m_fWallSpeed += m_fAccel;
					if (m_fWallSpeed >= SPEED_LIMIT)
						m_fWallSpeed = SPEED_LIMIT;
				}
			}
			else
			{
				if (!m_DirCheck[LEFT])
				{
					m_tInfo.fX += cos(m_fFrontAngle) * m_fWallSpeed;
					m_fWallSpeed += m_fAccel;
					if (m_fWallSpeed >= SPEED_LIMIT)
						m_fWallSpeed = SPEED_LIMIT;
				}
			}
			
		}

		break;
	
	case FALL_WALL:
		if (!m_bJump)
		{
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_State = ATTACK;
			AttackSound();
			Parring();

			m_fSpeed_Vertical = 12.f * sin(m_fAttackAngle);
			m_bJump = true;
			m_fFixAttackAngle = m_fAttackAngle;
			if ((0 <= m_fAttackAngle && 0.5f * PI >= m_fAttackAngle) || (0 > m_fAttackAngle && -0.5f * PI < m_fAttackAngle))
			{
				m_fFrontAngle = 0;
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 50.f;
			}
			else
			{
				m_fFrontAngle = PI;
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 10.f;
			}
		}
		else if (m_WallJump)
		{
			m_State = GRAB_WALL;
			m_fSpeed_Vertical -= 3.f;
			if (m_fFrontAngle == PI)
				m_tInfo.fX -= 34.f;
			if (m_fSpeed_Vertical < -1.f)
				m_fSpeed_Vertical = 0.f;
		}
		else
		{
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[RIGHT])
				{
					m_tInfo.fX += cos(m_fFrontAngle) * m_fWallSpeed;
					m_fWallSpeed += m_fAccel;
					if (m_fWallSpeed >= SPEED_LIMIT)
						m_fWallSpeed = SPEED_LIMIT;
				}
			}
			else
			{
				if (!m_DirCheck[LEFT])
				{
					m_tInfo.fX += cos(m_fFrontAngle) * m_fWallSpeed;
					m_fWallSpeed += m_fAccel;
					if (m_fWallSpeed >= SPEED_LIMIT)
						m_fWallSpeed = SPEED_LIMIT;
				}
			}
		}

		

		break;

	case ROLLSTART:
		if (CKeyMgr::Get_Instance()->Key_Pressing('A') || CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			Set_FrontAngle(PI);
			m_State = ROLL;


			PlayerPlaySound(L"player_roll.wav");
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing('D') || CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			Set_FrontAngle(0);
			m_State = ROLL;

			PlayerPlaySound(L"player_roll.wav");
		}

		if (CKeyMgr::Get_Instance()->Key_Up('S'))
		{
			m_RunningSoundTime = GetTickCount64();
			m_State = RUN;
		}


		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			if (g_SlowMotion || g_BossDead)
				g_SlowJumpTime = GetTickCount64();
			else
				g_SlowJumpTime = 0;

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

					PlayerPlaySound(L"player_jump.wav");
				}
			}
		}

		if (m_fSpeed_Vertical < 0)
			m_State = FALL;

		break;

	case ROLL:
		if (m_FrameMap[m_State].iFrameStart < m_FrameMap[m_State].iFrameEnd)
		{
			if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[LEFT])
				{
					m_tInfo.fX -= m_fSpeed;
					m_fSpeed += m_fAccel + 5.f;
					if (m_fSpeed >= SPEED_LIMIT + 5.f)
						m_fSpeed = SPEED_LIMIT + 5.f;
				}
			}
			else
			{
				if (!m_DirCheck[RIGHT])
				{
					m_tInfo.fX += m_fSpeed;
					m_fSpeed += m_fAccel + 5.f;
					if (m_fSpeed >= SPEED_LIMIT + 5.f)
						m_fSpeed = SPEED_LIMIT + 5.f;
				}
			}
		}
		else
			m_State = RUN_TO_IDLE;


		if (m_fSpeed_Vertical < 0)
			m_State = FALL;
					
				
	break;


	case HURTFLY:
		
		m_WallJump = false;

		if (!m_HurtOn)
		{
			m_bJump = true;
			m_fSpeed_Vertical = 4.f;
			m_HurtOn = true;
		}

		if (m_bJump)
		{
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 5.f;
			}
			else if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 5.f;
			}
		}
		else if (!m_bJump)
		{
			m_State = HURTGROUND;
			m_HurtOn = false;
			m_Levitation = true;
			m_tInfo.fY += 30.f;
		}
		break;

	case HURTGROUND:

		m_WallJump = false;

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			m_FrameMap[m_State].iFrameStart = m_FrameMap[m_State].iFrameEnd - 1;
			
		if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
		{
			m_State = IDLE;
			m_Levitation = false;
			m_tInfo.fY -= 30.f;
		}
		break;

	case DOORBREAK:
		break;
	}

	if (m_fFrontAngle == 0)
	{
		m_FrameMap[m_State].iMotion = 0;
	}
	else if (m_fFrontAngle == PI)
	{
		m_FrameMap[m_State].iMotion = 1;
	}

	if(m_State != JUMP_WALL && m_State != FALL_WALL)
		m_fWallSpeed =3.f;

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

	m_fAttackAngle = (-1.f) * atan2(ptMouse.y - (m_tInfo.fY + (m_tInfo.fCY * 0.5)), ptMouse.x - m_tInfo.fX);
}

void CPlayer::UseItem()
{
	CObj* Temp = CObjFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, 30, 30);
	Temp->SetOwner(this);
	Temp->Set_AttackAngle(m_fAttackAngle);
	dynamic_cast<CItem*>(Temp)->SetThrow(true);
	dynamic_cast<CItem*>(Temp)->SetITemType(m_ItemState);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	m_ItemState = ITEM_NONE;
	CUIMgr::Get_Instance()->SetInven(L"DEFAULT");
}


void CPlayer::PlayerPlaySound(TCHAR* _name)
{
	if (m_PlayerSoundCh == 0)
		CSoundMgr::Get_Instance()->PlaySound(_name, SOUND_PLAYER, SOUND_VOL3);
	else if (m_PlayerSoundCh == 1)
		CSoundMgr::Get_Instance()->PlaySound(_name, SOUND_PLAYER2, SOUND_VOL3);
	else if (m_PlayerSoundCh == 2)
		CSoundMgr::Get_Instance()->PlaySound(_name, SOUND_PLAYER3, SOUND_VOL3);

	++m_PlayerSoundCh;
	if (m_PlayerSoundCh == 3)
		m_PlayerSoundCh = 0;
}

void CPlayer::AttackSound()
{
	int randNum = rand() % 3;
	if (randNum == 0)
		PlayerPlaySound(L"sound_player_slash_1.wav");
	else if (randNum == 1)
		PlayerPlaySound(L"sound_player_slash_2.wav");
	else
		PlayerPlaySound(L"sound_player_slash_3.wav");
}

void CPlayer::BatteryChange()
{
	if (g_SlowMotion)
	{
		if (m_BatteryTime + 1000 < GetTickCount64())
		{
			--m_BatteryCount;
			if (m_BatteryCount <= 0)
			{
				m_BatteryCount = 0;
				g_SlowMotion = false;
				CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW);
				CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Exit.wav", SOUND_SLOW, SOUND_VOL2);
				CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_FOOTSTEP, 2.f);
				CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_PLAYER, 2.f);
				CSoundMgr::Get_Instance()->SetChannelFrequency(SOUND_PLAYER2, 2.f);
				m_SlowRun = 100;
			}
			m_BatteryTime = GetTickCount64();
			CUIMgr::Get_Instance()->SetBattery(m_BatteryCount);
			
		}
	}
	else
	{
		if (m_BatteryTime == 0)
			m_BatteryTime = GetTickCount64();
		else if (m_BatteryTime + 500 < GetTickCount64())
		{
			++m_BatteryCount;
			if (m_BatteryCount >= 9)
				m_BatteryCount = 9;
			else
				m_BatteryTime = GetTickCount64();

			CUIMgr::Get_Instance()->SetBattery(m_BatteryCount);			
		}
	}

}

void CPlayer::Attack()
{
	list<CObj*> *EnemyList = CObjMgr::Get_Instance()->Get_ObjList(ENEMY);
	if (!EnemyList->empty())
	{
		for (auto& iter : *EnemyList)
		{
			if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_AttackCollide, iter->Get_Collide()))
			{
				if (iter->Get_State() != HURT && iter->Get_State() != HURTGROUND)
				{
					iter->Set_State(HURT);
					iter->Set_AttackAngle(m_fAttackAngle);
					if (iter->Get_Info()->fX < m_tInfo.fX)
						iter->Set_FrontAngle(0);
					else
						iter->Set_FrontAngle(PI);

					CSoundMgr::Get_Instance()->PlaySound(L"death_sword.wav", SOUND_EFFECT, SOUND_VOL3);

					CObj* Temp = CObjFactory<CPlayerEffect>::Create();
					Temp->Set_State(PLAYER_EFFECT_HIT);
					Temp->Set_AttackAngle(m_fAttackAngle);
					Temp->Set_FrontAngle(m_fFrontAngle);
					Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 10.f, m_tInfo.fY);
					CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
				}

			}

		}
	}
	
}

void CPlayer::Parring()
{
	list<CObj*>* BulletList = CObjMgr::Get_Instance()->Get_ObjList(BULLET);

	for (auto& iter : *BulletList)
	{
		if (iter->GetOwner() != this)
		{
			if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_AttackCollide, iter->Get_Collide()))
			{				
				CObj* Temp = CObjFactory<CBullet>::Create();
				
				INFO* iterInfo = iter->Get_Info();
				Temp->Set_Pos(iterInfo->fX, iterInfo->fY);
				Temp->SetOwner(this);
				
				if (iter->Get_FrontAngle() == PI)
					Temp->Set_FrontAngle(0);
				else
					Temp->Set_FrontAngle(PI);
				
				float TempAngle = m_fAttackAngle;
				if (TempAngle >= 2.f * PI)
					TempAngle -= 2.f * PI;
				Temp->Set_AttackAngle(TempAngle);
				iter->Set_State(DEAD);
				CObjMgr::Get_Instance()->Add_Object(BULLET, Temp);
				PlayerPlaySound(L"slash_bullet.wav");

				//ReflectEffect
				{
					CObj* Temp = CObjFactory<CPlayerEffect>::Create();
					Temp->Set_State(PLAYER_EFFECT_REFLECT);
					Temp->Set_AttackAngle(m_fAttackAngle);
					Temp->Set_FrontAngle(m_fFrontAngle);
					dynamic_cast<CPlayerEffect*> (Temp)->Set_Distance(50.f * cos(m_fAttackAngle), 50.f * sin(m_fAttackAngle));
					Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 10.f, m_tInfo.fY);
					CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
				}
			}
		}
	}

}

void CPlayer::CameraReMake()
{
	if (!_components.empty())
	{
		_components.clear();
	}
}

void CPlayer::StateChangeEffect()
{
	if (m_State == ATTACK || m_State == ATTACK_WALL)
	{
		//AttackEffect
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAYER_EFFECT_SLASH);
			Temp->Set_AttackAngle(m_fAttackAngle);
			Temp->Set_FrontAngle(m_fFrontAngle);
			dynamic_cast<CPlayerEffect*> (Temp)->Set_Distance(50.f * cos(m_fAttackAngle), 50.f * sin(m_fAttackAngle));
			Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 10.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
		}
	}
	else if (m_State == JUMP_WALL)
	{
		//JumpEffect
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAER_EFFECT_JUMPDUST);
			Temp->Set_AttackAngle(m_fFrontAngle - PI / 2.f);
			Temp->Set_FrontAngle(m_fFrontAngle);
			Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 50.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
		}
	}
	else if (m_State == RUN && (m_PrevState == FALL || m_PrevState == FALL_WALL))
	{
		//LandEffect
		{
			CObj* Temp = CObjFactory<CPlayerEffect>::Create();
			Temp->Set_State(PLAYER_EFFECT_LANDDUST);
			Temp->Set_AttackAngle(0);
			Temp->Set_Pos(m_tInfo.fX - 25.f, m_tInfo.fY + 10.f);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
		}
	}
	else if (m_State == HURTFLY)
	{
		float FlipFrontAngle;
		if (m_fFrontAngle == PI)
			FlipFrontAngle = 0;
		else
			FlipFrontAngle = PI;

		//BloodEffect
		{
			CObj* Temp = CObjFactory<CBloodEffect>::Create();
			dynamic_cast<CBloodEffect*>(Temp)->Set_RandomState();
			Temp->Set_AttackAngle(FlipFrontAngle);
			Temp->Set_FrontAngle(FlipFrontAngle);
			Temp->Set_Pos(m_tInfo.fX + cos(FlipFrontAngle) * 10.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
		}

		//BloodEffect
		{
			CObj* Temp = CObjFactory<CBloodEffect>::Create();
			Temp->Set_State(BLOOD_EFFECT_ONE);
			Temp->Set_AttackAngle(FlipFrontAngle);
			Temp->Set_FrontAngle(FlipFrontAngle);
			Temp->Set_Pos(m_tInfo.fX + cos(FlipFrontAngle) * 10.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
		}

		//BloodEffect
		{
			CObj* Temp = CObjFactory<CBloodEffect>::Create();
			Temp->Set_State(BLOOD_EFFECT_MOVE);
			Temp->Set_AttackAngle(m_fAttackAngle);
			Temp->Set_FrontAngle(m_fFrontAngle);
			Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 5.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
		}
	}
}

void CPlayer::ShadowEffect()
{
	float Distance;
	if (m_fFrontAngle == 0)
		Distance = 30.f;
	else
		Distance = 5.f;

	if (m_State == ATTACK || m_State == JUMP_WALL || m_State == FALL || m_State == ROLL || m_State == RUN)
	{
		if (m_PrevFrame != m_FrameMap[m_State].iFrameStart)
		{
			CObj* Temp = CObjFactory<CPlayerShadow>::Create();
			Temp->Set_State(m_State);
			Temp->Set_FrontAngle(m_fFrontAngle);
			Temp->Set_Pos(m_tInfo.fX - cos(m_fFrontAngle) * Distance, m_tInfo.fY - 20.f);
			Temp->Set_FrameStart(m_State, m_FrameMap[m_State].iFrameStart);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			m_PrevFrame = m_FrameMap[m_State].iFrameStart;
		}
	}
	
}




int CPlayer::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL)
	{
		if(_dir == LEFT || _dir == RIGHT)
			m_WallJump = true;
		m_DirCheck[_dir] = true;
		if (_dir == TOP)
			m_fSpeed_Vertical *= (-0.2f);
		if (_dir == BOTTOM)
			m_tInfo.fY -= 10.f;
	}
	else if (targetType == WALL)
	{
		m_DirCheck[_dir] = true;
		if (_dir == TOP)
			m_fSpeed_Vertical *= (-0.2f);
		if (_dir == BOTTOM)
			m_tInfo.fY -= 10.f;
	}
	else if (targetType == BULLET)
	{
		if (_target->GetOwner() != this)
		{
			if (m_State != HURTFLY && m_State != HURTGROUND && m_State != ROLL) //구를때 무적
			{
				m_State = HURTFLY;
				if (_dir == LEFT)
					m_fFrontAngle = PI;
				else if (_dir == RIGHT)
					m_fFrontAngle = 0;
				_target->Set_State(DEAD);
				PlayerPlaySound(L"death_bullet.wav");
				PlayerPlaySound(L"playerdie.wav");
			}
		}
	}
	else if (targetType == BOSS)
	{
		if (_target->Get_State() == BOSS_DASH)
		{
			if (m_State != HURTFLY && m_State != HURTGROUND && m_State != ROLL) //구를때 무적
			{
				m_State = HURTFLY;
				if (_dir == LEFT)
					m_fFrontAngle = PI;
				else if (_dir == RIGHT)
					m_fFrontAngle = 0;
				PlayerPlaySound(L"playerdie.wav");
			}
		}
	}
	else if (targetType == FAN)
	{
		if (dynamic_cast<CFan*>(_target)->GetAttackOn())
		{
			if (m_State != HURTFLY && m_State != HURTGROUND) //구를때 무적
			{
				m_State = HURTFLY;
				m_fFrontAngle = 0;
				PlayerPlaySound(L"playerdie.wav");
			}
		}
		else
		{
			if (m_State != HURTFLY && m_State != HURTGROUND && m_State != ROLL) //구를때 무적
			{
				m_State = HURTFLY;
				m_fFrontAngle = 0;
				PlayerPlaySound(L"playerdie.wav");
			}
		}
	}
	

	return 0;
}

int CPlayer::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL)
	{
		m_WallJump = false;
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
	}
	else if (targetType == WALL)
	{
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
	}
	
		
	return 0;
}

int CPlayer::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == ITEM)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('E') || CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
		{
			if (!dynamic_cast<CItem*> (_target)->GetThrow() && m_ItemState == ITEM_NONE)
			{
				m_ItemState = dynamic_cast<CItem*>(_target)->GetITemType();
				if (m_ItemState == SWORD)
					CUIMgr::Get_Instance()->SetInven(L"SWORD");
				else if (m_ItemState == OILBOTTLE)
					CUIMgr::Get_Instance()->SetInven(L"OILBOTTLE");
				_target->Set_State(DEAD);
				PlayerPlaySound(L"pickup.wav");
			}
		}
	}
	else if (targetType == GRABWALL)
	{
		if (m_State == GRAB_WALL)
		{
			RECT* targetRect = _target->Get_Rect();
			if (m_tRect.top <= targetRect->top)
			{
					m_fSpeed_Vertical = (-0.5f);
			}
			else
				m_WallJump = true;
		}
	}
	

	return 0;
}




