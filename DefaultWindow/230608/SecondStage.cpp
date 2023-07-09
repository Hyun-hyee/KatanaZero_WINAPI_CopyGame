#include "stdafx.h"
#include "SecondStage.h"
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


CSecondStage::CSecondStage()
{
}

CSecondStage::~CSecondStage()
{
	Release();
}

void CSecondStage::Initialize()
{
	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage2_bg_render.png", L"SecondStage");
	CSceneManager::Get_Instance()->Set_BackSize({ 4288, 900 });
	Set_BackGroundKey(L"SecondStage");

	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,788-10}, { 1956,788-10 });
	CLineMgr::Get_Instance()->Add_Line({ 1956,788-10}, { 2196,536-10 });
	CLineMgr::Get_Instance()->Add_Line({ 2196,536-10}, { 2820,536-10 });
	CLineMgr::Get_Instance()->Add_Line({ 2820,536-10}, { 3060,784-10 });
	CLineMgr::Get_Instance()->Add_Line({ 3060,784-10}, { 4788,784-10 });

	CLineMgr::Get_Instance()->Add_Line({ 3068,585 - 10 }, { 3123,525 - 10 });
	CLineMgr::Get_Instance()->Add_Line({ 3123,525 - 10 }, { 3604,525 - 10 });

	//벽
	CSceneManager::Get_Instance()->AddLineRect(GRABWALL, -500, 410, 664, 644);
	CSceneManager::Get_Instance()->AddLineRect(GRABWALL, 804, 410, 1116, 640);
	CSceneManager::Get_Instance()->AddLineRect(WALL, 1116, 410, 1436, 448);

	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(0, 20, 572, 400));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(400, 16, 1600, 260));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(1568, 28, 1788, 640));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(1792, 32, 4788, 316));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(2592, 308, 2652, 388));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(3324, 324, 3388, 388));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(3744, 324, 4788, 576));
	//CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(3128, 532, 3604, 576));

	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(4, 788 + 10, 1956, 876));
	//CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(2196, 536 + 10, 2820, 616));
	//CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(3060, 784 + 10, 4268, 848));

	//아이템
	CObj* Temp = CObjFactory<CItem>::Create(444, 740, 30, 30);
	dynamic_cast<CItem*>(Temp)->SetITemType(SWORD);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	Temp = CObjFactory<CItem>::Create(1708, 756, 30, 30);
	dynamic_cast<CItem*>(Temp)->SetITemType(OILBOTTLE);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	Temp = CObjFactory<CItem>::Create(3240, 736, 30, 30);
	dynamic_cast<CItem*>(Temp)->SetITemType(OILBOTTLE);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	//적
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(1048, 352, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(1232, 320, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(1216, 688, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(2376, 460, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(2684, 460, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(3208, 456, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(3400, 704, 60, 72));

	//BGM
	CSoundMgr::Get_Instance()->PlayBGM(L"song_killyourtv.ogg", SOUND_VOL1);

	//플레이어 START 위치
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(0, 760);
}

void CSecondStage::Update()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->Update();

	CMementoMgr::Get_Instance()->Update();
}

void CSecondStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn())
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
}

void CSecondStage::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
	CUIMgr::Get_Instance()->Render(_hDC);
}

void CSecondStage::Release()
{
}




