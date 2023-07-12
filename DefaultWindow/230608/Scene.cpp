#include "stdafx.h"
#include "Scene.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "SoundMgr.h"

void CScene::BackGroundRender(HDC hDC)
{
	//사용할 이미지 Key 가져오기
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(m_BackGroundKey);
	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	Gdiplus::Graphics g(hDC);

	//이미지 출력 (빠름, 알파블랜딩 X)
	Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap,
		0 - ((int)cameraPos.x - WINCX / 2), // 복사 받을 위치 X,Y 좌표
		0 - ((int)cameraPos.y - WINCY / 2)
	);
}

void CScene::BackGroundRender(HDC hDC, int _x, int _y, TCHAR* _BackKey)
{
	//사용할 이미지 Key 가져오기
	Gdiplus::Bitmap* Image = CBmpMgr::Get_Instance()->Find_Img(_BackKey);
	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	Gdiplus::Graphics g(hDC);

	//이미지 출력 (빠름, 알파블랜딩 X)
	Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap,
		_x - ((int)cameraPos.x - WINCX / 2), // 복사 받을 위치 X,Y 좌표
		_y - ((int)cameraPos.y - WINCY / 2)
	);
}




void CScene::ReplaySceneBGM()
{
	CSoundMgr::Get_Instance()->StopAll();
	if(m_BGMkey != L"")
		CSoundMgr::Get_Instance()->PlayBGM(m_BGMkey, SOUND_VOL1);
}


