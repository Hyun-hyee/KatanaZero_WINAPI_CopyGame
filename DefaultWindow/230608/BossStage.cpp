#include "stdafx.h"
#include "BossStage.h"
#include "stdafx.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "SceneManager.h"
#include "Wall.h"
#include "GrabWall.h"
#include "Item.h"
#include "SoundMgr.h"
#include "ArmEnemy.h"
#include "GunEnemy.h"
#include "MementoMgr.h"
#include "CollisionMgr.h"
#include "Boss.h"
#include "UIMgr.h"

bool		g_BossStart = false;
bool		g_BossDead = false;
bool		g_FourRender = false;

CBossStage::CBossStage()
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	m_RGBTime = 0;
	m_RGBTimeStart = false;

	g_BossStart = false;
	g_BossDead = false;

	m_StartCollide = {500,0, 700, WINCY };

	//플레이어 위치 조정
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(230,0);

	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage5_bg_render.bmp", L"BossScene_2");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage5_bg_render_slow.bmp", L"BossScene_2_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage5_bg_render_blackWhite.png", L"BossScene_2_BlackWhite");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage5_bg_render_dark.bmp", L"BossScene_2_dark");
	CSceneManager::Get_Instance()->Set_BackSize({ 1344, 784 });
	Set_BackGroundKey(L"BossScene_2");

	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,575 }, { WINCX + 500,575 });

	//벽
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(0, 100, 155, 570));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(1185, 100, 1345, 650));

	//적
	m_Boss = CObjFactory<CBoss>::Create(WINCX - 300, 560, 60, 72);
	dynamic_cast<CBoss*> (m_Boss)->Set_Phase(2);
	dynamic_cast<CBoss*> (m_Boss)->Set_Life(8);//****************라이프 임시***************//
	CObjMgr::Get_Instance()->Add_Object(BOSS, m_Boss);

	//BGM
	CSoundMgr::Get_Instance()->StopAll();

	m_BGMkey = L"song_monster.ogg";
}

void CBossStage::Update()
{
	/**********************************************/
	if (!m_RGBTimeStart && dynamic_cast<CBoss*>(m_Boss)->Get_Life() == 4)
	{
		m_RGBTimeStart = true;
		m_RGBTime = GetTickCount64();
		g_ColorOverlay = RED;
		m_BackGroundKey = L"BossScene_2_dark";
		CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Enter.wav", SOUND_ARMENEMY, SOUND_VOL1);
	}
	
	if (m_RGBTimeStart)
	{
		if (m_RGBTime + 900 < GetTickCount64() && m_RGBTime != 0)
		{
			switch (g_ColorOverlay)
			{
			case RED:
				g_ColorOverlay = GREEN;
				m_RGBTime = GetTickCount64();
				break;
			case GREEN:
				g_ColorOverlay = BLUE;
				m_RGBTime = GetTickCount64();
				break;
			case BLUE:
				g_ColorOverlay = YELLOW;
				m_RGBTime = GetTickCount64();
				break;
			case YELLOW:
				g_ColorOverlay = MAJENTA;
				m_RGBTime = GetTickCount64();
				break;
			case MAJENTA:
				g_ColorOverlay = RED;
				m_RGBTime = GetTickCount64();
				//m_RGBTimeStart = false;
				//m_RGBTime = 0;
				break;

			}
		}
		if(dynamic_cast<CBoss*>(m_Boss)->Get_Life() == 2)
			g_ColorOverlay = MAJENTA5;
		if (dynamic_cast<CBoss*>(m_Boss)->Get_Life() == 0)
		{
			g_ColorOverlay = DEFAULT;
			m_RGBTimeStart = false;
		}
	}
	//else
		//g_ColorOverlay = DEFAULT;
	/**********************************************/

	//특정 위치 들어와야 보스 스테이지 시작 
	if (!g_BossStart)
		CheckStart();

	//배경 흑백 전환
	if (g_BossDead)
	{
		CSoundMgr::Get_Instance()->StopBGM();
		Set_BackGroundKey(L"BossScene_2_BlackWhite");
		g_ColorOverlay = DEFAULT;
	}
	else
	{
		if (!m_RGBTimeStart)
		{
			if (!g_SlowMotion && !g_TimeStop)
				Set_BackGroundKey(L"BossScene_2");
			else
				Set_BackGroundKey(L"BossScene_2_slow");
		}
	}
		
	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse)
		CObjMgr::Get_Instance()->Update();

	CMementoMgr::Get_Instance()->Update();

	if (GetAsyncKeyState('P'))
		g_BossDead = !g_BossDead;
}

void CBossStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse)
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();

}

void CBossStage::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
	CUIMgr::Get_Instance()->Render(_hDC);
}

void CBossStage::Release()
{
}

void CBossStage::CheckStart()
{
	if (CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_StartCollide, CObjMgr::Get_Instance()->Get_Player()->Get_Collide()))
	{
		g_BossStart = true;

		//BGM
		CSoundMgr::Get_Instance()->PlayBGM(L"song_monster.ogg", SOUND_VOL1);
	}
}
