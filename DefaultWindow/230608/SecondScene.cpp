#include "stdafx.h"
#include "SecondScene.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "SceneManager.h"
#include "Portal.h"
#include "FollowMonster.h"

CSecondScene::CSecondScene()
{
}

CSecondScene::~CSecondScene()
{
	Release();
}

void CSecondScene::Initialize()
{

	//다음 맵 포탈
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//이전 맵 포탈
	pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//몬스터 생성
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CFollowMonster>::Create(150, 900, 320 * SMALL, 240 * SMALL));

	//라인 생성
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });

	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/혜진/궁전.png", L"Ground");
}

void CSecondScene::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CSecondScene::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CSecondScene::Render(HDC _hDC)
{

	//사용할 이미지 Key 가져오기
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"Ground");
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

void CSecondScene::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	//ReleaseDC(g_hWnd, m_hDC);
}



