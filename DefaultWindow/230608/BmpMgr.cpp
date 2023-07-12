#include "stdafx.h"
#include "BmpMgr.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Functor.h"


CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CBitMap* pBmp = new CBitMap;
		pBmp->Load_Bmp(pFilePath);

		AlphaBlending(pBmp->Get_Image());
		m_mapBit.insert({ pImgKey, pBmp });

		//Gdiplus::Bitmap* Red = CloneBitmap(pBmp->Get_Image());
		
		CBitMap* pBmp2 = new CBitMap;
		pBmp2->Load_Bmp(pFilePath);
		AlphaBlending(pBmp2->Get_Image());
		ConvertToBlueOverlay(pBmp2->Get_Image());

		const TCHAR* RedKey = Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_R");
		m_mapBit.insert({ RedKey, pBmp2});
	}

}
//
//HDC CBmpMgr::Find_Img(const TCHAR* pImgKey)
//{
//	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));
//
//	if (iter == m_mapBit.end())
//		return nullptr;
//
//	return iter->second->Get_MemDC();
//}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();
}

Gdiplus::Bitmap* CBmpMgr::Find_Img(const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_Image();
}

CBitMap* CBmpMgr::Find_CBitMap(const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second;
}

void CBmpMgr::AlphaBlending(Gdiplus::Bitmap* bitmap)
{
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	for (int y = 0; y < bitmapData.Height; y++) {
		BYTE* row = reinterpret_cast<BYTE*>(bitmapData.Scan0) + (y * bitmapData.Stride);

		for (int x = 0; x < bitmapData.Width; x++) {
			BYTE* pixel = row + (x * 4);

			// 특정 컬러(255, 0, 255)를 투명하게 만들기
			if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255) {
				pixel[3] = 0; // 알파 값을 0으로 설정
			}
		}
	}

	bitmap->UnlockBits(&bitmapData);
}

TCHAR* CBmpMgr::Add_TCHAR_wstring(TCHAR* _tc, wstring _ws)
{
	// TCHAR*을 wstring으로 변환
	wstring tc = _tc;

	// 두 개의 wstring을 결합
	wstring Temp = tc + _ws;

	// 결합된 wstring을 TCHAR*로 변환
	size_t size = Temp.size() + 1;
	TCHAR* tCharStr = new TCHAR[size];
#ifdef UNICODE
	wcscpy_s(tCharStr, size, Temp.c_str());
#else
	strcpy_s(tCharStr, size, Temp.c_str());
#endif

	return tCharStr;
}


void CBmpMgr::ConvertToBlueOverlay(Gdiplus::Bitmap* bitmap)
{
	// 이미지 크기 얻기
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();

	// 비트맵 데이터 가져오기
	Gdiplus::BitmapData bmpData;
	bitmap->LockBits(&Gdiplus::Rect(0, 0, width, height), Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
	BYTE* pixelData = reinterpret_cast<BYTE*>(bmpData.Scan0);

	// 픽셀 데이터 순회
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			BYTE* pixel = pixelData + (y * bmpData.Stride) + (x * 4);

			// RGB 값을 HSV로 변환
			float h, s, v;
			RGBToHSV(pixel[2], pixel[1], pixel[0], h, s, v);

			// 파란색에 해당하는 Hue 값으로 설정
			//h = 240.0f;
			h = 340.0f;

			// HSV 값을 RGB로 변환하여 적용
			BYTE r, g, b;
			HSVToRGB(h, s, v, r, g, b);
			pixel[0] = b;
			pixel[1] = g;
			pixel[2] = r;
		}
	}

	// 비트맵 잠금 해제
	bitmap->UnlockBits(&bmpData);




}


Gdiplus::Bitmap* CBmpMgr::CloneBitmap(Gdiplus::Bitmap* sourceBitmap)
{
	// 현재 비트맵을 완전히 복사하여 새로운 비트맵을 생성
	Gdiplus::Bitmap* clonedBitmap = sourceBitmap->Clone(0, 0, sourceBitmap->GetWidth(), sourceBitmap->GetHeight(), sourceBitmap->GetPixelFormat());

	return clonedBitmap;
}


void CBmpMgr::RGBToHSV(BYTE r, BYTE g, BYTE b, float& h, float& s, float& v)
{
	float minVal = min(min(r, g), b);
	float maxVal = max(max(r, g), b);
	float delta = maxVal - minVal;

	// Hue 계산
	if (delta == 0)
		h = 0;
	else if (maxVal == r)
		h = 60 * ((g - b) / (int)delta % 6);
	else if (maxVal == g)
		h = 60 * ((b - r) / delta + 2);
	else if (maxVal == b)
		h = 60 * ((r - g) / delta + 4);

	// Saturation 계산
	if (maxVal == 0)
		s = 0;
	else
		s = delta / maxVal;

	// Value 계산
	v = maxVal;
}

void CBmpMgr::HSVToRGB(float h, float s, float v, BYTE& r, BYTE& g, BYTE& b)
{
	if (s == 0)
	{
		r = g = b = static_cast<BYTE>(v);
		return;
	}

	float hue = h / 60;
	int i = static_cast<int>(hue);
	float f = hue - i;
	float p = v * (1 - s);
	float q = v * (1 - s * f);
	float t = v * (1 - s * (1 - f));

	switch (i)
	{
	case 0:
		r = static_cast<BYTE>(v);
		g = static_cast<BYTE>(t);
		b = static_cast<BYTE>(p);
		break;
	case 1:
		r = static_cast<BYTE>(q);
		g = static_cast<BYTE>(v);
		b = static_cast<BYTE>(p);
		break;
	case 2:
		r = static_cast<BYTE>(p);
		g = static_cast<BYTE>(v);
		b = static_cast<BYTE>(t);
		break;
	case 3:
		r = static_cast<BYTE>(p);
		g = static_cast<BYTE>(q);
		b = static_cast<BYTE>(v);
		break;
	case 4:
		r = static_cast<BYTE>(t);
		g = static_cast<BYTE>(p);
		b = static_cast<BYTE>(v);
		break;
	default:
		r = static_cast<BYTE>(v);
		g = static_cast<BYTE>(p);
		b = static_cast<BYTE>(q);
		break;
	}
}