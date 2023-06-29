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
	//배경 이미지 경로
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




