#include "stdafx.h"
#include "StartMenu.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "SoundMgr.h"
#include "KeyMgr.h"


CStartMenu::CStartMenu()
{
}

CStartMenu::~CStartMenu()
{
	Release();
}

void CStartMenu::Initialize()
{
	//BGM
	CSoundMgr::Get_Instance()->PlayBGM(L"song_rainonbrick.ogg", SOUND_VOL1);

	//배경 이미지 경로
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Title/TitleFrameAll_resize.png", L"StartMenu");
	CSceneManager::Get_Instance()->Set_BackSize({ 1344, 784 });
	Set_BackGroundKey(L"StartMenu");

	m_BackFrame.AnimKey = L"BOSS_PREJUMP"; //
	m_BackFrame.iFrameStart = 0;
	m_BackFrame.iFrameEnd = 11;
	m_BackFrame.iMotion = 0;
	m_BackFrame.dwSpeed = 10;
	m_BackFrame.dwTime = GetTickCount64();
	m_BackFrame.iFrameSizeX = 1344;
	m_BackFrame.iFrameSizeY = 784;
}

void CStartMenu::Update()
{
	Move_Frame();

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE) || CKeyMgr::Get_Instance()->Key_Pressing(VK_RETURN))
	{
		CSceneManager::Get_Instance()->ToNextScene();
		CSoundMgr::Get_Instance()->PlaySound(L"sound_npc_dialogue_huntress_01.wav", SOUND_EFFECT,SOUND_VOL1);		
	}
}

void CStartMenu::LateUpdate()
{
}

void CStartMenu::Render(HDC _hDC)
{
	FrameRender(_hDC);
}

void CStartMenu::Release()
{
}

void CStartMenu::FrameRender(HDC hDC)
{
	// 사용할 CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_BackGroundKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	//카메라 위치(디폴트 -> 플레이어)
	//fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// 캔버스
	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

		g.DrawImage(pImage,
			Gdiplus::Rect(
				0,
				0,
				(int)m_BackFrame.iFrameSizeX, //복사 사이즈
				(int)m_BackFrame.iFrameSizeY  //복사 사이즈
			),
			m_BackFrame.iFrameStart * (int)m_BackFrame.iFrameSizeX,
			m_BackFrame.iMotion * (int)m_BackFrame.iFrameSizeY,
			m_BackFrame.iFrameSizeX,
			m_BackFrame.iFrameSizeY, //이미지 원본 사이즈
			Gdiplus::UnitPixel, &attr);
	
}

void CStartMenu::Move_Frame()
{
	if (m_BackFrame.dwTime + m_BackFrame.dwSpeed < GetTickCount64())
	{
			++m_BackFrame.iFrameStart;

			if (m_BackFrame.iFrameStart > m_BackFrame.iFrameEnd)
				m_BackFrame.iFrameStart = 0;

			m_BackFrame.dwTime = GetTickCount64();
	}
	

}