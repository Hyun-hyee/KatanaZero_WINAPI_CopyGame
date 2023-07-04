#include "stdafx.h"
#include "Item.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SoundMgr.h"

CItem::CItem()
{
	bThrow = false;
	m_Type = ITEM;
	m_Collider_type = RECTANGLE;
	m_ItemType = ITEM_NONE;
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_fSpeed = 30.f;
	SetCollideSize(40.f);
	InitImage();
}

void CItem::Update()
{
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
	
	if (bThrow)
	{
		m_tInfo.fX += cos(m_fAttackAngle) * m_fSpeed;
		m_tInfo.fY -= sin(m_fAttackAngle) * m_fSpeed;
	}
}

void CItem::LateUpdate(void)
{
}

void CItem::Render(HDC hdc)
{
	CollideRender(hdc);
	if(m_ItemType != ITEM_NONE)
		CObj::BasicRender(hdc);
}

void CItem::Release(void)
{
}

void CItem::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Item/spr_butcher_knife/0.png", L"Item_SWORD_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Item/spr_butcher_knife/1.png", L"Item_SWORD_1");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Item/spr_explosive_vial/0.png", L"Item_OILBOTTLE_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Texture/Item/spr_explosive_vial/1.png", L"Item_OILBOTTLE_1");
}

int CItem::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();
	if (bThrow)
	{
		if (targetType == WALL || targetType == GRABWALL || targetType == ENEMY)
		{
			if (m_ItemType == SWORD)
			{
				if (targetType == ENEMY)
				{
					if (_target->Get_State() != HURT && _target->Get_State() != HURTGROUND)
						Set_State(DEAD);
				}
				else
				{
					bThrow = false;
					SetCollideSize(40.f);
				}
				CSoundMgr::Get_Instance()->PlaySound(L"swordcrash.wav", SOUND_EFFECT, SOUND_VOL3);
			}
			else if (m_ItemType == OILBOTTLE)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"glass1.wav", SOUND_EFFECT, SOUND_VOL3);
				Set_State(DEAD);
			}
		}


	}
	else
	{
		if (targetType == PLAYER)
		{
			if (m_ItemType == SWORD)
				m_OneImgKey = L"Item_SWORD_1";
			else if (m_ItemType == OILBOTTLE)
				m_OneImgKey = L"Item_OILBOTTLE_1";
		}
	}
	return 0;
}

int CItem::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();
	if(!bThrow)
	{
		if (targetType == PLAYER)
		{
			if (m_ItemType == SWORD)
				m_OneImgKey = L"Item_SWORD_0";
			else if (m_ItemType == OILBOTTLE)
				m_OneImgKey = L"Item_OILBOTTLE_0";
		}
	}
	return 0;
}

int CItem::OnCollision(CObj* _target)
{
	return 0;
}

void CItem::SetITemType(ITEM_TYPE _type)
{
	m_ItemType = _type;
	if (m_ItemType == SWORD)
		m_OneImgKey = L"Item_SWORD_0";
	else if (m_ItemType == OILBOTTLE)
		m_OneImgKey = L"Item_OILBOTTLE_0";
}
