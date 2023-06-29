#include "stdafx.h"
#include "Obj.h"
#include "BitMap.h"
#include "SceneManager.h"
#include "BmpMgr.h"

CObj::CObj() : 
	m_fSpeed(0.f), m_State(IDLE), m_Type(OBJ_NONE),
	m_Collider_type(RECTANGLE),m_fAccel(0.f),m_Owner(nullptr),
	m_AnimKey(nullptr),m_AnimFrame(0),m_AnimFrameIndex(0),
	m_fFrontAngle(0.f), m_FrontCWidth(50.f)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tRect, sizeof(m_Collide));

	m_AnimTime = GetTickCount64();
}

CObj::~CObj()
{
}

void CObj::Render(HDC hDC)
{
	// ����� CBitmap
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_AnimKey);
	Gdiplus::Bitmap* pImage = pBitMap->Get_Image();

	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	// ĵ����
	Gdiplus::Graphics g(hDC);


	//�̹��� Ư�� ��ġ �÷� �������� �ڵ�
	//Gdiplus::Color ColourOfTopLeftPixel;
	//Gdiplus::Status eStatus = Image->GetPixel(0, 0, &ColourOfTopLeftPixel);
	//_ASSERTE(eStatus == Gdiplus::Ok);

	//���� ����
	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255),
		Gdiplus::ColorAdjustTypeBitmap);

	//GIF Ȱ��ȭ
	UINT count = pImage->GetFrameDimensionsCount();
	GUID* pDimensionIDs = new GUID[count];
	pImage->GetFrameDimensionsList(pDimensionIDs, count);
	UINT frameCount = pImage->GetFrameCount(&pDimensionIDs[0]);
	UINT uIndex = m_AnimFrameIndex; //������ ��ȯ (Update���� m_Count �������ֱ�)
	pImage->SelectActiveFrame(&pDimensionIDs[0], uIndex);

	g.TranslateTransform((int)m_tInfo.fX - ((int)cameraPos.x - WINCX / 2)
		, (int)m_tInfo.fY - ((int)cameraPos.y - WINCY / 2));
	if (m_fFrontAngle == 0.f)
		g.ScaleTransform(-1.f, 1.f);

	// ĵ���� ũ�� ����
	Gdiplus::Rect tRect = Gdiplus::Rect(
		(pImage->GetWidth() * m_tInfo.fRatioY / 2) * (-1),
		(pImage->GetHeight() * m_tInfo.fRatioY / 2) * (-1),
		pImage->GetWidth() * m_tInfo.fRatioY, //���� ������ (SMALL : 2/5)
		pImage->GetHeight() * m_tInfo.fRatioY //���� ������
	);

	//Gdiplus::Matrix mat;
	//g.GetTransform(&mat);
	//�̹��� ��� (����, ���ĺ��� O)
	g.DrawImage(pImage,
		tRect, 0, 0,
		pImage->GetWidth(), pImage->GetHeight(), //�̹��� ���� ������
		Gdiplus::UnitPixel, &attr);

	//g.DrawImage(pImage,
	//	Gdiplus::Rect(
	//		((int)m_tRect.left - ((int)cameraPos.x - WINCX / 2)),
	//		((int)m_tRect.top - ((int)cameraPos.y - WINCY / 2)),
	//		pImage->GetWidth()* SMALL, //���� ������ (SMALL : 2/5)
	//		pImage->GetHeight() * SMALL //���� ������
	//	), 0, 0,
	//	pImage->GetWidth(), pImage->GetHeight(), //�̹��� ���� ������
	//	Gdiplus::UnitPixel, &attr);


	//�̹��� ��� (����, ���ĺ��� X)
	/*Gdiplus::CachedBitmap cBitMap(Image, &g);
	g.DrawCachedBitmap(&cBitMap,
		(int)m_tRect.left - ((int)cameraPos.x - WINCX / 2),
		(int)m_tRect.top - ((int)cameraPos.y - WINCY / 2),
	);*/

	/*WCHAR text[100];
	_stprintf_s(text, L"[Player] x : %f \t y : %f", m_tInfo.fX, m_tInfo.fY);
	TextOutW(hDC, m_tInfo.fX - ((int)cameraPos.x - WINCX / 2), m_tInfo.fY - ((int)cameraPos.y - WINCY / 2), text, lstrlen(text));*/

	//GIF ������ ������Ʈ
	//������ �� ����� GIF ������ �� �°� �ʱ�ȭ ����� ��
	if (m_AnimTime + 80 < GetTickCount64())
	{
		{
			++m_AnimFrameIndex;
			if (m_AnimFrameIndex > m_AnimFrame)
				m_AnimFrameIndex = 0;
		}
		m_AnimTime = GetTickCount64();
	}


}

void CObj::Update_Rect()
{
	m_tRect.left	 = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));	
	m_tRect.top		 = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	 = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom   = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
	m_Collide.left	 = m_tRect.left	   - 5/2;
	m_Collide.top    = m_tRect.top	   - 5/2;
	m_Collide.right  = m_tRect.right   + 5/2;
	m_Collide.bottom = m_tRect.bottom  + 5/2;
	
	if (m_fFrontAngle == 0)
	{
		m_FrontCollide.left = m_tInfo.fX;
		m_FrontCollide.right = m_Collide.right + m_FrontCWidth;
	}
	if (m_fFrontAngle == PI)
	{
		m_FrontCollide.right = m_tInfo.fX;
		m_FrontCollide.left = m_Collide.left - m_FrontCWidth;
	}
	m_FrontCollide.top = m_Collide.top;
	m_FrontCollide.bottom = m_Collide.bottom;
}

void CObj::AnimChange(const TCHAR* _tcAnimKey)
{
	m_AnimKey = _tcAnimKey;
	m_AnimFrame = m_AnimMap[m_AnimKey];
}

void CObj::RatioFixByImage(const TCHAR* _tcAnimKey)
{
	CBitMap* pBitMap = CBmpMgr::Get_Instance()->Find_CBitMap(m_AnimKey);
	m_tInfo.fRatioX = m_tInfo.fCX / (float)pBitMap->Get_Image()->GetWidth();
	m_tInfo.fRatioY = m_tInfo.fCY / (float)pBitMap->Get_Image()->GetHeight();
}




