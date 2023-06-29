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
	//���� �� ��Ż
	CObj* pPortal = CObjFactory<CPortal>::Create(1820, 970, 40, 100);
	dynamic_cast<CPortal*>(pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	////���� �� ��Ż
	//pPortal = CObjFactory<CPortal>::Create(100, 970, 40, 100);
	//dynamic_cast<CPortal*>(pPortal)->Set_PortalType(PREV);
	//CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� ����
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });
	CLineMgr::Get_Instance()->Add_Line({ 180,750 }, { 680,750 });
	CLineMgr::Get_Instance()->Add_Line({ 530,530 }, { 1050,530 });
	CLineMgr::Get_Instance()->Add_Line({ 420,370 }, { 710,370 });
	CLineMgr::Get_Instance()->Add_Line({ 60,210 }, { 630,210 });
	CLineMgr::Get_Instance()->Add_Line({ 1110,380 }, { 1650,380 });
	CLineMgr::Get_Instance()->Add_Line({ 1520,180 }, { 1810,180 });


	//��� �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/����/����.png", L"FirstScene");

	//���� ����
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

	//����� �̹��� Key ��������
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"FirstScene");
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

void CHJS1::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	//ReleaseDC(g_hWnd, m_hDC);
}




