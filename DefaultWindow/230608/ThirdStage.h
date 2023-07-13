#pragma once
#include "Scene.h"

class CObj;

class CThirdStage :
    public CScene
{

public:
	CThirdStage();
	virtual ~CThirdStage();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

	void		ReplayAndNext();

	bool		CheckClear();

private:
	list<CObj*>* EnemyList;
	bool				m_Go;
};

