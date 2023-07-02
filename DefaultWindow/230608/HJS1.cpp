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
	
	CLineMgr::Get_Instance()->Add_Line({ -500,550 }, { WINCX + 500,550 });
	CLineMgr::Get_Instance()->Add_Line({ 500,300 }, { 800,300 });

	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::Create(77,400,155,370));	
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::Create(77 + 300, 450, 80, 200));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::Create(77+1070, 400, 155, 370));

	CObj* Temp = CObjFactory<CItem>::Create(800,550, 30, 30);
	dynamic_cast<CItem*>(Temp)->SetITemType(SWORD);
	CObjMgr::Get_Instance()->Add_Object(ITEM, Temp);

	//BGM
	CSoundMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->PlayBGM(L"song_ending.ogg", g_fSound);
}

void CHJS1::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CHJS1::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CHJS1::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
}

void CHJS1::Release()
{
}




