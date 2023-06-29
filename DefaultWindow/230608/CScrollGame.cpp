#include "stdafx.h"
#include "ScrollGame.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "SceneManager.h"

CScrollGame::CScrollGame() 
{
}

CScrollGame::~CScrollGame()
{
	Release();
}

void CScrollGame::Initialize()
{
	//m_hDC = GetDC(g_hWnd);
	ShowCursor(FALSE);

	//ĳ���� ����
	CObjMgr::Get_Instance()->Add_Object(PLAYER, CObjFactory<CPlayer>::Create());
	
	//���� ����
	LINEPOINT tLine[] =
	{
		{0.f, WINCY - 100.f },
		{WINCX/3.f, WINCY - 200.f },
		{WINCX * 2 / 3.f, WINCY - 300.f },
		{WINCX, WINCY - 100.f },
		{0.f, WINCY - 250.f},
		{WINCX/3.f, WINCY- 350.f},
		{0,WINCY - 50},
		{WINCX,WINCY - 50}
	};
	//CLineMgr::Get_Instance()->Add_Line(tLine[0], tLine[1]);
	//CLineMgr::Get_Instance()->Add_Line(tLine[1], tLine[2]);
	//CLineMgr::Get_Instance()->Add_Line(tLine[4], tLine[5]);
	//CLineMgr::Get_Instance()->Add_Line(tLine[6], tLine[7]);
	CLineMgr::Get_Instance()->Add_Line({ 400, 720 }, { 1480, 720 });
	CLineMgr::Get_Instance()->Add_Line({0, 1020}, { 1880, 1020 });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/TestBackground.png", L"Ground");
}

void CScrollGame::Update()
{


	CObjMgr::Get_Instance()->Update();
}

void CScrollGame::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
}

void CScrollGame::Render(HDC _hDC)
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

	CLineMgr::Get_Instance()->Render(_hDC);
	CObjMgr::Get_Instance()->Render(_hDC);

	//���콺 ��ǥ
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	WCHAR text[100];
	wsprintf(text, L" x : %d \t y : %d", ptMouse.x, ptMouse.y);
	TextOutW(_hDC, ptMouse.x + 5, ptMouse.y, text, lstrlen(text));
}

void CScrollGame::Release()
{
	CObjMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	//ReleaseDC(g_hWnd, m_hDC);
}


