#include "stdafx.h"
#include "FourScene.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "SceneManager.h"
#include "Portal.h"
#include "FollowMonster.h"

#include "WatcherLight.h"
#include "Imp.h"
#include "BanBan.h"
CFourScene::CFourScene()
{
}

CFourScene::~CFourScene()
{
	Release();
}

void CFourScene::Initialize()
{

	//���� �� ��Ż
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� �� ��Ż
	pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� ����
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBanBan>	   ::Create(150, 130, 320 * SMALL, 240 * SMALL));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CBanBan>	   ::Create(1600, 330, 320 * SMALL, 240 * SMALL));

	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CImp>			::Create(600, 900, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CImp>			::Create(1200, 900, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CImp>			::Create(200, 700, 320 * 2 / 3, 240 * 2 / 3));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CImp>			::Create(800, 480, 320 * 2 / 3, 240 * 2 / 3));

	//���� ����
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });

	CLineMgr::Get_Instance()->Add_Line({ 180,750 }, { 680,750 });
	CLineMgr::Get_Instance()->Add_Line({ 530,530 }, { 1050,530 });
	CLineMgr::Get_Instance()->Add_Line({ 420,370 }, { 710,370 });
	CLineMgr::Get_Instance()->Add_Line({ 60,210 }, { 630,210 });
	CLineMgr::Get_Instance()->Add_Line({ 1110,380 }, { 1650,380 });
	CLineMgr::Get_Instance()->Add_Line({ 1520,180 }, { 1810,180 });


	//��� �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/����/�ð���ƴ��.png", L"Ground");

}

void CFourScene::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CFourScene::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CFourScene::Render(HDC _hDC)
{

	//����� �̹��� Key ��������
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"Ground");
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	Gdiplus::Graphics g(_hDC);

	//�̹��� ��� (����, ���ĺ��� X)
	Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap,
		0 - ((int)cameraPos.x - WINCX / 2), // ���� ���� ��ġ X,Y ��ǥ
		0 - ((int)cameraPos.y - WINCY / 2)
	);

	CObjMgr::Get_Instance()->Render(_hDC);

}

void CFourScene::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	//ReleaseDC(g_hWnd, m_hDC);
}



