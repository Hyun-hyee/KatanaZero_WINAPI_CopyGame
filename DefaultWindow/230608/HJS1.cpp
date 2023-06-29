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
#include "Portal.h"
#include "BasicMonster.h"

CHJS1::CHJS1()
{
}

CHJS1::~CHJS1()
{
	Release();
}

void CHJS1::Initialize()
{
	//다음 맵 포탈
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	////이전 맵 포탈
	//pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	//dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	//CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//라인 생성
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });
	CLineMgr::Get_Instance()->Add_Line({ 180,750 }, { 680,750 });
	CLineMgr::Get_Instance()->Add_Line({ 530,530 }, { 1050,530 });
	CLineMgr::Get_Instance()->Add_Line({ 420,370 }, { 710,370 });
	CLineMgr::Get_Instance()->Add_Line({ 60,210 }, { 630,210 });
	CLineMgr::Get_Instance()->Add_Line({ 1110,380 }, { 1650,380 });
	CLineMgr::Get_Instance()->Add_Line({ 1520,180 }, { 1810,180 });


	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/혜진/동굴.png", L"FirstScene");

	//몬스터 생성
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(80, 900, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(600, 900, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(1200, 900, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(1700, 900, 320 * 2 / 3, 240 * 2 / 3));

	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(200, 700, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(800, 480, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(1200, 330, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBasicMonster>::Create(1600, 130, 320 * 2 / 3, 240 * 2 / 3));

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

	//사용할 이미지 Key 가져오기
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"FirstScene");
	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	Gdiplus::Graphics g(_hDC);

	//이미지 출력 (빠름, 알파블랜딩 X)
	Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap,
		0 - ((int)cameraPos.x - WINCX / 2), // 복사 받을 위치 X,Y 좌표
		0 - ((int)cameraPos.y - WINCY / 2)
	);

	CObjMgr::Get_Instance()->Render(_hDC);
}

void CHJS1::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	//ReleaseDC(g_hWnd, m_hDC);
}




