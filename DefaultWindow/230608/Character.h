#pragma once
#include "Obj.h"
class Component;

class CCharacter :
    public CObj
{
public:
	CCharacter();
	virtual ~CCharacter();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

public:
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

protected:
	vector<Component*>	_components;
	
protected:
	// ���� ����
	bool				m_bJump;
	float				m_fJump_Power;
	float				m_fJump_Angle;

	// �̴� ���� ����
	int					m_iAdditionJump_MaxCount;
	int					m_iAdditionJump_Count;
};

