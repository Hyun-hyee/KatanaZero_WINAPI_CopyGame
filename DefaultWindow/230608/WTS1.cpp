#include "stdafx.h"
#include "WTS1.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SceneManager.h"
#include "LineMgr.h"
#include "ObjFactory.h"
#include "Portal.h"
#include "ChMonster.h"
#include "Honeycomb.h"
#include "Thorn.h"
#include "ShootingStar.h"
#include "WTB.h"
//#include "LineFactory.h"
//#include "NonLine.h"
//#include "MonsterLine.h"

CWTS1::CWTS1()
{
}

CWTS1::~CWTS1()
{
	Release();
}

void CWTS1::Initialize()
{
	//´ÙÀ½ ¸Ê Æ÷Å»
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//ÀÌÀü ¸Ê Æ÷Å»
	pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//CLineMgr::Get_Instance()->Add_Line({ -100,880 }, { 1880,880 });
	/*CLineMgr::Get_Instance()->Add_Line(CLineFactory<CNonLine>::Create({ -100,880 }, { 1880,880 }));
	CLine* pLine = CLineFactory<CMonsterLine>::Create({ 380,500 }, { 480,500 });
	dynamic_cast<CMonsterLine*>(pLine)->Create_Monster();
	CLineMgr::Get_Instance()->Add_Line(pLine);*/

	CLineMgr::Get_Instance()->Add_Line({ -100,940 }, { 2000,940 });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/¿ìÅÃ/¸Ê/»ç³ÉÅÍ.png", L"CWTS1");

	//CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CChMonster>::Create(100.f, 370.f,320*SMALL,240*SMALL));
	CObj* pchMonster = CObjFactory<CChMonster>::Create(100.f, 370.f, 320 * SMALL, 240 * SMALL);
	dynamic_cast<CChMonster*>(pchMonster)->Set_Move_Range(-100, 1880);
	CObjMgr::Get_Instance()->Add_Object(MONSTER, pchMonster);

	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CHoneycomb>::Create(500.f, 370.f, 320 * SMALL, 240 * SMALL));
	
}

void CWTS1::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CWTS1::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CWTS1::Render(HDC _hDC)
{
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"CWTS1");
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	Gdiplus::Graphics g(_hDC);

	Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap, 0 - ((int)cameraPos.x - WINCX / 2), 0 - ((int)cameraPos.y - WINCY / 2));

	//CLineMgr::Get_Instance()->Render(_hDC);
	CObjMgr::Get_Instance()->Render(_hDC);
}

void CWTS1::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
}
