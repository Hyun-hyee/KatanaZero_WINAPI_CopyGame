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
#include "UIObject.h"


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
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage2_bg_render.png", L"SecondStage");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2_bg_render_gray.png", L"SecondStage_gray");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_01.bmp", L"SecondStage1");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_02.bmp", L"SecondStage2");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_03.bmp", L"SecondStage3");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_04.bmp", L"SecondStage4");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_01_slow.bmp", L"SecondStage1_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_02_slow.bmp", L"SecondStage2_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_03_slow.bmp", L"SecondStage3_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp_Background(L"../Resource/images/stage2/stage2_bg_render_04_slow.bmp", L"SecondStage4_slow");
		
	CSceneManager::Get_Instance()->Set_BackSize({ 4288, 900 });
	Set_BackGroundKey(L"SecondStage_gray");

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
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"song_nightmare.ogg", SOUND_VOL1);
	m_BGMkey = L"song_nightmare.ogg";
	//플레이어 START 위치
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	m_pPlayer->Set_Pos(0, 760);


	//적 리스트
	m_EnemyList = CObjMgr::Get_Instance()->Get_ObjList(ENEMY);
	m_Go = false;
}

void CSecondStage::Update()
{
	if (!m_Go)
	{
		bool Check = true;
		for (auto iter : *m_EnemyList)
		{
			if (iter->Get_State() != HURTGROUND)
				Check = false;
		}
		if (Check)
		{
			CObj* Temp = CObjFactory<CUIObject>::Create(4090, 660, 10, 10);
			Temp->Set_State(UI_GO);
			dynamic_cast<CUIObject*> (Temp)->Set_FirstX(4090);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);

			Temp = CObjFactory<CUIObject>::Create(4093, 690, 10, 10);
			Temp->Set_State(UI_ARROW);
			dynamic_cast<CUIObject*> (Temp)->Set_FirstX(4093);
			CObjMgr::Get_Instance()->Add_Object(EFFECT, Temp);
			m_Go = true;
		}
	}

	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse)
		CObjMgr::Get_Instance()->Update();
	else if (CMementoMgr::Get_Instance()->GetReverseOn())
		m_Go = false;

	CMementoMgr::Get_Instance()->Update();
}

void CSecondStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse)
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
	
	ReplayAndNext();
}

void CSecondStage::Render(HDC _hDC)
{
	INFO* Pos = m_pPlayer->Get_Info();

	if (!g_ClearReverse)
	{
		if (!g_SlowMotion && !g_TimeStop)
		{
			if (Pos->fX + WINCX * 0.5f < 2120)
			{
				BackGroundRender(_hDC, 0, 0, L"SecondStage1");
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2");
			}
			else if (Pos->fX + WINCX * 0.5f >= 2120 && Pos->fX + WINCX * 0.5f < 2120 + 300)
			{
				BackGroundRender(_hDC, 0, 0, L"SecondStage1");
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2");
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3");
			}
			else if (Pos->fX + WINCX * 0.5f >= 2120 + 300 && Pos->fX + WINCX * 0.5f < 3400)
			{
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2");
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3");
			}
			else if (Pos->fX + WINCX * 0.5f >= 3400 && Pos->fX + WINCX * 0.5f < 3400 + 300)
			{
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2");
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3");
				BackGroundRender(_hDC, 1020 + 1100 + 1280, 0, L"SecondStage4");
			}
			else if (Pos->fX + WINCX * 0.5f >= 3400 + 300)
			{
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3");
				BackGroundRender(_hDC, 1020 + 1100 + 1280, 0, L"SecondStage4");
			}
		}
		else
		{

			if (Pos->fX + WINCX * 0.5f < 2120)
			{
				BackGroundRender(_hDC, 0, 0, L"SecondStage1_slow");
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2_slow");
			}
			else if (Pos->fX + WINCX * 0.5f >= 2120 && Pos->fX + WINCX * 0.5f < 2120 + 300)
			{
				BackGroundRender(_hDC, 0, 0, L"SecondStage1_slow");
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2_slow");
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3_slow");
			}
			else if (Pos->fX + WINCX * 0.5f >= 2120 + 300 && Pos->fX + WINCX * 0.5f < 3400)
			{
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2_slow");
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3_slow");
			}
			else if (Pos->fX + WINCX * 0.5f >= 3400 && Pos->fX + WINCX * 0.5f < 3400 + 300)
			{
				BackGroundRender(_hDC, 1020, 0, L"SecondStage2_slow");
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3_slow");
				BackGroundRender(_hDC, 1020 + 1100 + 1280, 0, L"SecondStage4_slow");
			}
			else if (Pos->fX + WINCX * 0.5f >= 3400 + 300)
			{
				BackGroundRender(_hDC, 1020 + 1100, 0, L"SecondStage3_slow");
				BackGroundRender(_hDC, 1020 + 1100 + 1280, 0, L"SecondStage4_slow");
			}
		}
	}
	else
		BackGroundRender(_hDC);
	
	
	

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
	CUIMgr::Get_Instance()->Render(_hDC);
}

void CSecondStage::Release()
{
}

void CSecondStage::ReplayAndNext()
{
	if (CSceneManager::Get_Instance()->GetClearStage() && !m_ReplayOn)
	{
		g_ClearReverse = true;
		m_ReplayOn = true;
	}

	if (m_ReplayOn && !g_ClearReverse)
	{
		CSceneManager::Get_Instance()->ToNextScene();
		m_ReplayOn = false;
	}
}


