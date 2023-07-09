#pragma once
#include "Scene.h"

class CBossStage_1 :
    public CScene
{
public:
	CBossStage_1();
	virtual ~CBossStage_1();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

private:
	RECT		m_StartCollide;
	DWORD		m_ExplosionTime;
	bool		m_ExplosionOn;

public:
	void		CheckStart();
};

