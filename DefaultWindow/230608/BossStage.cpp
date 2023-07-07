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

bool		g_BossStart = false;

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

	m_StartCollide = {500,0, 700, WINCY };


	//플레이어 위치 조정
	CObjMgr::Get_Instance()->Get_Player()->SetfX(230);

	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage5_bg_render.bmp", L"BossScene");
	CSceneManager::Get_Instance()->Set_BackSize({ 1344, 784 });
	Set_BackGroundKey(L"BossScene");

	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,575 }, { WINCX + 500,575 });
	//CLineMgr::Get_Instance()->Add_Line({ 500,425 }, { 800,425 });

	//벽
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(0, 100, 155, 570));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(1185, 100, 1345, 650));
	//CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(0, 0, 1345, 190));

	////아이템
	//CObj* Temp = CObjFactory<CItem>::Create(800, 550, 30, 30);
	//dynamic_cast<CItem*>(Temp)->SetITemType(SWORD);
	//CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	//Temp = CObjFactory<CItem>::Create(500, 550, 30, 30);
	//dynamic_cast<CItem*>(Temp)->SetITemType(OILBOTTLE);
	//CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	////적
	//CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(600, 250, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CBoss>::Create(1000, 560, 60, 72));

	//BGM
	CSoundMgr::Get_Instance()->PlayBGM(L"song_ending.ogg", SOUND_VOL1);
}

void CBossStage::Update()
{
	//특정 위치 들어와야 보스 스테이지 시작 
	if (!g_BossStart)
		CheckStart();
	
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->Update();

	CMementoMgr::Get_Instance()->Update();
}

void CBossStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
}

void CBossStage::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
}

void CBossStage::Release()
{
}

void CBossStage::CheckStart()
{
	if (CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_StartCollide, CObjMgr::Get_Instance()->Get_Player()->Get_Collide()))
		g_BossStart = true;
}
