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
	//// ����� CBitmap
	//CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_AnimKey);
	//Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	////ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	//fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//// ĵ����
	//Gdiplus::Graphics g(hDC);


	////�̹��� Ư�� ��ġ �÷� �������� �ڵ�
	////Gdiplus::Color ColourOfTopLeftPixel;
	////Gdiplus::Status eStatus = Image->GetPixel(0, 0, &ColourOfTopLeftPixel);
	////_ASSERTE(eStatus == Gdiplus::Ok);

	////���� ����
	//Gdiplus::ImageAttributes attr;
	//attr.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255),
	//	Gdiplus::ColorAdjustTypeBitmap);

	////GIF Ȱ��ȭ
	//UINT count = pImage->GetFrameDimensionsCount();
	//GUID* pDimensionIDs = new GUID[count];
	//pImage->GetFrameDimensionsList(pDimensionIDs, count);
	//UINT frameCount = pImage->GetFrameCount(&pDimensionIDs[0]);
	//UINT uIndex = m_AnimFrameIndex; //������ ��ȯ (Update���� m_Count �������ֱ�)
	//pImage->SelectActiveFrame(&pDimensionIDs[0], uIndex);

	//g.TranslateTransform((int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
	//	, (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	//if (m_fFrontAngle == 0.f)
	//	g.ScaleTransform(-1.f, 1.f);

	//// ĵ���� ũ�� ����
	//Gdiplus::Rect tRect = Gdiplus::Rect(
	//	(pImage->GetWidth() * m_tInfo.fRatioY / 2) * (-1),
	//	(pImage->GetHeight() * m_tInfo.fRatioY / 2) * (-1),
	//	pImage->GetWidth() * m_tInfo.fRatioY, //���� ������ (SMALL : 2/5)
	//	pImage->GetHeight() * m_tInfo.fRatioY //���� ������
	//);

	////Gdiplus::Matrix mat;
	////g.GetTransform(&mat);
	
	////�̹��� ��� (����, ���ĺ��� O)
	//g.DrawImage(pImage,
	//	tRect, 0, 0,
	//	pImage->GetWidth(), pImage->GetHeight(), //�̹��� ���� ������
	//	Gdiplus::UnitPixel, &attr);

	////g.DrawImage(pImage,
	////	Gdiplus::Rect(
	////		((int)m_tRect.left - ((int)cameraPos.x - WINCX / 2)),
	////		((int)m_tRect.top - ((int)cameraPos.y - WINCY / 2)),
	////		pImage->GetWidth()* SMALL, //���� ������ (SMALL : 2/5)
	////		pImage->GetHeight() * SMALL //���� ������
	////	), 0, 0,
	////	pImage->GetWidth(), pImage->GetHeight(), //�̹��� ���� ������
	////	Gdiplus::UnitPixel, &attr);


	////�̹��� ��� (����, ���ĺ��� X)
	///*Gdiplus::CachedBitmap cBitMap(Image, &g);
	//g.DrawCachedBitmap(&cBitMap,
	//	(int)m_tRect.left - ((int)cameraPos.x - WINCX / 2),
	//	(int)m_tRect.top - ((int)cameraPos.y - WINCY / 2),
	//);*/

	///*WCHAR text[100];
	//_stprintf_s(text, L"[Player] x : %f \t y : %f", m_tInfo.fX, m_tInfo.fY);
	//TextOutW(hDC, m_tInfo.fX - ((int)cameraPos.x - WINCX / 2), m_tInfo.fY - ((int)cameraPos.y - WINCY / 2), text, lstrlen(text));*/

	////GIF ������ ������Ʈ
	////������ �� ����� GIF ������ �� �°� �ʱ�ȭ ����� ��
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

void CObj::FrameRenderToBlackWhite(HDC hDC)
{
	// ����� CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	Gdiplus::Bitmap* BlackImage = CloneBitmap(pImage);
	ConvertToBlackWhite(BlackImage);

	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// ĵ����
	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

	if (m_fFrontAngle == 0 || (m_FrameReverse == true && m_fFrontAngle == PI))
	{
		g.DrawImage(BlackImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL //���� ������
			),
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	else if (m_fFrontAngle == PI || (m_FrameReverse == true && m_fFrontAngle == 0))
	{
		g.DrawImage(BlackImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL //���� ������
			),
			(m_FrameMap[m_State].iFrameEnd - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}

	delete BlackImage;
	BlackImage = nullptr;
}


void CObj::RatioFixByImage(const TCHAR* _tcAnimKey)
{
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	m_tInfo.fRatioX = m_tInfo.fCX / (float)pBitMap->Get_Image()->GetWidth();
	m_tInfo.fRatioY = m_tInfo.fCY / (float)pBitMap->Get_Image()->GetHeight();
}

void CObj::BasicRender(HDC hDC)
{	
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_OneImgKey);
	
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}
	

	Gdiplus::Graphics g(hDC);

	//�̹��� ��� (����, ���ĺ��� X)
	g.DrawImage(pImage,
		Gdiplus::Rect(
			((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
			((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
			m_tInfo.fCX,  //���� ������
			m_tInfo.fCY //���� ������
		),
		0,
		0,
		pImage->GetWidth(),
		pImage->GetHeight(), //�̹��� ���� ������
		Gdiplus::UnitPixel);

	if (g_ClearReverse)
	{
		delete pImage;
		pImage = nullptr;
	}
}

void CObj::FrameRender(HDC hDC)
{
	// ����� CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}

	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// ĵ����
	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	//attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
	//	Gdiplus::ColorAdjustTypeBitmap);

	if (m_fFrontAngle == 0 || (m_FrameReverse == true && m_fFrontAngle == PI ))
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL //���� ������
			),
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	else if (m_fFrontAngle == PI || (m_FrameReverse == true && m_fFrontAngle == 0))
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL //���� ������
			),
			(m_FrameMap[m_State].iFrameEnd - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	
	if (g_ClearReverse)
	{
		delete pImage;
		pImage = nullptr;
	}
}

void CObj::FrameRender_OriginSize(HDC hDC)
{
	// ����� CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// ĵ����
	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

	if (m_fFrontAngle == 0 )
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX , //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY  //���� ������
			),
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	else if (m_fFrontAngle == PI)
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX , //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY  //���� ������
			),
			(m_FrameMap[m_State].iFrameEnd - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}

	if (g_ClearReverse)
	{
		delete pImage;
		pImage = nullptr;
	}
}

void CObj::FrameRender_OriginSize(HDC hDC,int _y) //������ ������Ʈ��
{
	// ����� CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// ĵ����
	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) - ((int)cameraPos.x - WINCX / 2)),
				((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) - ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX, //���� ������
				m_tInfo.fCY  //���� ������
			),
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);

		if (g_ClearReverse)
		{
			delete pImage;
			pImage = nullptr;
		}
}




void CObj::RotateRender(HDC hDC, float _angle)
{
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//����� �̹��� Key ��������
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_OneImgKey);
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}
	Gdiplus::Graphics g (hDC);

	g.TranslateTransform((int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2) 
		, (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	//rotate
	g.RotateTransform(_angle);
	g.TranslateTransform(-(int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
		, -(int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	//�̹��� ��� (����, ���ĺ��� X)
  	g.DrawImage(pImage,
		Gdiplus::Rect(
			((int)(m_tInfo.fX - m_tInfo.fCX * 0.5f) + ((int)cameraPos.x - WINCX / 2)),
			((int)(m_tInfo.fY - m_tInfo.fCY * 0.5f) + ((int)cameraPos.y - WINCY / 2)),
			pImage->GetWidth(),  //���� ������
			pImage->GetHeight()//���� ������
		),
		0,
		0,
		pImage->GetWidth(),
		pImage->GetHeight(), //�̹��� ���� ������
		Gdiplus::UnitPixel);

	g.ResetTransform();

	if (g_ClearReverse)
	{
		delete pImage;
		pImage = nullptr;
	}
}

//Rect ����
void CObj::RotateFrameRender_Vertical(HDC hDC, float _angle, float _resizeX, float _resizeY)
{
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//����� �̹��� Key ��������
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}
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

	if (!m_FrameReverse)
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tInfo.fX + ((int)cameraPos.x - WINCX / 2)),
				((int)m_tInfo.fY + ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL * _resizeX, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL * _resizeY),//���� ������
			
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iFrameStart * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	else
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tInfo.fX + ((int)cameraPos.x - WINCX / 2)),
				((int)m_tInfo.fY + ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL * _resizeX, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL * _resizeY),//���� ������
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeX,
			(m_FrameMap[m_State].iFrameEnd - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	

	g.ResetTransform();

	if (g_ClearReverse)
	{
		delete pImage;
		pImage = nullptr;
	}
}

//�߽� ����
void CObj::RotateFrameRender (HDC hDC, float _angle, float _resizeX, float _resizeY)
{
	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	//����� �̹��� Key ��������
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_FrameMap[m_State].AnimKey);
	Gdiplus::Bitmap* pImage;
	if (!g_ClearReverse)
	{
		//����� �̹��� Key ��������
		pImage = pBitMap->Get_Image();
	}
	else
	{
		pImage = CloneBitmap(pBitMap->Get_Image());
		ConvertToGrayScale(pImage);
	}
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

	if (!m_FrameReverse)
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tInfo.fX + ((int)cameraPos.x - WINCX / 2)),
				((int)m_tInfo.fY + ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL * _resizeX, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL * _resizeY),//���� ������

			m_FrameMap[m_State].iFrameStart* (int)m_FrameMap[m_State].iFrameSizeX,
			m_FrameMap[m_State].iMotion * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}
	else
	{
		g.DrawImage(pImage,
			Gdiplus::Rect(
				((int)m_tInfo.fX + ((int)cameraPos.x - WINCX / 2)),
				((int)m_tInfo.fY + ((int)cameraPos.y - WINCY / 2)),
				(int)m_FrameMap[m_State].iFrameSizeX * SMALL * _resizeX, //���� ������
				(int)m_FrameMap[m_State].iFrameSizeY * SMALL * _resizeY),//���� ������
			m_FrameMap[m_State].iFrameEnd * (int)m_FrameMap[m_State].iFrameSizeX,
			(m_FrameMap[m_State].iMotion - m_FrameMap[m_State].iFrameStart) * (int)m_FrameMap[m_State].iFrameSizeY,
			(int)m_FrameMap[m_State].iFrameSizeX,
			(int)m_FrameMap[m_State].iFrameSizeY, //�̹��� ���� ������
			Gdiplus::UnitPixel, &attr);
	}

	g.ResetTransform();

	if (g_ClearReverse)
	{
		delete pImage;
		pImage = nullptr;
	}
}



void CObj::Move_Frame()
{
	if (g_SlowMotion || g_BossDead)
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
	if (g_SlowMotion || g_BossDead)
	{
		m_fSpeed = 1.f;
		//m_fSpeed_Vertical -= 1.f;
	}
}



//****************************Bitmap �÷� ���� ����****************************/

void CObj::ConvertToBlackWhite(Gdiplus::Bitmap* bitmap)
{
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();

	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, width, height);
	bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	BYTE* scan0 = reinterpret_cast<BYTE*>(bitmapData.Scan0);
	int stride = bitmapData.Stride;

	for (int y = 0; y < height; ++y)
	{
		BYTE* row = scan0 + y * stride;
		for (int x = 0; x < width; ++x)
		{
			BYTE* pixel = row + x * 4;

			// �ٻ������� ��ġ�ϴ� ��� �����ϰ�, �׷��� ���� ��� ���������� ��ȯ
			if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255)
			{
				pixel[0] = pixel[1] = pixel[2] = 0; // ������ (RGB: 0, 0, 0)
				pixel[3] = 0; // ���� (Alpha) ä���� 0���� ����
			}
			else
			{
				pixel[0] = pixel[1] = pixel[2] = 0; // ������ (RGB: 0, 0, 0)
				pixel[3] = 255; // ���� (Alpha) ä���� 255�� ����
			}
		}
	}

	bitmap->UnlockBits(&bitmapData);
}

void CObj::ConvertToGrayScale(Gdiplus::Bitmap* bitmap)
{
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();

	Gdiplus:: BitmapData bitmapData;
	Gdiplus:: Rect rect(0, 0, width, height);
	bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	BYTE* scan0 = reinterpret_cast<BYTE*>(bitmapData.Scan0);
	int stride = bitmapData.Stride;

	for (int y = 0; y < height; ++y)
	{
		BYTE* row = scan0 + y * stride;
		for (int x = 0; x < width; ++x)
		{
			BYTE* pixel = row + x * 4;
			BYTE gray = static_cast<BYTE>((pixel[0] + pixel[1] + pixel[2]) / 3);
			pixel[0] = pixel[1] = pixel[2] = gray;
		}
	}

	bitmap->UnlockBits(&bitmapData);
}


Gdiplus::Bitmap* CObj::CloneBitmap(Gdiplus::Bitmap* sourceBitmap)
{
	// ���� ��Ʈ���� ������ �����Ͽ� ���ο� ��Ʈ���� ����
	Gdiplus::Bitmap* clonedBitmap = sourceBitmap->Clone(0, 0, sourceBitmap->GetWidth(), sourceBitmap->GetHeight(), sourceBitmap->GetPixelFormat());

	return clonedBitmap;
}