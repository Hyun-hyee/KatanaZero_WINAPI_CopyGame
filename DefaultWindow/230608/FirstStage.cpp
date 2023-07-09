#include "stdafx.h"
#include "FirstStage.h"
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
#include "Fan.h"
#include "UIMgr.h"


CFirstStage::CFirstStage()
{
}

CFirstStage::~CFirstStage()
{
	Release();
}

void CFirstStage::Initialize()
{
	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Stage2bg.bmp", L"FirstStage");
	CSceneManager::Get_Instance()->Set_BackSize({ 1950, 1400 });
	Set_BackGroundKey(L"FirstStage");

	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,1208 }, {580,1208 });

	//벽
	CSceneManager::Get_Instance()->AddLineRect(GRABWALL, 572, 856, 1200, 1244);
	CSceneManager::Get_Instance()->AddLineRect(GRABWALL, 1192, 576, 2000, 1000);

	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(-500, 664, 440, 1052));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(0, 364, 1060, 676));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(1068, 272, 2000, 380));

	//오브젝트
	CObjMgr::Get_Instance()->Add_Object(FAN, CObjFactory<CFan>::CreateRECT(1580, 404, 1620, 548));


	//아이템
	//CObj* Temp = CObjFactory<CItem>::Create(800, 550, 30, 30);
	//dynamic_cast<CItem*>(Temp)->SetITemType(SWORD);
	//CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	//Temp = CObjFactory<CItem>::Create(500, 550, 30, 30);
	//dynamic_cast<CItem*>(Temp)->SetITemType(OILBOTTLE);
	//CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	////적
	//CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(600, 200, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(680, 750, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(880, 750, 60, 72));

	//BGM
	CSoundMgr::Get_Instance()->PlayBGM(L"song_killyourtv.ogg", SOUND_VOL1);

	//플레이어 START 위치
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(100, 1200);
}

void CFirstStage::Update()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->Update();

	CMementoMgr::Get_Instance()->Update();
}

void CFirstStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
}

void CFirstStage::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
	CUIMgr::Get_Instance()->Render(_hDC);
}

void CFirstStage::Release()
{
}




