#pragma once

#include "BitMap.h"

class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	//HDC			Find_Img(const TCHAR* pImgKey);
	void		Release();

	Gdiplus::Bitmap*  Find_Img(const TCHAR* pImgKey);
	CBitMap* Find_CBitMap(const TCHAR* pImaKey);

	void AlphaBlending(Gdiplus::Bitmap* bitmap);

public:
	static CBmpMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBmpMgr;
		}

		return m_pInstance;
	}
	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CBmpMgr* m_pInstance;
	map<const TCHAR*, CBitMap*>			m_mapBit;

};

