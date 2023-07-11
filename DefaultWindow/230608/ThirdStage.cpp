#include "stdafx.h"
#include "ThirdStage.h"
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
#include "KeyMgr.h"

CThirdStage::CThirdStage()
{
}

CThirdStage::~CThirdStage()
{
	Release();
}

void CThirdStage::Initialize()
{
	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/Stage3/Stage3.png", L"ThirdStage");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/Stage3/Stage3_slow.png", L"ThirdStage_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/Stage3/Stage3_gray.png", L"ThirdStage_gray");
	CSceneManager::Get_Instance()->Set_BackSize({ 1600, 784 });
	Set_BackGroundKey(L"ThirdStage");

	//라인
	CLineMgr::Get_Instance()->Add_Line({ -500,665 }, { 2000,665 });

	CLineMgr::Get_Instance()->Add_Line({ 0,508 }, { 328,508 });
	CLineMgr::Get_Instance()->Add_Line({ 0,368 }, { 420,368 });
	CLineMgr::Get_Instance()->Add_Line({ 0,244 }, { 360,244 });
	CLineMgr::Get_Instance()->Add_Line({ 1244,244 }, { 1600,244 });
	CLineMgr::Get_Instance()->Add_Line({ 1180,368 }, { 1600,368 });
	CLineMgr::Get_Instance()->Add_Line({ 1284,508 }, { 1600,508 });

	//벽
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(-500, 0, 15, 784));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(1600 - 30, 0, 2000, 784));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(-500, 0, 2000, 68));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(-500, 68 ,376,124));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(1240, 72, 2000, 128));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(324, 388, 332, 500));
	CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(1264, 400, 1272, 516));

	//적
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(260, 420, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(260, 330, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(160, 210, 60, 72));

	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(1360, 420, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(1360, 330, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CGunEnemy>::Create(1460, 210, 60, 72));

	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(300, 640, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(400, 640, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(500, 640, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(600, 640, 60, 72));
	CObjMgr::Get_Instance()->Add_Object(ENEMY, CObjFactory<CArmEnemy>::Create(700, 640, 60, 72));


	CObj* Temp = CObjFactory<CArmEnemy>::Create(1600 - 300, 640, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);
	Temp = CObjFactory<CArmEnemy>::Create(1600 - 400, 640, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);
	Temp = CObjFactory<CArmEnemy>::Create(1600 - 500, 640, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);
	Temp = CObjFactory<CArmEnemy>::Create(1600 - 600, 640, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);
	Temp = CObjFactory<CArmEnemy>::Create(1600 - 700, 640, 60, 72);
	Temp->Set_FrontAngle(PI);
	CObjMgr::Get_Instance()->Add_Object(ENEMY, Temp);

	//BGM
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"song_edm.ogg", SOUND_VOL1);
	m_BGMkey = L"song_edm.ogg";

	//플레이어 START 위치
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(800, 90);

	EnemyList = CObjMgr::Get_Instance()->Get_ObjList(ENEMY);
}

void CThirdStage::Update()
{
	if (CheckClear())
	{
		if(CKeyMgr::Get_Instance()->Key_Pressing('W'))
				CSceneManager::Get_Instance()->SetClearStage(true);
	}

	if (!g_ClearReverse)
	{
		if (!g_SlowMotion && !g_TimeStop)
			Set_BackGroundKey(L"ThirdStage");
		else
			Set_BackGroundKey(L"ThirdStage_slow");
	}
	else
		Set_BackGroundKey(L"ThirdStage_gray");

	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse)
		CObjMgr::Get_Instance()->Update();

	CMementoMgr::Get_Instance()->Update();
}

void CThirdStage::LateUpdate()
{
	if (!CMementoMgr::Get_Instance()->GetReverseOn() && !g_ClearReverse)
		CObjMgr::Get_Instance()->LateUpdate();

	CMementoMgr::Get_Instance()->LateUpdate();
	
	CheckClear();
	ReplayAndNext();
}

void CThirdStage::Render(HDC _hDC)
{
	BackGroundRender(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CMementoMgr::Get_Instance()->Render(_hDC);
	CUIMgr::Get_Instance()->Render(_hDC);
}

void CThirdStage::Release()
{
}

void CThirdStage::ReplayAndNext()
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

bool CThirdStage::CheckClear()
{
	bool Check = true;
	for (const auto iter : *EnemyList)
	{
		if (iter->Get_State() != HURTGROUND)
			Check = false;
	}

	return Check;
}




