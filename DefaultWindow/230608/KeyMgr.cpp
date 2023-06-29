#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;


CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::Key_Down(int _iKey)
{
	// 이전에는 눌린 적이 없고 현재 눌렸을 경우
	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey]; //true
		return true;
	}

	return false;
}

bool CKeyMgr::Key_Up(int _iKey)
{
	// 이전에는 눌린 적이 있고 현재 눌리지 않았을 경우
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

void CKeyMgr::Key_LateUpdate()
{
	for (int i = 0; i < VK_MAX; ++i) //눌린적 있고 현재 눌리지 않은 것 false로?
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];

		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}
