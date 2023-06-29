#include "stdafx.h"
#include "WTS2.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "ObjFactory.h"
#include "Portal.h"
#include "WTB.h"
//#include "LineFactory.h"
//#include "NonLine.h"

CWTS2::CWTS2()
{
}

CWTS2::~CWTS2()
{
	Release();
}

void CWTS2::Initialize()
{

	//´ÙÀ½ ¸Ê Æ÷Å»
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//ÀÌÀü ¸Ê Æ÷Å»
	pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//CLineMgr::Get_Instance()->Add_Line({ -40,960 }, { 1880,960 });
	//CLineMgr::Get_Instance()->Add_Line(CLineFactory<CNonLine>::Create({ -100,880 }, { 1880,880 }));
	CLineMgr::Get_Instance()->Add_Line({ -100,910 }, { 2000,910 });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/¿ìÅÃ/¸Ê/ÇÇ¿¡¸£.png", L"CWTS2");

	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CWTB>::Create(1000.f, 910.f, 320, 80));
}

void CWTS2::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CWTS2::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CWTS2::Render(HDC _hDC)
{
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"CWTS2");

	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	Gdiplus::Graphics g(_hDC);

	Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap, 0 - ((int)cameraPos.x - WINCX / 2), 0 - ((int)cameraPos.y - WINCY / 2));

	CObjMgr::Get_Instance()->Render(_hDC);
}

void CWTS2::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
}
