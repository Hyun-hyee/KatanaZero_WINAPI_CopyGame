#pragma once
#include "Define.h"


class CLine
{
public:
	CLine();
	CLine(LINEPOINT _tLpoint, LINEPOINT _tRpoint);
	~CLine();

private:
	LINE m_tInfo;

public:
	void Render(HDC hDC);
	LINE Get_Info() { return m_tInfo; }




};

