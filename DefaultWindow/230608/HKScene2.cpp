#include "stdafx.h"
#include "HKScene2.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "SceneManager.h"
#include "Portal.h"

#include "Monster_Vellum.h"

CHKScene2::CHKScene2()
{
}

CHKScene2::~CHKScene2()
{
	Release();
}

void CHKScene2::Initialize()
{
	//��Ż ����
	CObj* pPortal = CObjFactory<CPortal>::Create(1840, 860, 40, 100);
	dynamic_cast<CPortal*> (pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� ����
	CLineMgr::Get_Instance()->Add_Line({ -100,890 }, { 2000,890 });
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });

	//��� �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/����/��������.png", L"HKScene2");

	//���� ����
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_Vellum>::Create(960, 900, 400, 600));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_Vellum>::Create(1200, 900, 400, 300));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_Vellum>::Create(600, 900, 400, 100));

}

void CHKScene2::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CHKScene2::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CHKScene2::Render(HDC _hDC)
{
	//����� �̹��� Key ��������
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"HKScene2");
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

void CHKScene2::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
}
