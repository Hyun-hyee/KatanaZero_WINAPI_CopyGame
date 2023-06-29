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

	//���� �� ��Ż
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� �� ��Ż
	pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� ����
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CFollowMonster>::Create(150, 900, 320 * SMALL, 240 * SMALL));

	//���� ����
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });

	//��� �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/����/����.png", L"Ground");
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

void CSecondScene::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	//ReleaseDC(g_hWnd, m_hDC);
}



