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
#include "LaserObject.h"
#include "PlayerEffect.h"

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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Stage2bg_gray.bmp", L"FirstStage_gray");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Stage2bg_slow.bmp", L"FirstStage_slow");
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
	
	CObj* Temp = CObjFactory<CLaserObject>::Create();
	Temp->Set_Pos(1127, 618);
	dynamic_cast<CLaserObject*>(Temp)->SetHeight(445);
	CObjMgr::Get_Instance()->Add_Object(LASEROBJECT, Temp);

	////적
	//CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(600, 200, 60, 72));
	Temp = CObjFactory<CArmEnemy>::Create(680, 750, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);
	Temp = CObjFactory<CArmEnemy>::Create(880, 750, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);

	//BGM
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"song_killyourtv.ogg", SOUND_VOL1);
	m_BGMkey = L"song_killyourtv.ogg";

	//플레이어 START 위치
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(100, 1200);

}

void CFirstStage::Update()
{
	if (!g_ClearReverse)
	{
		if (!g_SlowMotion && !g_TimeStop)
			Set_BackGroundKey(L"FirstStage");
		else
			Set_BackGroundKey(L"FirstStage_slow");
	}
	else
		Set_BackGroundKey(L"FirstStage_gray");

	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse )
		CObjMgr::Get_Instance()->Update();

	CMementoMgr::Get_Instance()->Update();
}

void CFirstStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse )
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
	ReplayAndNext();
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

void CFirstStage::ReplayAndNext()
{
	if (CSceneManager::Get_Instance()->GetClearStage()&& !m_ReplayOn)
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




