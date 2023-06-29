#pragma once
#include "Define.h"

class CBitMap
{
public:
	CBitMap();
	~CBitMap();

public:
	HDC			Get_MemDC() { return m_hMemDC; }
	
	void		Load_Bmp(const TCHAR* pFilePath);
	void		Release();

private:
	HDC			m_hMemDC;
	/*
	HBITMAP		m_hBitMap;
	HBITMAP		m_hOldBmp;*/
	
	Gdiplus::Bitmap* 		m_hBitMap;
	Gdiplus::Bitmap* 		m_hOldBmp;

public:
	Gdiplus::Bitmap*  Get_Image() { return m_hBitMap; }

};

