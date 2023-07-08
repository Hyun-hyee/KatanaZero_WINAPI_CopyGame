#include "stdafx.h"
#include "BOSS.h"
#include "ObjFactory.h"
#include "BmpMgr.h"
#include "LineMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "Item.h"
#include "KeyMgr.h"
#include "Bullet.h"
#include "Laser.h"
#include "ObjFactory.h"

CBoss::CBoss()
{
	m_Type = BOSS;
	m_Collider_type = RECTANGLE;
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize(void)
{
	InitImage();
	InitPatternList();

	m_State = BOSS_IDLE;

	m_fFrontAngle = PI;
	m_fSpeed = 12.f;
	m_fAccel = 0.1f;
	m_CheckCWidth = 150.f;
	m_bFollow = false;
	m_HurtOn = false;
	m_WalkTime = GetTickCount64();
	m_fFrontCWidth = 450.f;
	m_AttackOn = false;
	m_BulletOn = false;
	m_BulletHurt = false;

	m_LaserTime = 0;
	m_BulletTime = 0; //안쓰면 지우기
	m_WallJump = false;
	m_Laser = nullptr;
	m_Laser90 = false;
	m_PatternOn = false;
	m_PatternIndex = 0;
	m_Levitation = false;

	m_Life = 1;

	m_Target = CObjMgr::Get_Instance()->Get_Player();
}

void CBoss::Update(void)
{
	if(g_BossStart)
		PatternChange();
	StateUpdate();

	if (g_SlowMotion)
	{
		m_fSpeed = 0.5f;
		m_SlowTime = 5000;
	}
	else
	{
		m_SlowTime = 0;
		m_fSpeed = 12.f;
	}
		
	Jump();
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
	Update_CheckCollide();

	CObj::Move_Frame();
}

void CBoss::LateUpdate(void)
{
	if (m_Laser != nullptr)
	{
		if (m_Laser->Get_State() == DEAD)
			m_Laser = nullptr;
	}
}

void CBoss::Render(HDC hDC)
{
	CObj::FrameRender(hDC);

	//CObj::CollideRender(hDC);
	//CObj::CollideRender(hDC, m_CheckCollide);
}

void CBoss::Release(void)
{
}

void CBoss::InitImage()
{
	//IDLE
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headhunter_idle_12x2.bmp", L"BOSS_IDLE");
	
	//JUMP 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_prejump_3x2.bmp", L"BOSS_PREJUMP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_jump_1x2.bmp", L"BOSS_JUMP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_land_4x2.bmp", L"BOSS_LAND");

	//ROLL
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_roll_7x2.png", L"BOSS_ROLL");

	//DASH 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_predash_8x2.bmp",	 L"BOSS_PREDASH");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_dash_1x2.bmp",		 L"BOSS_DASH");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_dash_end_10x2.bmp",	 L"BOSS_DASHEND");

	//DEAD 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_dead_19x2.bmp",	 L"BOSS_DEAD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_diefly_4x2.png",	 L"BOSS_DIEFLY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_dieground_8x2.bmp",	 L"BOSS_DIEGROUND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_nohead_6x2.bmp", L"BOSS_NOHEAD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_nohead_stop_1x2.png", L"BOSS_NOHEAD_STOP");

	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headfly_8x2.bmp", L"BOSS_HEADFLY");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headground_8x2.bmp", L"BOSS_HEADGROUND");

	//AIM
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headhunter_aim_19x2.bmp", L"BOSS_AIM_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headhunter_aim_1x2.bmp", L"BOSS_AIM_STOP");

	//DRAWGUN
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headhunter_drawgun_8x2.bmp", L"BOSS_DRAWGUN");

	//HURT RECOVER
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headhunter_hurt_recover_10x2.bmp", L"BOSS_HURTRECOVER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headhunter_hurt_recover_13x2.bmp", L"BOSS_HURTRECOVER_FADE");
	
	//PATTERN
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_in_pattern_4x2.bmp", L"BOSS_PATTERN_IN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_out_pattern_4x2.bmp", L"BOSS_PATTERN_OUT");

	//SWEEP
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_in_sweep_4x2.bmp", L"BOSS_SWEEP_IN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_out_sweep_3x2.bmp", L"BOSS_SWEEP_OUT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_sweep_18x2.bmp", L"BOSS_SWEEP_NONFADE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_sweep_26x2.bmp", L"BOSS_SWEEP_FADE");

	//SHOOT
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_takeoutgun_7x2.bmp", L"BOSS_TAKEOUTGUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_shoot_8x2.bmp", L"BOSS_SHOOT");

	//TELEPORT
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_teleport_in_4x2.bmp", L"BOSS_TELEPORT_IN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_teleport_8x2.bmp", L"BOSS_TELEPORT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_teleport_ground_8x2.bmp", L"BOSS_TELEPORT_GROUND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_teleport_out_4x2.bmp", L"BOSS_TELEPORT_OUT");

	//WALL
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_wallgrab_3x2.bmp", L"BOSS_WALLGRAB");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_walljump_7x2.bmp", L"BOSS_WALLJUMP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_walljump_fall_7x2.png", L"BOSS_WALLJUMP_FALL");

	FRAME TempFrame;
	TempFrame.AnimKey = L"BOSS_IDLE";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 11;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 44;
	TempFrame.iFrameSizeY = 88;
	m_FrameMap.insert({ BOSS_IDLE, TempFrame });

	TempFrame.AnimKey = L"BOSS_PREJUMP"; //
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 2;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 66;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_PREJUMP, TempFrame });

	TempFrame.AnimKey = L"BOSS_JUMP"; //
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 54;
	TempFrame.iFrameSizeY = 88;
	m_FrameMap.insert({ BOSS_JUMP, TempFrame });

	TempFrame.AnimKey = L"BOSS_LAND";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 66;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_LAND, TempFrame });

	TempFrame.AnimKey = L"BOSS_ROLL";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 118;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_ROLL, TempFrame });

	TempFrame.AnimKey = L"BOSS_PREDASH";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 114;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_PREDASH, TempFrame });

	TempFrame.AnimKey = L"BOSS_DASH";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 102;
	TempFrame.iFrameSizeY = 50;
	m_FrameMap.insert({ BOSS_DASH, TempFrame });

	TempFrame.AnimKey = L"BOSS_DASHEND";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 100;
	TempFrame.iFrameSizeY = 92;
	m_FrameMap.insert({ BOSS_DASHEND, TempFrame });

	TempFrame.AnimKey = L"BOSS_DEAD";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 18;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 112;
	TempFrame.iFrameSizeY = 42;
	m_FrameMap.insert({ BOSS_DEAD, TempFrame });

	TempFrame.AnimKey = L"BOSS_DIEFLY";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 82;
	TempFrame.iFrameSizeY = 52;
	m_FrameMap.insert({ BOSS_DIEFLY, TempFrame });

	///
	TempFrame.AnimKey = L"BOSS_DIEGROUND";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 116;
	TempFrame.iFrameSizeY = 82;
	m_FrameMap.insert({ BOSS_DIEGROUND, TempFrame });

	TempFrame.AnimKey = L"BOSS_NOHEAD";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 5;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 120;
	TempFrame.iFrameSizeY = 44;
	m_FrameMap.insert({ BOSS_NOHEAD, TempFrame });

	TempFrame.AnimKey = L"BOSS_NOHEAD_STOP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 120;
	TempFrame.iFrameSizeY = 44;
	m_FrameMap.insert({ BOSS_NOHEAD_STOP, TempFrame });
	
	TempFrame.AnimKey = L"BOSS_AIM_MOVE"; //
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 18;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 78;
	TempFrame.iFrameSizeY = 104;
	m_FrameMap.insert({ BOSS_AIM_MOVE, TempFrame });

	TempFrame.AnimKey = L"BOSS_AIM_STOP";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 78;
	TempFrame.iFrameSizeY = 104;
	m_FrameMap.insert({ BOSS_AIM_STOP, TempFrame });

	TempFrame.AnimKey = L"BOSS_DRAWGUN"; //
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 76;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_DRAWGUN, TempFrame });

	///
	TempFrame.AnimKey = L"BOSS_HURTRECOVER"; //
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 140;
	TempFrame.iFrameSizeY = 106;
	m_FrameMap.insert({ BOSS_HURTRECOVER, TempFrame });

	TempFrame.AnimKey = L"BOSS_HURTRECOVER_FADE";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 12;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 140;
	TempFrame.iFrameSizeY = 106;
	m_FrameMap.insert({ BOSS_HURTRECOVER_FADE, TempFrame });

	TempFrame.AnimKey = L"BOSS_PATTERN_IN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 70;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 72;
	TempFrame.iFrameSizeY = 62;
	m_FrameMap.insert({ BOSS_PATTERN_IN, TempFrame });

	TempFrame.AnimKey = L"BOSS_PATTERN_OUT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 70;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 72;
	TempFrame.iFrameSizeY = 62;
	m_FrameMap.insert({ BOSS_PATTERN_OUT, TempFrame });

	///
	TempFrame.AnimKey = L"BOSS_SWEEP_IN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 108;
	TempFrame.iFrameSizeY = 84;
	m_FrameMap.insert({ BOSS_SWEEP_IN, TempFrame });

	TempFrame.AnimKey = L"BOSS_SWEEP_OUT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 2;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 68;
	TempFrame.iFrameSizeY = 82;
	m_FrameMap.insert({ BOSS_SWEEP_OUT, TempFrame });

	TempFrame.AnimKey = L"BOSS_SWEEP_NONFADE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 17;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 118;
	TempFrame.iFrameSizeY = 92;
	m_FrameMap.insert({ BOSS_SWEEP_NONFADE, TempFrame });

	TempFrame.AnimKey = L"BOSS_SWEEP_FADE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 25;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 118;
	TempFrame.iFrameSizeY = 92;
	m_FrameMap.insert({ BOSS_SWEEP_FADE, TempFrame });

	TempFrame.AnimKey = L"BOSS_TAKEOUTGUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 94;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_TAKEOUTGUN, TempFrame });
	
	///
	TempFrame.AnimKey = L"BOSS_SHOOT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 136;
	TempFrame.iFrameSizeY = 92;
	m_FrameMap.insert({ BOSS_SHOOT, TempFrame });

	TempFrame.AnimKey = L"BOSS_TELEPORT_IN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 56;
	TempFrame.iFrameSizeY = 90;
	m_FrameMap.insert({ BOSS_TELEPORT_IN, TempFrame });

	TempFrame.AnimKey = L"BOSS_TELEPORT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 56;
	TempFrame.iFrameSizeY = 90;
	m_FrameMap.insert({ BOSS_TELEPORT, TempFrame });

	TempFrame.AnimKey = L"BOSS_TELEPORT_GROUND";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 82;
	TempFrame.iFrameSizeY = 70;
	m_FrameMap.insert({ BOSS_TELEPORT_GROUND, TempFrame });

	TempFrame.AnimKey = L"BOSS_TELEPORT_OUT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 56;
	TempFrame.iFrameSizeY = 90;
	m_FrameMap.insert({ BOSS_TELEPORT_OUT, TempFrame });

	///
	TempFrame.AnimKey = L"BOSS_WALLGRAB";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 2;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 60;
	TempFrame.iFrameSizeY = 86;
	m_FrameMap.insert({ BOSS_WALLGRAB, TempFrame });

	TempFrame.AnimKey = L"BOSS_WALLJUMP";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 80;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 108;
	TempFrame.iFrameSizeY = 124;
	m_FrameMap.insert({ BOSS_WALLJUMP, TempFrame });

	TempFrame.AnimKey = L"BOSS_WALLJUMP_FALL";//
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 108;
	TempFrame.iFrameSizeY = 124;
	m_FrameMap.insert({ BOSS_WALLJUMP_FALL, TempFrame });

}

void CBoss::Attack()
{
	CObj* Temp = CObjFactory<CBullet>::Create();
	Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 10.f, m_tInfo.fY - 5.f);
	Temp->Set_FrontAngle(m_fFrontAngle);
	Temp->Set_AttackAngle(m_fAttackAngle);
	Temp->SetOwner(this);
	CObjMgr::Get_Instance()->Add_Object(BULLET, Temp);
}

void CBoss::LaserAttack(LASERTYPE _type, float _x, float _y)
{
	if (m_Laser != nullptr)
	{
		m_Laser->Set_State(DEAD);
		m_Laser = nullptr;
	}
	m_Laser = CObjFactory<CLaser>::Create();
	dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(false);
	dynamic_cast<CLaser*>(m_Laser)->SetLaserType(_type);
	dynamic_cast<CLaser*>(m_Laser)->SetAxis(_x,_y);
	m_Laser->Set_AttackAngle(m_fAttackAngle);
	CObjMgr::Get_Instance()->Add_Object(LASER, m_Laser);

}

void CBoss::StateUpdate()
{
	INFO* fTargetInfo = m_Target->Get_Info();
	float fTargetX = fTargetInfo->fX;

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

		m_PrevState = m_State;
	}
	
		
	switch (m_State)
	{
	case BOSS_IDLE :  //IDLE

		break;

	case BOSS_PREJUMP : //Idle -> 점프 전 준비(무릎굽힘)
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			Set_State(BOSS_JUMP);
			m_bJump = true;
			m_fSpeed_Vertical = 15.f;
		}
		break;

	case BOSS_JUMP : //점프 1장짜리
		if (m_WallJump)
		{
			Set_State(BOSS_WALLGRAB);
			m_bJump = false;
			m_fSpeed_Vertical = 0.f;
			
			if (m_fFrontAngle == 0)
				m_fFrontAngle = PI;
			else
			{
				m_fFrontAngle = 0;
				m_tInfo.fX += m_fSpeed * 2;
			}
		}
		
		if(m_fFrontAngle == 0)
			m_tInfo.fX += m_fSpeed * cos(PI);
		else
			m_tInfo.fX += m_fSpeed * cos(0.f);

		break;

	case BOSS_LAND : //착지하고(무릎 굽히고)-> Idle(핌)
		break;

	case BOSS_ROLL : //구르기
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(BOSS_PREJUMP);

		m_tInfo.fX += m_fSpeed * cos(m_fFrontAngle);
		break;

	case BOSS_PREDASH : //Idle -> 대쉬 전 착지 자세 모션(준비자세?)
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			Set_State(BOSS_DASH);
		}
		break;

	case BOSS_DASH : //대시 한장짜리
		if(m_tInfo.fX < 290.f || m_tInfo.fX > WINCX - 290.f)
			Set_State(BOSS_DASHEND);
		
		if(!g_SlowMotion)
			m_tInfo.fX += cos(m_fFrontAngle) * (50.f);
		else
			m_tInfo.fX += cos(m_fFrontAngle) * (10.f);
		break;

	case BOSS_DASHEND : //대시하면서 칼 공격내려치고 idle
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(BOSS_PATTERN_OUT);
		break;

	case BOSS_DEAD : //바닥 기어감
		
		if (m_fFrontAngle == 0)
		{
			if (!m_DirCheck[LEFT])
				m_tInfo.fX += 0.3f;
		}
		else if (m_fFrontAngle == PI)
		{
			if (!m_DirCheck[RIGHT])
				m_tInfo.fX -= 0.3f;
		}
		if (CheckHurt())
			Set_State(BOSS_NOHEAD);
		break;

	case BOSS_DIEFLY : //마지막 날라갈때?
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(BOSS_DIEGROUND);

		
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[LEFT])
					m_tInfo.fX += 3.f;
			}
			else if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX -= 3.f;
			}
		
		break;

	case BOSS_DIEGROUND : //마지막 날아가서 떨어질때
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			Set_State(BOSS_DEAD);
			m_tInfo.fY += 30.f;
			m_Levitation = true;
		}
			
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[LEFT])
					m_tInfo.fX += 3.f;
			}
			else if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX -= 3.f;
			}
		
		break;
	case BOSS_NOHEAD : //머리 잘린 몸통
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(BOSS_NOHEAD_STOP);
		break;

	case BOSS_NOHEAD_STOP: //머리 잘린 몸통 정지
		break;


	case BOSS_AIM_MOVE : //레이저 아래 -> 옆 -> 위 조준
		break;
	case BOSS_AIM_STOP : //레이저 옆으로 조준 한장짜리
		if (dynamic_cast<CLaser*>(m_Laser)->GetLaserAttack() && m_LaserTime + 300 < GetTickCount64())
		{
			m_FrameReverse = true;
			m_LaserTime = 0;
			Set_State(BOSS_DRAWGUN);
			dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(false);
		}
		else if (!dynamic_cast<CLaser*>(m_Laser)->GetLaserAttack() && m_LaserTime + 600 < GetTickCount64())
		{
			dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(true);
			m_LaserTime = GetTickCount64();
		}
		break;

	case BOSS_DRAWGUN : //레이저 꺼냄 -> 조준 
		if (m_FrameReverse == false && m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			m_FrameMap[m_State].iFrameStart = 0;
			m_FrameMap[m_State].dwTime = GetTickCount64();
			m_LaserTime = GetTickCount64();
			LaserAttack(LASER_FOLLOW, m_tInfo.fX + cos(m_fFrontAngle) * 30.f, m_tInfo.fY);
			m_Laser->Set_AttackAngle(m_fFrontAngle);

			Set_State(BOSS_AIM_STOP);
		}
		else if (m_FrameReverse == true && m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd) //레이저 조준 -> 집어넣음
		{
			m_FrameReverse = false;
			Set_State(BOSS_ROLL);
		}

		break;
	case BOSS_HURTRECOVER : //맞고 날아서 바닥 착지
		break;

	case BOSS_HURTRECOVER_FADE :  //맞고 날아서 바닥 착지 후 은신
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Pattern_Laser180();

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

		break;
	case BOSS_PATTERN_IN : // 은신->착지 자세로 나타남
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			if (m_PatternOn)
				m_PatternOn = false;
		}
		break;
	case BOSS_PATTERN_OUT : //착지 자세-> 은신
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			if (m_PatternOn)
				m_PatternOn = false;
		}
		break;

	case BOSS_SWEEP_IN : //은신 -> 벽 잡은 상태?에서 옆 레이저 조준한 상태로 나타남 
		break;
	case BOSS_SWEEP_OUT : //반 사라진 상태 + 옆 레이저 조준 상태 -> 완전히 사라짐
		break;
	case BOSS_SWEEP_NONFADE : //180도 레이저
		break;
	case BOSS_SWEEP_FADE : //은신-> 나타남 + 180도 레이저 -> 은신
		
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			if (m_PatternOn)
				m_PatternOn = false;
			m_Levitation = false;
		}
		
		if (m_fFrontAngle == PI)
		{
			if (m_FrameMap[m_State].iFrameStart == 3)
			{
				LaserAttack(LASER_LONG, (m_tInfo.fX + cos(m_fFrontAngle) * (15.f)) + cos(m_fFrontAngle) * 20.f, m_tInfo.fY + 30.f * sin(m_fAttackAngle));
				m_fAttackAngle = m_fFrontAngle;
				m_Laser->Set_AttackAngle(m_fAttackAngle);
				m_PrevFrame = 0;
			}
			else if (m_FrameMap[m_State].iFrameStart == 4)
				dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(true);
		}
		else if (m_fFrontAngle == 0)
		{
			if (m_FrameMap[m_State].iFrameStart == 3)
			{
				LaserAttack(LASER_LONG, (m_tInfo.fX + cos(m_fFrontAngle) * (15.f)) + cos(m_fFrontAngle) * 60.f, m_tInfo.fY + 30.f * sin(m_fAttackAngle));
				m_fAttackAngle = m_fFrontAngle;
				m_Laser->Set_AttackAngle(m_fAttackAngle);
				m_PrevFrame = 0;
			}
			else if (m_FrameMap[m_State].iFrameStart == 4)
				dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(true);
		}

		if (m_PrevFrame != m_FrameMap[m_State].iFrameStart) //애니메이션 프레임 카운트랑 맞춰서 업데이트
		{
			if (m_Laser != nullptr && m_FrameMap[m_State].iFrameStart > 4)
			{
				dynamic_cast<CLaser*>(m_Laser)->RotateAxis(cos(m_fFrontAngle) * (-1) * 7.f, m_tInfo.fY + 30.f * sin(m_fAttackAngle));
				m_fAttackAngle += cos(m_fFrontAngle) * 0.2f;
				m_Laser->Set_AttackAngle(m_fAttackAngle);
				m_PrevFrame = m_FrameMap[m_State].iFrameStart;
			}
		}


		if (!m_Laser90) //180도 레이저
		{
			if (m_FrameMap[m_State].iFrameStart == m_FrameMap[m_State].iFrameEnd - 6)
			{
				if (m_Laser != nullptr)
				{
					dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(false);
					m_Laser->Set_State(DEAD);
					m_Laser = nullptr;
					m_PrevFrame = 0;
				}
			}
		}
		else //90도 레이저
		{
			if (m_FrameMap[m_State].iFrameStart == 14)
			{
				if (m_Laser != nullptr)
				{
					dynamic_cast<CLaser*>(m_Laser)->SetLaserAttack(false);
					m_Laser->Set_State(DEAD);
					m_Laser = nullptr;
					m_PrevFrame = 0;
				}
			}
		}
		
		break;


	case BOSS_TAKEOUTGUN : //바닥 서서 옷에서 총꺼내서 조준(지뢰)
		break;
	case BOSS_SHOOT : //옆 조준 -> 지뢰 조준
		break;



	case BOSS_TELEPORT_IN : //은신 -> 나타나서 위->아래로 레이저(끝)
		if (m_FrameMap[m_State].iFrameStart == m_FrameMap[m_State].iFrameEnd)
		{
			Set_State(BOSS_TELEPORT);
			m_fAttackAngle = PI / 2.f;
			LaserAttack(LASER_SHORT, m_tInfo.fX - 5.f, m_tInfo.fY + 40.f);
		}
		m_fFrontAngle = PI;
		break;
	case BOSS_TELEPORT : //은신 -> 나타나서 위->아래로 레이저 쏘고 -> 은신 
			if (m_FrameMap[m_State].iFrameStart == m_FrameMap[m_State].iFrameEnd)
				Set_State(BOSS_TELEPORT_OUT);

		break;
	case BOSS_TELEPORT_GROUND ://은신 -> 나타나서 옆으로 레이저 쏘고 -> 은신 
		break;
	case BOSS_TELEPORT_OUT : //위->아래 레이저 -> 은신(끝)
		if (m_FrameMap[m_State].iFrameStart == m_FrameMap[m_State].iFrameEnd)
		{
			if (m_PatternOn)
				m_PatternOn = false;
			m_Levitation = false;
		}
		break;
	case BOSS_WALLGRAB : //벽잡기
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			Set_State(BOSS_WALLJUMP);
			m_bJump = true;
			m_fSpeed_Vertical = 10.f;
			Flip_FrontAngle();
			m_fAttackAngle = m_fFrontAngle;
			m_BulletTime = GetTickCount64();
			Attack();
		}
			
		break;
	case BOSS_WALLJUMP ://180도 총쏘면서 벽점프 
		m_tInfo.fX += m_fSpeed * cos(m_fFrontAngle);
		if (m_BulletTime + 40 < GetTickCount64())
		{
			Attack();
			m_fAttackAngle -= cos(m_fFrontAngle) * PI / 14.f;
			m_BulletTime = GetTickCount64();
		}

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			Set_State(BOSS_WALLJUMP_FALL);
			m_BulletTime = 0;
		}
	
		break;

	case BOSS_WALLJUMP_FALL:
		m_tInfo.fX += m_fSpeed * cos(m_fFrontAngle);
		

		if (!m_bJump)
		{
			Flip_FrontAngle();
			Set_State(BOSS_PATTERN_OUT);
		}
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

}

void CBoss::Jump()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (!m_WallJump && !m_Levitation)
	{
		if (0.f < fY - m_tInfo.fY)
			m_bJump = true;
	}


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
			if (g_SlowMotion)
			{
				if (g_SlowJumpTime + 80 < GetTickCount64())
				{
					if (m_State != GRAB_WALL)
					{
						m_tInfo.fY -= m_fSpeed_Vertical;

						if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
						else
							m_fSpeed_Vertical -= (0.034f * G) * 1.2f;
					}

					g_SlowJumpTime = GetTickCount64();
				}

			}
			else
			{

				m_tInfo.fY -= m_fSpeed_Vertical;

				if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
					m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
				else
					m_fSpeed_Vertical -= (0.034f * G) * 1.2f;


			}

		}
	}
	else if (bLineCol && !m_WallJump && !m_Levitation)
	{
		m_tInfo.fY = fY;
		m_fSpeed_Vertical = 0.f;
	}



}

void CBoss::Update_CheckCollide()
{
	if (m_fFrontAngle == PI)
	{
		m_CheckCollide.left = m_FrontCollide.left - m_CheckCWidth;
		m_CheckCollide.right = m_FrontCollide.right + m_CheckCWidth * 0.8;
	}
	else
	{
		m_CheckCollide.right = m_FrontCollide.right + m_CheckCWidth;
		m_CheckCollide.left = m_FrontCollide.left - m_CheckCWidth * 0.8;
	}
	m_CheckCollide.top = m_FrontCollide.top - 60.f;
	m_CheckCollide.bottom = m_FrontCollide.bottom;
}

bool CBoss::CheckTargetFront()
{
	if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_CheckCollide, m_Target->Get_Collide()))
		return true;
	else
		return false;
}

int CBoss::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		m_WallJump = true;
		m_DirCheck[_dir] = true;
	}
	else if (targetType == BULLET)
	{
		if (m_State != HURT && m_State != HURTGROUND)
		{
			if (_target->GetOwner() != this && _target->GetOwner()->Get_Type() != ENEMY)
			{
				m_State = BOSS_HURTRECOVER_FADE;
				if (_dir == LEFT)
					m_fFrontAngle = 0;
				else if (_dir == RIGHT)
					m_fFrontAngle = PI;
				_target->Set_State(DEAD);
				CSoundMgr::Get_Instance()->PlaySound(L"death_bullet.wav", SOUND_EFFECT, SOUND_VOL3);
			}
		}
	}

	return OBJ_NOEVENT;
}

int CBoss::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
		m_WallJump = false;
	}

	return OBJ_NOEVENT;
}

int CBoss::OnCollision(CObj* _target, DIR _dir)
{
	return OBJ_NOEVENT;
}



/*****************************************************************/
//                           PATTERN                             //
/*****************************************************************/

void CBoss::InitPatternList()
{
	if (m_Phase == 1) //페이즈 1
	{
	}
	else if (m_Phase == 2) //페이즈 2
	{

	}


	m_PatternList.push_back(&CBoss::Pattern_Dash);
	m_PatternList.push_back(&CBoss::Pattern_LeftLaser90);



	m_PatternList.push_back(&CBoss::Pattern_LaserBottom_1);
	m_PatternList.push_back(&CBoss::Pattern_LaserBottom_2);
	m_PatternList.push_back(&CBoss::Pattern_RightLaser90);
	m_PatternList.push_back(&CBoss::Pattern_Move_LeftBottom);
	m_PatternList.push_back(&CBoss::Pattern_GroundLaser);
	m_PatternList.push_back(&CBoss::Pattern_Move_RightBottom);
	m_PatternList.push_back(&CBoss::Pattern_GroundLaser);
	m_PatternList.push_back(&CBoss::Pattern_Laser180);
}

void CBoss::PatternChange()
{
	if (m_PatternIndex < m_PatternList.size())
	{
		if (!m_PatternOn)
		{
			(this->*m_PatternList[m_PatternIndex])();
			m_PatternOn = true;
			++m_PatternIndex;
		}
		else
		{
			if (m_State != BOSS_HURTRECOVER && m_State != BOSS_HURTRECOVER_FADE)
			{
				if (CheckHurt())
				{
					--m_Life;
					if (m_Life > 0)
					{
						Set_State(BOSS_HURTRECOVER_FADE);
						m_bJump = true;
						m_Levitation = false;
						m_fSpeed_Vertical = 4.f;

						
					}
					else if (m_Life == 0)
					{
						Set_State(BOSS_DIEFLY);
					}
					
					if (m_Laser != nullptr)
					{
						m_Laser->Set_State(DEAD);
						m_Laser = nullptr;
					}

				}
			}
			
		}
	}
	else
	{
		m_PatternIndex = 0;
		Pattern_Move_RightBottom();
	}
	


}

bool CBoss::CheckHurt()
{
	CObj* Player = CObjMgr::Get_Instance()->Get_Player();
	if (Player->Get_State() == ATTACK)
	{
		RECT* PlayerAttackCollide = Player->Get_AttackCollide();
		DIR dir = CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_Collide, PlayerAttackCollide);
		if (DIR_NONE != dir)
		{
			if (m_Life > 0)
			{
				if (dir == LEFT)
					m_fFrontAngle = 0;
				else
					if (dir == RIGHT)
						m_fFrontAngle = PI;
			}

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void CBoss::Pattern_GroundLaser()
{
	m_Levitation = false;
	Set_State(BOSS_DRAWGUN);
}

void CBoss::Pattern_Laser180()
{
	m_Laser90 = false;
	m_bJump = false;
	m_Levitation = true;
	Set_Pos(WINCX * 0.5f, 220.f);
	Set_State(BOSS_SWEEP_FADE);
}

void CBoss::Pattern_LeftLaser90()
{
	Set_Pos(300.f , 220.f);

	m_Laser90 = true;	
	m_bJump = false;
	m_Levitation = true;
	Set_State(BOSS_SWEEP_FADE);
}

void CBoss::Pattern_RightLaser90()
{
	Set_Pos(WINCX - 300.f, 220.f);

	m_Laser90 = true;
	m_bJump = false;
	m_Levitation = true;
	Set_State(BOSS_SWEEP_FADE);
}

void CBoss::Pattern_LaserBottom_1()
{
	m_bJump = false;
	m_Levitation = true;
	Set_Pos(300.f, 220.f);
	Set_State(BOSS_TELEPORT_IN);
}

void CBoss::Pattern_LaserBottom_2()
{
	m_bJump = false;
	m_Levitation = true;
	Set_Pos(600.f , 220.f);
	Set_State(BOSS_TELEPORT_IN);
}

void CBoss::Pattern_LaserBottom_3()
{
	m_bJump = false;
	m_Levitation = true;
	Set_Pos(WINCX - 600.f, 220.f);
	Set_State(BOSS_TELEPORT_IN);
}

void CBoss::Pattern_LaserBottom_4()
{
	m_bJump = false;
	m_Levitation = true;
	Set_Pos(WINCX - 300.f, 220.f);
	Set_State(BOSS_TELEPORT_IN);
}

void CBoss::Pattern_Dash()
{
	if (m_tInfo.fX > WINCX * 0.5f)
		m_fFrontAngle = PI;
	else
		m_fFrontAngle = 0;

	Set_State(BOSS_PREDASH);
}



void CBoss::Pattern_Move_LeftBottom()
{
	m_fFrontAngle = 0;
	Set_Pos(300.f, 560.f);
	Set_State(BOSS_PATTERN_IN);
}

void CBoss::Pattern_Move_RightBottom()
{
	m_fFrontAngle = PI;
	Set_Pos(WINCX - 300.f, 560.f);
	Set_State(BOSS_PATTERN_IN);
}
