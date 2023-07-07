#include "stdafx.h"
#include "Obj.h"
#include "BitMap.h"
#include "SceneManager.h"
#include "BmpMgr.h"

CObj::CObj() : 
	m_fSpeed(0.f), m_State(IDLE), m_Type(OBJ_NONE),
	m_Collider_type(RECTANGLE),m_fAccel(0.f),m_Owner(nullptr),
	m_fFrontAngle(0.f), m_fFrontCWidth(50.f),m_PrevState(OBJ_STATE_END),
	m_CollideSize(5.f/2.f), m_OneImgKey(L""), m_FrameReverse(false)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tRect, sizeof(m_Collide));
}

CObj::~CObj()
{
}

//CObj& CObj::operator=(const CObj& other)
//{
//	memcpy(&m_tInfo, &other.m_tInfo, sizeof(m_tInfo));
//	memcpy(&m_tRect, &other.m_tRect, sizeof(m_tRect));
//	this->m_FrameMap = other.m_FrameMap;
//	this->m_OneImgKey = other.m_OneImgKey;
//	this->m_fFrontAngle = other.m_fFrontAngle;
//	this->m_State = other.m_State;
//
//	return *this;
//}

//
//CObj::CObj( CObj& other)
//{
//	memcpy(&m_tInfo, &other.m_tInfo,sizeof( m_tInfo ));
//	memcpy(&m_tRect, &other.m_tRect, sizeof(m_tRect));
//	other.m_FrameMap =this->m_FrameMap;
//	other.m_OneImgKey = this->m_OneImgKey;
//	other.m_fFrontAngle = this->m_fFrontAngle;
//}

void CObj::Render(HDC hDC)
{
	//// 사용할 CBitmap
	//CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_AnimKey);
	//Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	////카메라 위치(디폴트 -> 플레이어)
	//fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//// 캔버스
	//Gdiplus::Graphics g(hDC);


	////이미지 특정 위치 컬러 가져오는 코드
	////Gdiplus::Color ColourOfTopLeftPixel;
	////Gdiplus::Status eStatus = Image->GetPixel(0, 0, &ColourOfTopLeftPixel);
	////_ASSERTE(eStatus == Gdiplus::Ok);

	////알파 블랜딩
	//Gdiplus::ImageAttributes attr;
	//attr.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255),
	//	Gdiplus::ColorAdjustTypeBitmap);

	////GIF 활성화
	//UINT count = pImage->GetFrameDimensionsCount();
	//GUID* pDimensionIDs = new GUID[count];
	//pImage->GetFrameDimensionsList(pDimensionIDs, count);
	//UINT frameCount = pImage->GetFrameCount(&pDimensionIDs[0]);
	//UINT uIndex = m_AnimFrameIndex; //프레임 전환 (Update에서 m_Count 조절해주기)
	//pImage->SelectActiveFrame(&pDimensionIDs[0], uIndex);

	//g.TranslateTransform((int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
	//	, (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	//if (m_fFrontAngle == 0.f)
	//	g.ScaleTransform(-1.f, 1.f);

	//// 캔버스 크기 설정
	//Gdiplus::Rect tRect = Gdiplus::Rect(
	//	(pImage->GetWidth() * m_tInfo.fRatioY / 2) * (-1),
	//	(pImage->GetHeight() * m_tInfo.fRatioY / 2) * (-1),
	//	pImage->GetWidth() * m_tInfo.fRatioY, //복사 사이즈 (SMALL : 2/5)
	//	pImage->GetHeight() * m_tInfo.fRatioY //복사 사이즈
	//);

	////Gdiplus::Matrix mat;
	////g.GetTransform(&mat);
	
	////이미지 출력 (느림, 알파블랜딩 O)
	//g.DrawImage(pImage,
	//	tRect, 0, 0,
	//	pImage->GetWidth(), pImage->GetHeight(), //이미지 원본 사이즈
	//	Gdiplus::UnitPixel, &attr);

	////g.DrawImage(pImage,
	////	Gdiplus::Rect(
	////		((int)m_tRect.left - ((int)cameraPos.x - WINCX / 2)),
	////		((int)m_tRect.top - ((int)cameraPos.y - WINCY / 2)),
	////		pImage->GetWidth()* SMALL, //복사 사이즈 (SMALL : 2/5)
	////		pImage->GetHeight() * SMALL //복사 사이즈
	////	), 0, 0,
	////	pImage->GetWidth(), pImage->GetHeight(), //이미지 원본 사이즈
	////	Gdiplus::UnitPixel, &attr);


	////이미지 출력 (빠름, 알파블랜딩 X)
	///*Gdiplus::CachedBitmap cBitMap(Image, &g);
	//g.DrawCachedBitmap(&cBitMap,
	//	(int)m_tRect.left - ((int)cameraPos.x - WINCX / 2),
	//	(int)m_tRect.top - ((int)cameraPos.y - WINCY / 2),
	//);*/

	///*WCHAR text[100];
	//_stprintf_s(text, L"[Player] x : %f \t y : %f", m_tInfo.fX, m_tInfo.fY);
	//TextOutW(hDC, m_tInfo.fX - ((int)cameraPos.x - WINCX / 2), m_tInfo.fY - ((int)cameraPos.y - WINCY / 2), text, lstrlen(text));*/

	////GIF 프레임 업데이트
	////프레임 꼭 사용할 GIF 프레임 수 맞게 초기화 해줘야 함
	//if (m_AnimTime + 80 < GetTickCount64())
	//{
	//	{
	//		++m_AnimFrameIndex;
	//		if (m_AnimFrameIndex > m_AnimFrame)
	//			m_AnimFrameIndex = 0;
	//	}
	//	m_AnimTime = GetTickCount64();
	//}
}

void CObj::Update_Rect()
{
	m_tRect.left	 = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));	
	m_tRect.top		 = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	 = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom   = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
	m_Collide.left	 = m_tRect.left	   - m_CollideSize;
	m_Collide.top    = m_tRect.top	   - m_CollideSize;
	m_Collide.right  = m_tRect.right   + m_CollideSize;
	m_Collide.bottom = m_tRect.bottom  + m_CollideSize;
	
	if (m_fFrontAngle == 0)
	{
		m_FrontCollide.left = m_tInfo.fX;
		m_FrontCollide.right = m_Collide.right + m_fFrontCWidth;
	}
	if (m_fFrontAngle == PI)
	{
		m_FrontCollide.right = m_tInfo.fX;
		m_FrontCollide.left = m_Collide.left - m_fFrontCWidth;
	}
	m_FrontCollide.top = m_Collide.top;
	m_FrontCollide.bottom = m_Collide.bottom;
}

void CObj::CollideRender(HDC hDC)
{
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

	HPEN myPen = CreatePen(PS_SOLID, 0, RGB(255, 0, 255));
	HPEN oldPen = (HPEN)SelectObject(hDC, myPen);

	Rectangle(hDC, m_tRect.left - ((float)cameraPos.x - WINCX / 2),
		m_tRect.top - ((float)cameraPos.y - WINCY / 2),
		m_tRect.right - ((float)cameraPos.x - WINCX / 2),
		m_tRect.bottom - ((float)cameraPos.y - WINCY / 2));

	Rectangle(hDC, m_Collide.left - ((float)cameraPos.x - WINCX / 2),
		m_Collide.top - ((float)cameraPos.y - WINCY / 2),
		m_Collide.right - ((float)cameraPos.x - WINCX / 2),
		m_Collide.bottom - ((float)cameraPos.y - WINCY / 2));

	SelectObject(hDC, oldBrush);
	DeleteObject(myBrush);

	SelectObject(hDC, oldPen);
	DeleteObject(myPen);

	if (m_Type == PLAYER || m_Type == ENEMY)
	{
		myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		myPen = CreatePen(PS_SOLID, 0, RGB(0, 255, 255));
		oldPen = (HPEN)SelectObject(hDC, myPen);

		if (m_Type == ENEMY)
		{
			Rectangle(hDC, m_FrontCollide.left - ((float)cameraPos.x - WINCX / 2),
				m_FrontCollide.top - ((float)cameraPos.y - WINCY / 2),
				m_FrontCollide.right - ((float)cameraPos.x - WINCX / 2),
				m_FrontCollide.bottom - ((float)cameraPos.y - WINCY / 2));
		}

		Rectangle(hDC, m_AttackCollide.left - ((float)cameraPos.x - WINCX / 2),
			m_AttackCollide.top - ((float)cameraPos.y - WINCY / 2),
			m_AttackCollide.right - ((float)cameraPos.x - WINCX / 2),
			m_AttackCollide.bottom - ((float)cameraPos.y - WINCY / 2));

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);

		SelectObject(hDC, oldPen);
		DeleteObject(myPen);
	}
	
}

void CObj::CollideRender(HDC hDC, RECT _collide)
{
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

	HPEN myPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(hDC, myPen);

	Rectangle(hDC, _collide.left - ((float)cameraPos.x - WINCX / 2),
		_collide.top - ((float)cameraPos.y - WINCY / 2),
		_collide.right - ((float)cameraPos.x - WINCX / 2),
		_collide.bottom - ((float)cameraPos.y - WINCY / 2));

	SelectObject(hDC, oldBrush);
	DeleteObject(myBrush);

	SelectObject(hDC, oldPen);
	DeleteObject(myPen);

}


void CObj::RatioFixByImage(const TCHAR* _tcAnimKey)
{
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	m_tInfo.fRatioX = m_tInfo.fCX / (float)pBitMap->Get_Image()->GetWidth();
	m_tInfo.fRatioY = m_tInfo.fCY / (float)pBitMap->Get_Image()->GetHeight();
}

void CObj::BasicRender(HDC hDC)
{	
	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//사용할 이미지 Key 가져오기
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_OneImgKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	Gdiplus::Graphics g(hDC);

	//이미지 출력 (빠름, 알파블랜딩 X)
	g.DrawImage(pImage,
		Gdiplus::Rect(
			((int)m_tRect.left - ((int)cameraPos.x - WINCX / 2)),
			((int)m_tRect.top - ((int)cameraPos.y - WINCY / 2)),
			m_tInfo.fCX,  //복사 사이즈
			m_tInfo.fCY //복사 사이즈
		),
		0,
		0,
		pImage->GetWidth(),
		pImage->GetHeight(), //이미지 원본 사이즈
		Gdiplus::UnitPixel);
}

void CObj::FrameRender(HDC hDC)
{
	// 사용할 CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// 캔버스
	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

	if (m_fFrontAngle == 0 || (m_FrameReverse == true && m_fFrontAngle == PI ))
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tRect.left - ((int)cameraPos.x - WINCX / 2)),
				((int)m_tRect.top - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //복사 사이즈
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL //복사 사이즈
			),
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //이미지 원본 사이즈
			Gdiplus::UnitPixel, &attr);
	}
	else if (m_fFrontAngle == PI || (m_FrameReverse == true && m_fFrontAngle == 0))
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tRect.left - ((int)cameraPos.x - WINCX / 2)),
				((int)m_tRect.top - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //복사 사이즈
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL //복사 사이즈
			),
			(m_FrameMap[m_State].iFrameEnd - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //이미지 원본 사이즈
			Gdiplus::UnitPixel, &attr);
	}

}

void CObj::RotateRender(HDC hDC, float _angle)
{
	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//사용할 이미지 Key 가져오기
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_OneImgKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	Gdiplus::Graphics g (hDC);

	g.TranslateTransform((int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
	, (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	//rotate
	g.RotateTransform(_angle);
	g.TranslateTransform(-(int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
		,- (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));

	//이미지 출력 (빠름, 알파블랜딩 X)
  	g.DrawImage(pImage,
		Gdiplus::Rect(
			((int)m_tRect.left + ((int)cameraPos.x - WINCX / 2)),
			((int)m_tRect.top + ((int)cameraPos.y - WINCY / 2)),
			pImage->GetWidth(),  //복사 사이즈
			pImage->GetHeight()//복사 사이즈
		),
		0,
		0,
		pImage->GetWidth(),
		pImage->GetHeight(), //이미지 원본 사이즈
		Gdiplus::UnitPixel);

	g.ResetTransform();
}

void CObj::RotateFrameRender_Vertical(HDC hDC, float _angle)
{
	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//사용할 이미지 Key 가져오기
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	Gdiplus::Graphics g(hDC);

	g.TranslateTransform((int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
		, (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	//rotate
	g.RotateTransform(_angle);
	g.TranslateTransform(-(int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
		, -(int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));


	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

	float MoreSmall = 0.5f;



	if (!m_FrameReverse)
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tRect.left + ((int)cameraPos.x - WINCX / 2)),
				((int)m_tRect.top + ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //복사 사이즈
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL * MoreSmall //복사 사이즈
			),
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //이미지 원본 사이즈
			Gdiplus::UnitPixel, &attr);
	}
	else
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tRect.left + ((int)cameraPos.x - WINCX / 2)),
				((int)m_tRect.top + ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //복사 사이즈
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL * MoreSmall //복사 사이즈
			),
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeX,
			(m_FrameMap[m_State].iFrameEnd - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //이미지 원본 사이즈
			Gdiplus::UnitPixel, &attr);
	}
	

	g.ResetTransform();
}


void CObj::Move_Frame()
{
	if (g_SlowMotion)
	{
		if (m_FrameMap[m_State].dwTime + m_FrameMap[m_State].dwSpeed + 80 < GetTickCount64())
		{
			++m_FrameMap[m_State].iFrameStart;

			if (m_FrameMap[m_State].iFrameStart > m_FrameMap[m_State].iFrameEnd)
				m_FrameMap[m_State].iFrameStart = 0;

			m_FrameMap[m_State].dwTime = GetTickCount64();
		}
	}
	else
	{
		if (m_FrameMap[m_State].dwTime + m_FrameMap[m_State].dwSpeed < GetTickCount64())
		{
			++m_FrameMap[m_State].iFrameStart;

			if (m_FrameMap[m_State].iFrameStart > m_FrameMap[m_State].iFrameEnd)
				m_FrameMap[m_State].iFrameStart = 0;

			m_FrameMap[m_State].dwTime = GetTickCount64();
		}
	}
	
}

void CObj::UpdateAttackCollide()
{
	m_AttackCInfo.fX = m_tInfo.fX + m_AttackCDistance * cos(m_fAttackAngle);
	m_AttackCInfo.fY = m_tInfo.fY - m_AttackCDistance * sin(m_fAttackAngle);
	
	m_AttackCollide.left = m_AttackCInfo.fX - 0.5 * m_AttackCInfo.fCX;
	m_AttackCollide.top = m_AttackCInfo.fY - 0.5 * m_AttackCInfo.fCY;
	m_AttackCollide.right = m_AttackCInfo.fX + 0.5 * m_AttackCInfo.fCX;
	m_AttackCollide.bottom = m_AttackCInfo.fY + 0.5 * m_AttackCInfo.fCY;
}

void CObj::SlowMotionUpdate()
{
	if (g_SlowMotion)
	{
		m_fSpeed = 1.f;
		//m_fSpeed_Vertical -= 1.f;
	}
}




