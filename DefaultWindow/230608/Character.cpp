#include "stdafx.h"
#include "Character.h"
#include "Component.h"


CCharacter::CCharacter() : 
  m_bJump(false), m_fJump_Power(0.f), m_fJump_Angle(0.f)
, m_iAdditionJump_MaxCount(0), m_iAdditionJump_Count(0)
{
}

CCharacter::~CCharacter()
{
	for (Component* component : _components)
	{
		delete component;
		component = nullptr;
	}
}

void CCharacter::Initialize()
{
	for (Component* component : _components)
	{
		component->Initialize();
	}
}

void CCharacter::Update()
{
	for (Component* component : _components)
	{
		component->Update();
	}
}

void CCharacter::LateUpdate(void)
{
}

void CCharacter::Render(HDC hdc)
{
	for (Component* component : _components)
	{
		component->Render(hdc);
	}
}

void CCharacter::Release(void)
{

}

int CCharacter::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}

void CCharacter::AddComponent(Component* component)
{
	if (component == nullptr)
		return;

	component->SetOwner(this);
	_components.push_back(component);
}

void CCharacter::RemoveComponent(Component* component)
{
	auto findIt = std::find(_components.begin(), _components.end(), component);
	if (findIt == _components.end())
		return;

	_components.erase(findIt);
}



