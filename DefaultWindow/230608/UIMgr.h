#pragma once
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Functor.h"
class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();
	static CUIMgr* m_pInstance;

public:
	static CUIMgr* Get_Instance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new CUIMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void		Initialize();
	void		Render(HDC hDC);
	void		Release();


	void		MouseRender(HDC hDC);
	void		UIRender(wstring _key, HDC hDC);

private:
	map<wstring,UI> m_CheckUIList;
	wstring m_Battery;
	wstring m_Inven;

public:
	void SetBattery(wstring _num);
	void SetBattery(int _num);
	void SetInven(wstring _item);
	void SetInven(ITEMTYPE _item);

};

