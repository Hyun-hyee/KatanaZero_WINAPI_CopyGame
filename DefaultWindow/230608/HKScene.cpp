#include "stdafx.h"
#include "HKScene.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "SceneManager.h"
#include "Portal.h"

#include "Monster_ZombieMushroom.h"

CHKScene::CHKScene()
{
}

CHKScene::~CHKScene()
{
	Release();
}

void CHKScene::Initialize()
{
	//��Ż ����
	CObj* pPortal = CObjFactory<CPortal>::Create(1840, 980, 40, 100);
	dynamic_cast<CPortal*> (pPortal)->Set_PortalType(NEXT);
	CObjMgr::Get_Instance()->Add_Object(PORTAL, pPortal);

	//���� ����
	CLineMgr::Get_Instance()->Add_Line({ -100,970 }, { 2000,970 });

	//��� �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/����/�׷� 2.png", L"HKScene");

	//���� ����
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_ZombieMushroom>::Create(350, 900, 320, 240));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_ZombieMushroom>::Create(1450, 900, 320, 240));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_ZombieMushroom>::Create(250, 900, 320, 240));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_ZombieMushroom>::Create(750, 900, 320, 240));
	CObjMgr::Get_Instance()->Add_Object(MONSTER, CObjFactory<CMonster_ZombieMushroom>::Create(1650, 900, 320, 240));


}

void CHKScene::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CHKScene::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CHKScene::Render(HDC _hDC)
{
	//����� �̹��� Key ��������
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(L"HKScene");
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

void CHKScene::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
}
