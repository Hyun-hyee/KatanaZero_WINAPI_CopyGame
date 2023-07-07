#include "stdafx.h"
#include "UIMgr.h"
#include "BmpMgr.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
{
	m_Battery = L"BATTERY_UI_9";
	m_Inven = L"INVEN_DEFAULT";
}

CUIMgr::~CUIMgr()
{
}



void CUIMgr::Render(HDC hDC)
{
	//SetBattery(L"3");
	//SetInven(L"OILBOTTLE");
	MouseRender(hDC);
	UIRender(L"TOP_UI",hDC);
	UIRender(m_Battery, hDC);
	UIRender(m_Inven, hDC);

}

void CUIMgr::Release()
{
}


void CUIMgr::MouseRender(HDC hDC)
{
	//����� �̹��� Key ��������
	Gdiplus::Bitmap* pImage = CBmpMgr::Get_Instance()->Find_Img(L"MOUSE");
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);	// ���콺 ��ġ ���� ������ �Լ�

	ScreenToClient(g_hWnd, &ptMouse); // ��ũ�� ���� ��ǥ�� �츮�� ������ â ��ǥ�� ��ȯ

	Gdiplus::Graphics g(hDC);

	Gdiplus::ImageAttributes attr;
	attr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255),
		Gdiplus::ColorAdjustTypeBitmap);

	g.DrawImage(pImage,
		Gdiplus::Rect(
			ptMouse.x - 25 * SMALL,
			ptMouse.y - 25 * SMALL,
			50 * SMALL, //���� ������
			50 * SMALL //���� ������
		),
		0,
		0,
		50,
		50, //�̹��� ���� ������
		Gdiplus::UnitPixel, &attr);
}

void CUIMgr::UIRender(wstring _key, HDC hDC)
{
	//����� �̹��� Key ��������
	Gdiplus::Bitmap* pImage = CBmpMgr::Get_Instance()->Find_Img((wchar_t*)_key.c_str());
	
	Gdiplus::Graphics g(hDC);

	//�̹��� ��� (����, ���ĺ��� X)
	g.DrawImage(pImage,
		Gdiplus::Rect(
			m_CheckUIList[_key].PosX,
			m_CheckUIList[_key].PosY,
			m_CheckUIList[_key].SizeX, //���� ������
			m_CheckUIList[_key].SizeY//���� ������
		),
		0,
		0,
		pImage->GetWidth(),
		pImage->GetHeight(), //�̹��� ���� ������
		Gdiplus::UnitPixel);
}

void CUIMgr::SetBattery(wstring _num)
{
	wstring Temp = L"BATTERY_UI_";
	m_Battery = Temp + _num;
}

void CUIMgr::SetBattery(int _num)
{
	wstring Temp = L"BATTERY_UI_";
	m_Battery = Temp + to_wstring(_num);
}

void CUIMgr::SetInven(wstring _item)
{
	wstring Temp = L"INVEN_";
	m_Inven = Temp + _item;
}

void CUIMgr::SetInven(ITEMTYPE _item)
{
	wstring Temp = L"INVEN_";
	m_Inven = Temp + ItemType[_item];
}



void CUIMgr::Initialize()
{
	//���콺 �̹��� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/cursor.bmp", L"MOUSE");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/HUD.png", L"TOP_UI");
	UI TempUI = { L"TOP_UI" , 0, 0, WINCX, 36, true };
	m_CheckUIList.insert({ L"TOP_UI" ,TempUI });

	//���͸� �ܰ躰 �̹���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/0.png", L"BATTERY_UI_0");
	TempUI = { L"BATTERY_UI_0", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_0" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/1.png", L"BATTERY_UI_1");
	TempUI = { L"BATTERY_UI_1", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_1" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/2.png", L"BATTERY_UI_2");
	TempUI = { L"BATTERY_UI_2", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_2" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/3.png", L"BATTERY_UI_3");
	TempUI = { L"BATTERY_UI_3", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_3" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/4.png", L"BATTERY_UI_4");
	TempUI = { L"BATTERY_UI_4", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_4" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/5.png", L"BATTERY_UI_5");
	TempUI = { L"BATTERY_UI_5", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_5" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/6.png", L"BATTERY_UI_6");
	TempUI = { L"BATTERY_UI_6", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_6" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/7.png", L"BATTERY_UI_7");
	TempUI = { L"BATTERY_UI_7", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_7" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/8.png", L"BATTERY_UI_8");
	TempUI = { L"BATTERY_UI_8", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_8" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Battery/9.png", L"BATTERY_UI_9");
	TempUI = { L"BATTERY_UI_9", 0, 3, 123, 30, true };
	m_CheckUIList.insert({ L"BATTERY_UI_9" ,TempUI });

	//�κ��丮 �ڽ�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Inven/0.png", L"INVEN_DEFAULT");
	TempUI = { L"INVEN_DEFAULT", WINCX - 80, 3, 70, 30, true };
	m_CheckUIList.insert({ L"INVEN_DEFAULT" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Inven/1.png", L"INVEN_SWORD");
	TempUI = { L"INVEN_DEFAULT", WINCX - 80, 3, 70, 30, true };
	m_CheckUIList.insert({ L"INVEN_SWORD" ,TempUI });

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Inven/3.png", L"INVEN_OILBOTTLE");
	TempUI = { L"INVEN_DEFAULT", WINCX - 80, 3, 70, 30, true };
	m_CheckUIList.insert({ L"INVEN_OILBOTTLE" ,TempUI });


}

