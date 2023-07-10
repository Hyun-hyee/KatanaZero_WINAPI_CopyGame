#pragma once
#include "Define.h"

class CScene
{
public:
	CScene() : PrevScene(nullptr), NextScene(nullptr), m_ReplayOn(false)
	{
		m_bSceneOn = false;
		m_BackGroundKey = L"";
	};
	virtual ~CScene() {};

public:
	virtual void		Initialize()		PURE;
	virtual void		Update()			PURE;
	virtual void		LateUpdate()		PURE;
	virtual void		Render(HDC _hDC)	PURE;
	virtual void		Release()			PURE;

protected:
	bool	m_bSceneOn;
	CScene* PrevScene;
	CScene* NextScene;
	TCHAR*	m_BackGroundKey;

	bool	m_ReplayOn;

public:
	void	Set_SceneOn(bool _SceneOn) { m_bSceneOn = _SceneOn; }
	bool	Get_SceneOn() { return m_bSceneOn; }

	void	Set_PrevScene(CScene* _scene) { PrevScene = _scene; }
	void	Set_NextScene(CScene* _scene) { NextScene = _scene; }

	CScene* Get_PrevScene() { return PrevScene; }
	CScene* Get_NextScene() { return NextScene; }

	void	BackGroundRender(HDC hDC);
	void 	BackGroundRender(HDC hDC, int _x, int _y, TCHAR* _BackKey);
	void	Set_BackGroundKey(TCHAR* _key) { m_BackGroundKey = _key; }


};

