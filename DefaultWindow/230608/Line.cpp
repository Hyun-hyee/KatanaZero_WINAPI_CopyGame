#include "stdafx.h"
#include "Line.h"

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
	MoveToEx(hDC, (int)m_tInfo.tLpoint.fX, (int)m_tInfo.tLpoint.fY, nullptr);
	LineTo(hDC, (int)m_tInfo.tRpoint.fX, (int)m_tInfo.tRpoint.fY);
}
