#include "stdafx.h"
#include "HJS1.h"
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

float	g_fSound = 1.f;

CHJS1::CHJS1()
{
}

CHJS1::~CHJS1()
{
	Release();
}

void CHJS1::Initialize()
{
	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage5_bg_render.bmp", L"FirstScene");
	CSceneManager::Get_Instance()->Set_BackSize({1344, 784});
	Set_BackGroundKey(L"FirstScene");
	
	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,575 }, { WINCX + 500,575 });
	CLineMgr::Get_Instance()->Add_Line({ 500,325 }, { 800,325 });
	//CLineMgr::Get_Instance()->Add_Line({ 500,425 }, { 800,425 });

	//벽
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(0,190,155,570));	
	CSceneManager::Get_Instance()->AddLineRect(GRABWALL,285,350,365,570);
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(1185, 190, 1345, 650));
	CSceneManager::Get_Instance()->AddLineRect(WALL, 500, 325, 800, 400);

	//아이템
	CObj* Temp = CObjFactory<CItem>::Create(800,550, 30, 30);
	dynamic_cast<CItem*>(Temp)->SetITemType(SWORD);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	Temp = CObjFactory<CItem>::Create(500, 550, 30, 30);
	dynamic_cast<CItem*>(Temp)->SetITemType(OILBOTTLE);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	//적
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(600, 250, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(600, 560, 60, 72));

	//BGM
	CSoundMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->PlayBGM(L"song_ending.ogg", g_fSound);
}

void CHJS1::Update()
{
	if(!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->Update();
	
	CMementoMgr::Get_Instance()->Update();
}

void CHJS1::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
}

void CHJS1::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
}

void CHJS1::Release()
{
}




