#pragma once
#include "Scene.h"

class CObj;
class CSecondStage :
    public CScene
{
public:
	CSecondStage();
	virtual ~CSecondStage();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

	void		ReplayAndNext();

private:
	CObj*		m_pPlayer;

};

