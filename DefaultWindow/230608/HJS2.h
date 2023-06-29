#pragma once
#include "Scene.h"
class CHJS2 :
    public CScene
{
public:
	CHJS2();
	virtual ~CHJS2();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

private:
	bool		BossOn;
};

