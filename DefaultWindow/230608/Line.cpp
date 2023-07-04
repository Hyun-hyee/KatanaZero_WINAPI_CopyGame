#include "stdafx.h"
#include "Line.h"
#include "SceneManager.h"

CLine::CLine()
{
}

CLine::CLine(LINEPOINT _tLpoint, LINEPOINT _tRpoint)
	: m_tInfo(_tLpoint, _tRpoint)
{
}

CLine::~CLine()
{
}

void CLine::Render(HDC hDC)
{
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

	HPEN myPen = CreatePen(PS_SOLID, 0, RGB(0, 255, 255));
	HPEN oldPen = (HPEN)SelectObject(hDC, myPen);

	MoveToEx(hDC, (int)m_tInfo.tLpoint.fX - ((float)cameraPos.x - WINCX / 2)
				, (int)m_tInfo.tLpoint.fY - ((float)cameraPos.y - WINCY / 2) + 25.f, nullptr);
	LineTo(hDC, (int)m_tInfo.tRpoint.fX - ((float)cameraPos.x - WINCX / 2),
				(int)m_tInfo.tRpoint.fY - ((float)cameraPos.y - WINCY / 2) + 25.f);


	//Rectangle(hDC, m_tRect.left - ((float)cameraPos.x - WINCX / 2),
	//	m_tRect.top - ((float)cameraPos.y - WINCY / 2),
	//	m_tRect.right - ((float)cameraPos.x - WINCX / 2),
	//	m_tRect.bottom - ((float)cameraPos.y - WINCY / 2));


	SelectObject(hDC, oldBrush);
	DeleteObject(myBrush);

	SelectObject(hDC, oldPen);
	DeleteObject(myPen);

}
