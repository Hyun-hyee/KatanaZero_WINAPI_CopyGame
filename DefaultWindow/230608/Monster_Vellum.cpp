#include "stdafx.h"
#include "Monster_Vellum.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "Vellum_TailAttack.h"
#include "ObjFactory.h"
#include "SceneManager.h"

CMonster_Vellum::CMonster_Vellum()
	: m_eState(STATE::IDLE), m_bStateChanged(false), dwStateTimer(GetTickCount64()), dwTailAttackTimer(GetTickCount64())
{
}

CMonster_Vellum::~CMonster_Vellum()
{
	Release();
}

void CMonster_Vellum::Initialize(void)
{
#pragma region ImageLoad
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/stand.gif", L"Vellum_STAND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/move.gif", L"Vellum_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/charge.gif", L"Vellum_CHARGE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/hit1.gif", L"Vellum_HIT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/die1.gif", L"Vellum_DIE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack1.gif", L"Vellum_ATTACK1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack2.gif", L"Vellum_ATTACK2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack3.gif", L"Vellum_ATTACK3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack4.gif", L"Vellum_ATTACK4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack5.gif", L"Vellum_ATTACK5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack6.gif", L"Vellum_ATTACK6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack7.gif", L"Vellum_ATTACK7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack8.gif", L"Vellum_ATTACK8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack9.gif", L"Vellum_ATTACK9");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack10.gif", L"Vellum_ATTACK10");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack11.gif", L"Vellum_ATTACK11");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack12.gif", L"Vellum_ATTACK12");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack13.gif", L"Vellum_ATTACK13");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����/attack14.gif", L"Vellum_ATTACK14");
#pragma endregion

#pragma region ImageFrame
	m_AnimMap.insert({ L"Vellum_STAND" , 24 });
	m_AnimMap.insert({ L"Vellum_MOVE" , 10 });
	m_AnimMap.insert({ L"Vellum_CHARGE" , 52 });
	m_AnimMap.insert({ L"Vellum_HIT" , 1 });
	m_AnimMap.insert({ L"Vellum_DIE" , 49 });
	m_AnimMap.insert({ L"Vellum_ATTACK1" , 33 });
	m_AnimMap.insert({ L"Vellum_ATTACK2" , 21 });
	m_AnimMap.insert({ L"Vellum_ATTACK3" , 29 });
	m_AnimMap.insert({ L"Vellum_ATTACK4" , 28 });
	m_AnimMap.insert({ L"Vellum_ATTACK5" , 27 });
	m_AnimMap.insert({ L"Vellum_ATTACK6" , 13 });
	m_AnimMap.insert({ L"Vellum_ATTACK7" , 33 });
	m_AnimMap.insert({ L"Vellum_ATTACK8" , 32 });
	m_AnimMap.insert({ L"Vellum_ATTACK9" , 21 });
	m_AnimMap.insert({ L"Vellum_ATTACK10" , 23 });
	m_AnimMap.insert({ L"Vellum_ATTACK11" , 23 });
	m_AnimMap.insert({ L"Vellum_ATTACK12" , 32 });
	m_AnimMap.insert({ L"Vellum_ATTACK13" , 12 });
	m_AnimMap.insert({ L"Vellum_ATTACK14" , 7 });
#pragma endregion

#pragma region FunctionLinking
	m_mFnAction.emplace(STATE::IDLE, &CMonster_Vellum::Idle);
	m_mFnAction.emplace(STATE::MOVE_TO_ENEMY, &CMonster_Vellum::MoveToEnemy);
	m_mFnAction.emplace(STATE::MOVE_TO_ANYWHERE, &CMonster_Vellum::MoveToAnywhere);
	m_mFnAction.emplace(STATE::HIDE, &CMonster_Vellum::Hide);
	m_mFnAction.emplace(STATE::ATTACK_DIGOUT, &CMonster_Vellum::Attack_Digout);
	m_mFnAction.emplace(STATE::ATTACK_DIGOUT_SHORT, &CMonster_Vellum::Attack_DigoutShort);
	m_mFnAction.emplace(STATE::ATTACK_METEOR, &CMonster_Vellum::Attack_Meteor);
	m_mFnAction.emplace(STATE::ATTACK_RANGED, &CMonster_Vellum::Attack_Ranged);
	m_mFnAction.emplace(STATE::ATTACK_NEEDLE, &CMonster_Vellum::Attack_Needle);
	m_mFnAction.emplace(STATE::CHARGE, &CMonster_Vellum::Charge);
	m_mFnAction.emplace(STATE::ATTACK_BREATH, &CMonster_Vellum::Attack_Breath);
	m_mFnAction.emplace(STATE::ATTACK_BREATH_END, &CMonster_Vellum::Attack_BreathEnd);
	m_mFnAction.emplace(STATE::ATTACK_TAIL, &CMonster_Vellum::Attack_Tail);
	m_mFnAction.emplace(STATE::HIT, &CMonster_Vellum::Hit);
	m_mFnAction.emplace(STATE::DIE, &CMonster_Vellum::Die);
	m_mFnAction.emplace(STATE::SET_TARGET, &CMonster_Vellum::SetTarget);
#pragma endregion

	AnimChange(L"Vellum_STAND");
	RatioFixByImage(m_AnimKey);

	m_eState = STATE::SET_TARGET;

	m_iHP = 1000;
	m_fSpeed = 2.5f;
}

void CMonster_Vellum::Update(void)
{
	StateUpdate();
	Jump();

	__super::Update_Rect();
}

void CMonster_Vellum::LateUpdate(void)
{
}

void CMonster_Vellum::Render(HDC hDC)
{
	CCharacter::Render(hDC);
	CObj::Render(hDC);

	//ī�޶� ��ġ(����Ʈ -> �÷��̾�)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	WCHAR text[100];
	_stprintf_s(text, L"[ü��] x : %i", m_iHP);
	TextOutW(hDC, m_tInfo.fX - ((int)cameraPos.x - WINCX / 2), m_tInfo.fY - ((int)cameraPos.y - WINCY / 2), text, lstrlen(text));
}

void CMonster_Vellum::Release(void)
{
}

int CMonster_Vellum::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL) //������ ��ų
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER) //������ �÷��̾
		{
			m_iHP -= 1;

			if (STATE::DIE != m_eState)
			{
				if (m_iHP <= 0)
					StateChange(STATE::DIE);
			}
				
		}
	}

	return OBJ_NOEVENT;
}

void CMonster_Vellum::StateUpdate()
{
	m_mFnAction[m_eState](this);
}

void CMonster_Vellum::Jump()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (0.f < fY - m_tInfo.fY)
		m_bJump = true;

	if (m_bJump)
	{
		if (bLineCol && (fY < m_tInfo.fY - m_fSpeed_Vertical) && m_fSpeed_Vertical < 0.f)
		{
			m_bJump = false;
			m_tInfo.fY = fY;
			m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
		}
		else
		{
			m_tInfo.fY -= m_fSpeed_Vertical;
			m_fSpeed_Vertical -= (0.034f * G);
		}
	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
	}
	else
	{
		m_tInfo.fY -= m_fSpeed_Vertical;
		m_fSpeed_Vertical -= (0.034f * G);
	}


}



void CMonster_Vellum::Idle()
{
	AnimChange(L"Vellum_STAND");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// �ð� ������ ���� �ٲ�
	if (dwStateTimer + (ULONGLONG)300 < GetTickCount64())
	{
		// ���׿� ����
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::ATTACK_METEOR);
		}
		// ������ ��
		else
		{
			StateChange(STATE::HIDE);
		}
	}
}

void CMonster_Vellum::MoveToEnemy()
{
	AnimChange(L"Vellum_MOVE");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// Ÿ���� ���� �� Ÿ�� ������ �̵���
	if (m_pTarget)
	{
		if (0 > m_pTarget->Get_Info()->fX - m_tInfo.fX)
		{
			m_tInfo.fX -= m_fSpeed;
			m_fFrontAngle = PI;
		}
		else if (0 < m_pTarget->Get_Info()->fX - m_tInfo.fX)
		{
			m_tInfo.fX += m_fSpeed;
			m_fFrontAngle = 0.f;
		}

		// ���� �ȿ� ��� ������ ����
		if (5 > abs(m_pTarget->Get_Info()->fX - m_tInfo.fX))
		{
			// ��������
			if (m_iPatternRandom >= 7)
			{
				StateChange(STATE::ATTACK_RANGED);
			}
			// ª���� ����
			else if (m_iPatternRandom >= 3)
			{
				StateChange(STATE::ATTACK_DIGOUT_SHORT);
			}
			// �극�� ����
			else
			{
				StateChange(STATE::CHARGE);
			}
			return;
		}
	}

	// �ð� ������ ���� �ٲ�
	if (dwStateTimer + (ULONGLONG)5000 < GetTickCount64())
	{
		// ��������
		if (m_iPatternRandom >= 7)
		{
			StateChange(STATE::ATTACK_RANGED);
		}
		// �������� ����
		else if (m_iPatternRandom >= 5)
		{
			StateChange(STATE::ATTACK_TAIL);
		}
		// ª���� ����
		else if (m_iPatternRandom >= 2)
		{
			StateChange(STATE::ATTACK_DIGOUT_SHORT);
		}
		// �극�� ����
		else
		{
			StateChange(STATE::CHARGE);
		}
	}
}

void CMonster_Vellum::MoveToAnywhere()
{
	AnimChange(L"Vellum_MOVE");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	if (m_tInfo.fX >= 1720.f || m_tInfo.fX <= 200.f)
	{
		// ��������
		if (m_iPatternRandom >= 7)
		{
			StateChange(STATE::ATTACK_RANGED);
		}
		// �������� ����
		else if (m_iPatternRandom >= 5)
		{
			StateChange(STATE::ATTACK_TAIL);
		}
		// ª���� ����
		else if (m_iPatternRandom >= 2)
		{
			StateChange(STATE::ATTACK_DIGOUT_SHORT);
		}
		// �극�� ����
		else
		{
			StateChange(STATE::CHARGE);
		}
		return;
	}

	if (0 > m_iPatternRandom - 10)
	{
		m_tInfo.fX -= m_fSpeed;
		m_fFrontAngle = PI;
	}
	else if (0 < m_iPatternRandom - 10)
	{
		m_tInfo.fX += m_fSpeed;
		m_fFrontAngle = 0.f;
	}

	// �ð� ������ ���� �ٲ�
	if (dwStateTimer + (ULONGLONG)5000 < GetTickCount64())
	{
		// ��������
		if (m_iPatternRandom >= 7)
		{
			StateChange(STATE::ATTACK_RANGED);
		}
		// ª���� ����
		else if (m_iPatternRandom >= 3)
		{
			StateChange(STATE::ATTACK_DIGOUT_SHORT);
		}
		// �극�� ����
		else
		{
			StateChange(STATE::CHARGE);
		}
	}
}

void CMonster_Vellum::Hide()
{
	AnimChange(L"Vellum_ATTACK6");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// �� ������ �Ǹ� IDLE��
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		// ���׿� ����
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		// ������ ��
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CMonster_Vellum::Attack_Digout()
{
	AnimChange(L"Vellum_ATTACK2");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;

		if (m_pTarget)
		{
			if (0 > m_pTarget->Get_Info()->fCX - m_tInfo.fCX)
				m_fFrontAngle = PI;
			else
				m_fFrontAngle = 0.f;
		}
	}

	// �� ������ �Ǹ� IDLE��
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		StateChange(STATE::IDLE);
	}
}

void CMonster_Vellum::Attack_DigoutShort()
{
	AnimChange(L"Vellum_ATTACK7");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;

		if (m_pTarget)
		{
			if (0 < m_pTarget->Get_Info()->fCX - m_tInfo.fCX)
				m_fFrontAngle = PI;
			else
				m_fFrontAngle = 0.f;
		}
	}

	// �� ������ �Ǹ� IDLE��
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		// ���׿� ����
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		// ������ ��
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CMonster_Vellum::Attack_Meteor()
{
	AnimChange(L"Vellum_ATTACK5");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;

		if (m_pTarget)
		{
			if (0 > m_pTarget->Get_Info()->fCX - m_tInfo.fCX)
				m_fFrontAngle = PI;
			else
				m_fFrontAngle = 0.f;
		}
	}

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
	}

	// �ð� ������ ���� �ٲ�
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		StateChange(STATE::HIDE);
	}
}

void CMonster_Vellum::Attack_Ranged()
{
	AnimChange(L"Vellum_ATTACK8");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;

		if (m_pTarget)
		{
			if (0 > m_pTarget->Get_Info()->fCX - m_tInfo.fCX)
				m_fFrontAngle = PI;
			else
				m_fFrontAngle = 0.f;
		}
	}

	// �ִϸ��̼� ������ �ٸ� ��������
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		// ���׿� ����
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		// ������ ��
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CMonster_Vellum::Attack_Needle()
{
	AnimChange(L"Vellum_ATTACK10");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}
}

void CMonster_Vellum::Charge()
{
	AnimChange(L"Vellum_CHARGE");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;

		if (m_pTarget)
		{
			if (0 > m_pTarget->Get_Info()->fCX - m_tInfo.fCX)
				m_fFrontAngle = PI;
			else
				m_fFrontAngle = 0.f;
		}
	}

	// �ִϸ��̼� ������ �ٸ� ��������
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		StateChange(STATE::ATTACK_BREATH);
	}
}

void CMonster_Vellum::Attack_Breath()
{
	AnimChange(L"Vellum_ATTACK13");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// �ִϸ��̼� ������ �ٸ� ��������
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		StateChange(STATE::ATTACK_BREATH_END);
	}
}

void CMonster_Vellum::Attack_BreathEnd()
{
	AnimChange(L"Vellum_ATTACK14");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// �ִϸ��̼� ������ �ٸ� ��������
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		StateChange(STATE::HIDE);
	}
}

void CMonster_Vellum::Attack_Tail()
{
	AnimChange(L"Vellum_MOVE");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		dwTailAttackTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// �������� ���� ������ �Ѵ�.
	if (dwTailAttackTimer + (ULONGLONG)4000 < GetTickCount64())
	{
		dwTailAttackTimer = GetTickCount64();
		
		INFO* pTargetInfo = m_pTarget->Get_Info();
		CObj* pSkill = CObjFactory<CVellum_TailAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 500.f, 500.f);
		pSkill->SetOwner(this);
		dynamic_cast<CVellum_TailAttack*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
	}

	// ���� ����
	if (dwStateTimer + (ULONGLONG)10000 < GetTickCount64())
	{
		StateChange(STATE::MOVE_TO_ANYWHERE);
	}
}

void CMonster_Vellum::Hit()
{
	AnimChange(L"Vellum_HIT");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}
}

void CMonster_Vellum::Die()
{
	AnimChange(L"Vellum_DIE");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;

		if (m_pTarget)
		{
			if (0 > m_pTarget->Get_Info()->fCX - m_tInfo.fCX)
				m_fFrontAngle = PI;
			else
				m_fFrontAngle = 0.f;
		}
	}

	// �ִϸ��̼� ������ �ٸ� ��������
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		Set_State(DEAD);
	}
}

void CMonster_Vellum::SetTarget()
{
	AnimChange(L"Vellum_MOVE");

	// ���� ���� ����.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	if (nullptr == m_pTarget)
	{
		m_pTarget = CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front();
	}

	StateChange(STATE::ATTACK_DIGOUT);
	//StateChange(STATE::ATTACK_TAIL);
	
}
