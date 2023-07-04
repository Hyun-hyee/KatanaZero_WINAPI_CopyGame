#include "stdafx.h"
#include "SceneManager.h"
#include "ObjMgr.h"
#include "Player.h"
#include "ObjFactory.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "UIMgr.h"
#include "BmpMgr.h"
#include "HJS1.h"
#include "SoundMgr.h"
#include "Wall.h"
#include "GrabWall.h"
#include "MementoMgr.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

bool g_SlowMotion = false;
DWORD g_SlowJumpTime = 0;

CSceneManager::CSceneManager()
{
	m_PlayScene = nullptr;
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	ShowCursor(FALSE);
	srand((unsigned int)time(NULL));

	ULONG_PTR ptr; //Gdi+사용을 위한 포인터객체
	Gdiplus::GdiplusStartupInput in; //gdi+입력값객체
	Gdiplus::GdiplusStartup(&ptr, &in, 0); //시작

	_hdc = GetDC(g_hWnd);
	::GetClientRect(g_hWnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc와 호환되는 DC를 생성
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // _hdc와 호환되는 비트맵 생성
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
	::DeleteObject(prev);

	//ObjMgr,MementoMgr (순서 중요)
	CObjMgr::Get_Instance()->Initialize();
	CMementoMgr::Get_Instance()->Initialize();

	//UIMgr
	CUIMgr::Get_Instance()->Initialize();

	//캐릭터 생성
	CObjMgr::Get_Instance()->Add_Object(PLAYER, CObjFactory<CPlayer>::Create());

	//Scene 생성
	SceneList.push_back(new CHJS1);
	SceneList.back()->Initialize();
	SceneList.back()->Set_SceneOn(true);
	m_PlayScene = SceneList.back();
}

void CSceneManager::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down('9'))
		ToPrevScene();

	if (CKeyMgr::Get_Instance()->Key_Down('0'))
		ToNextScene();

	for (auto iter = SceneList.begin(); iter != SceneList.end(); ++iter)
	{
		if ((*iter)->Get_SceneOn())
			(*iter)->Update();
	}
}

void CSceneManager::LateUpdate()
{
	for (auto iter = SceneList.begin(); iter != SceneList.end(); ++iter)
	{
		if ((*iter)->Get_SceneOn())
			(*iter)->LateUpdate();
   	}
	CKeyMgr::Get_Instance()->Key_LateUpdate();
}

void CSceneManager::Render()
{

	for (auto iter = SceneList.begin(); iter != SceneList.end(); ++iter)
	{
		if ((*iter)->Get_SceneOn())
			(*iter)->Render(_hdcBack);
	}
	CUIMgr::Get_Instance()->Render(_hdcBack);
	CLineMgr::Get_Instance()->Render(_hdcBack);
	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void CSceneManager::ToNextScene()
{
	CScene* pNextScene = m_PlayScene->Get_NextScene();
	if (pNextScene)
	{
		pNextScene->Set_SceneOn(true);
		m_PlayScene->Set_SceneOn(false);
		CObjMgr::Get_Instance()->Change_Scene();
		CLineMgr::Get_Instance()->Change_Scene();
		pNextScene->Initialize();
		m_PlayScene = pNextScene;
		CObjMgr::Get_Instance()->Get_Player()->SetfX(100.f);
	}
}

void CSceneManager::ToPrevScene()
{
	CScene* pPrevScene = m_PlayScene->Get_PrevScene();
	if (pPrevScene)
	{
		pPrevScene->Set_SceneOn(true);
		m_PlayScene->Set_SceneOn(false);
		CObjMgr::Get_Instance()->Change_Scene();
		CLineMgr::Get_Instance()->Change_Scene();
		pPrevScene->Initialize();
		m_PlayScene = pPrevScene;
		CObjMgr::Get_Instance()->Get_Player()->SetfX(1820.f);
	}
}

void CSceneManager::AddLineRect(OBJID _walltype, float _left, float _top, float _right, float _bottom)
{
	if(_walltype == GRABWALL)
		CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CGrabWall>::CreateRECT(_left, _top, _right, _bottom));
	else if (_walltype == WALL)
		CObjMgr::Get_Instance()->Add_Object(WALL, CObjFactory<CWall>::CreateRECT(_left, _top, _right, _bottom));

	CLineMgr::Get_Instance()->Add_Line({ _left - 10.f,_top - 10.f }, { _right + 10.f,_top - 10.f });
}




void CSceneManager::Release()
{
	for (auto& iter : SceneList)
		Safe_Delete<CScene*>(iter);
	SceneList.clear();

	Gdiplus::GdiplusShutdown(ptr);
	CObjMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CMementoMgr::Destroy_Instance();
	//ReleaseDC(g_hWnd, m_hDC);
}

