#pragma once
#include "Scene.h"

class CBossStage :
    public CScene
{
public:
	CBossStage();
	virtual ~CBossStage();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

private:
	RECT		m_StartCollide;

public:
	void		CheckStart();

};

