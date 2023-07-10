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

CBossStage::CBossStage()
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	g_BossStart = false;
	g_BossDead = false;

	m_StartCollide = {500,0, 700, WINCY };

	//플레이어 위치 조정
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(230,0);

	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage5_bg_render.bmp", L"BossScene_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage5_bg_render_slow.bmp", L"BossScene_2_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage5_bg_render_blackWhite.png", L"BossScene_2_BlackWhite");
	CSceneManager::Get_Instance()->Set_BackSize({ 1344, 784 });
	Set_BackGroundKey(L"BossScene_2");

	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,575 }, { WINCX + 500,575 });

	//벽
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(0, 100, 155, 570));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(1185, 100, 1345, 650));

	//적
	CObj* BossTemp = CObjFactory<CBoss>::Create(WINCX - 300, 560, 60, 72);
	dynamic_cast<CBoss*> (BossTemp)->Set_Phase(2);
	dynamic_cast<CBoss*> (BossTemp)->Set_Life(6);
	CObjMgr::Get_Instance()->Add_Object(BOSS, BossTemp);

	//BGM
	CSoundMgr::Get_Instance()->StopAll();
}

void CBossStage::Update()
{

	//특정 위치 들어와야 보스 스테이지 시작 
	if (!g_BossStart)
		CheckStart();

	//배경 흑백 전환
	if (g_BossDead)
	{
		CSoundMgr::Get_Instance()->StopBGM();
		Set_BackGroundKey(L"BossScene_2_BlackWhite");
	}
	else
	{
		if (!g_SlowMotion)
			Set_BackGroundKey(L"BossScene_2");
		else
			Set_BackGroundKey(L"BossScene_2_slow");
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
