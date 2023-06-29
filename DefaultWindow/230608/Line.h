#pragma once
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Functor.h"



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

