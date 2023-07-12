#include "stdafx.h"
#include "BmpMgr.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Functor.h"


CBmpMgr* CBmpMgr::m_pInstance = nullptr;
COLOR	g_ColorOverlay = COLOR::DEFAULT;

CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp_Background(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CBitMap* pBmp = new CBitMap;
		pBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pBmp });
	}
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

		//*************************Red ver*************************//
		CBitMap* pBmpR = new CBitMap;
		pBmpR->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpR->Get_Image(),340.f,1.f);

		TCHAR* KeyTemp;
		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_R", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpR });
				
		//*************************Green ver*************************//
		CBitMap* pBmpG = new CBitMap;
		pBmpG->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpG->Get_Image(), 120.f, 1.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_G", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpG });
		
		//*************************Blue ver*************************//
		CBitMap* pBmpB = new CBitMap;
		pBmpB->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpB->Get_Image(), 180.f, 1.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_B", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpB });


		//*************************Majenta ver*************************//
		CBitMap* pBmpM = new CBitMap;
		pBmpM->Load_Bmp(pFilePath);
		//AlphaBlending(pBmpM->Get_Image());
		ConvertToColorOverlay(pBmpM->Get_Image(), 300.f, 1.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_M", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpM });
		
		pBmpM = new CBitMap;
		pBmpM->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpM->Get_Image(), 300.f, 0.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_M1", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpM });
		
		pBmpM = new CBitMap;
		pBmpM->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpM->Get_Image(), 300.f, 0.3f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_M2", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpM });
		
		pBmpM = new CBitMap;
		pBmpM->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpM->Get_Image(), 300.f, 0.6f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_M3", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpM });
		
		pBmpM = new CBitMap;
		pBmpM->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpM->Get_Image(), 120.f, 3.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_M4", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpM });

		pBmpM = new CBitMap;
		pBmpM->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpM->Get_Image(), 130.f, 3.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_M5", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpM });

		//*************************Yellow ver*************************//
		CBitMap* pBmpY = new CBitMap;
		pBmpY->Load_Bmp(pFilePath);
		ConvertToColorOverlay(pBmpY->Get_Image(), 60.f, 1.f);

		Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), L"_Y", KeyTemp);
		KeyList.push_back(KeyTemp);
		m_mapBit.insert({ KeyList.back(), pBmpY });
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

	for (auto& iter : KeyList)
	{
		delete[] iter;
		iter = nullptr;
	}
	KeyList.clear();
}

Gdiplus::Bitmap* CBmpMgr::Find_Img(const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_Image();
}

CBitMap* CBmpMgr::Find_CBitMap(const TCHAR* pImgKey) //이거씀!
{
	TCHAR* Temp;
	
	Add_TCHAR_wstring(const_cast<TCHAR*>(pImgKey), COLORKEY[g_ColorOverlay], Temp);

	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(Temp));

	if (iter == m_mapBit.end())
		return nullptr;

	delete[] Temp;
	return iter->second;
}

CBitMap* CBmpMgr::Find_CBitMap_Background(const TCHAR* pImgKey) 
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second;
}

void CBmpMgr::Add_TCHAR_wstring(TCHAR* _tc, wstring _ws, TCHAR* & Combine)
{
	// TCHAR*을 wstring으로 변환
	wstring tc = _tc;

	// 두 개의 wstring을 결합
	wstring Temp = tc + _ws;

	// 결합된 wstring을 TCHAR*로 변환
	size_t size = Temp.size() + 1;
	Combine = new TCHAR[size];
#ifdef UNICODE
	wcscpy_s(Combine, size, Temp.c_str());
#else
	strcpy_s(Combine, size, Temp.c_str());
#endif

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
			if (reinterpret_cast<uintptr_t>(pixel) != 0xCCCCCCCC)
			{
				if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255) {
					pixel[3] = 0;
				} // 알파 값을 0으로 설정
			}


			
			
		}
	}
	

	bitmap->UnlockBits(&bitmapData);
}




void CBmpMgr::ConvertToColorOverlay(Gdiplus::Bitmap* bitmap, float _h, float _s)
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

			// 특정 컬러(255, 0, 255)를 투명하게 만들기
			if (pixel[2] == 255 && pixel[1] == 0 && pixel[0] == 255) {
				pixel[3] = 0; // 알파 값을 0으로 설정
			}
			else {
				// RGB 값을 HSV로 변환
				float h, s, v;
				RGBToHSV(pixel[2], pixel[1], pixel[0], h, s, v);

				// Hue 및 Saturation 값 조정
				h = _h;
				s = _s;

				// HSV 값을 RGB로 변환하여 적용
				BYTE r, g, b;
				HSVToRGB(h, s, v, r, g, b);
				pixel[0] = b;
				pixel[1] = g;
				pixel[2] = r;
			}
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