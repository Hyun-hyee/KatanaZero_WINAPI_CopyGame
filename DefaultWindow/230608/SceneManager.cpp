#include "stdafx.h"
#include "SceneManager.h"
#include "FirstScene.h"
#include "SecondScene.h"
#include "ObjMgr.h"
#include "Player.h"
#include "ObjFactory.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "WTS1.h"
#include "WTS2.h"
#include "FourScene.h"
#include "FiveScene.h"
#include "HJS1.h"
#include "HJS2.h"
#include "HKScene.h"
#include "HKScene2.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

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


	//캐릭터 생성
	CObjMgr::Get_Instance()->Add_Object(PLAYER, CObjFactory<CPlayer>::Create());

	//Scene 생성
	SceneList.push_back(new CHJS1);
	SceneList.back()->Initialize();
	SceneList.back()->Set_SceneOn(true);
	m_PlayScene = SceneList.back();

	SceneList.push_back(new CHJS2);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CFirstScene);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CSecondScene);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CWTS1);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CWTS2);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CFourScene);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CFiveScene);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CHKScene);
	SceneList.back()->Set_SceneOn(false);

	SceneList.push_back(new CHKScene2);
	SceneList.back()->Set_SceneOn(false);

	SceneList[0]->Set_NextScene(SceneList[1]);
	SceneList[1]->Set_NextScene(SceneList[2]);
	SceneList[2]->Set_NextScene(SceneList[3]);
	SceneList[3]->Set_NextScene(SceneList[4]);
	SceneList[4]->Set_NextScene(SceneList[5]);
	SceneList[5]->Set_NextScene(SceneList[6]);
	SceneList[6]->Set_NextScene(SceneList[7]);
	SceneList[7]->Set_NextScene(SceneList[8]);
	SceneList[8]->Set_NextScene(SceneList[9]);
	SceneList[9]->Set_NextScene(SceneList[0]);
	//SceneList[7]->Set_NextScene(SceneList[0]);

	SceneList[9]->Set_PrevScene(SceneList[8]);
	SceneList[8]->Set_PrevScene(SceneList[7]);
	SceneList[7]->Set_PrevScene(SceneList[6]);
	SceneList[6]->Set_PrevScene(SceneList[5]);
	SceneList[5]->Set_PrevScene(SceneList[4]);
	SceneList[4]->Set_PrevScene(SceneList[3]);
	SceneList[3]->Set_PrevScene(SceneList[2]);
	SceneList[2]->Set_PrevScene(SceneList[1]);
	SceneList[1]->Set_PrevScene(SceneList[0]);
	SceneList[0]->Set_PrevScene(SceneList[9]);
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

	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void CSceneManager::Release()
{
	Gdiplus::GdiplusShutdown(ptr);
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
