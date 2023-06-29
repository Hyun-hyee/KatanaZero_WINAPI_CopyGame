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
	//��� �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/stage5_bg_render.bmp", L"FirstScene");
	CSceneManager::Get_Instance()->Set_BackSize({1344, 784});
	
	CLineMgr::Get_Instance()->Add_Line({ -500,550 }, { WINCX + 500,550 });

	
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




