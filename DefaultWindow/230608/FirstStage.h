#pragma once
#include "Scene.h"

class CObj;

class CFirstStage :
    public CScene
{
public:
	CFirstStage();
	virtual ~CFirstStage();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

	void		ReplayAndNext();

private:
	list<CObj*>*		m_EnemyList;
	bool				m_Go;
};

