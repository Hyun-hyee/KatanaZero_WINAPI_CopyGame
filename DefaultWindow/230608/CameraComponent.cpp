#include "stdafx.h"
#include "CameraComponent.h"
#include "Character.h"
#include "SceneManager.h"
#include "KeyMgr.h"

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Initialize()
{
}

void CameraComponent::Update()
{
	if (_owner == nullptr)
		return;

	BackSize = CSceneManager::Get_Instance()->Get_BackSize();
	
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_CONTROL))
	{

		if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		{
			pos.y -= 10.f;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		{
			pos.x -= 10.f;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		{
			pos.y += 10.f;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		{
			pos.x += 10.f;
		}
	}
	else
	{
		//오너 위치 저장
		pos = { _owner->Get_Info()->fX, _owner->Get_Info()->fY };
	}


	//카메라 배경 범위 제한
	if (pos.x <= 400.f)
		pos.x = 400.f;
	else if (pos.x >= BackSize.x - 400.f)
		pos.x = BackSize.x - 400.f;

	if (pos.y <= 300.f)
		pos.y = 300.f;
	else if (pos.y >= BackSize.y - 300.f)
		pos.y = BackSize.y - 300.f;

	CSceneManager::Get_Instance()->SetCameraPos(pos);

}

void CameraComponent::Render(HDC hdc)
{
}
