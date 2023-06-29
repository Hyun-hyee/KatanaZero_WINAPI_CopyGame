#include "stdafx.h"
#include "Monster_ZombieMushroom.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "Vellum_TailAttack.h"
#include "ObjFactory.h"
#include "SceneManager.h"
#include "FollowBullet.h"

CMonster_ZombieMushroom::CMonster_ZombieMushroom()
	: m_eState(STATE::IDLE), m_bStateChanged(false), dwStateTimer(GetTickCount64()), dwTailAttackTimer(GetTickCount64())
{
}

CMonster_ZombieMushroom::~CMonster_ZombieMushroom()
{
	Release();
}

void CMonster_ZombieMushroom::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/학준/몬스터3/Idle.gif", L"Monster3_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/학준/몬스터3/Move.gif", L"Monster3_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/학준/몬스터3/Die.gif", L"Monster3_DIE");

	m_AnimMap.insert({ L"Monster3_IDLE" , 2 });
	m_AnimMap.insert({ L"Monster3_MOVE" , 4 });
	m_AnimMap.insert({ L"Monster3_DIE" , 3 });

#pragma region FunctionLinking
	m_mFnAction.emplace(STATE::IDLE, &CMonster_ZombieMushroom::Idle);
	m_mFnAction.emplace(STATE::MOVE_TO_ENEMY, &CMonster_ZombieMushroom::MoveToEnemy);
	m_mFnAction.emplace(STATE::MOVE_TO_ANYWHERE, &CMonster_ZombieMushroom::MoveToAnywhere);
	m_mFnAction.emplace(STATE::DIE, &CMonster_ZombieMushroom::Die);
	m_mFnAction.emplace(STATE::ATTACK_TAIL, &CMonster_ZombieMushroom::AttackTail);
	m_mFnAction.emplace(STATE::ATTACK_ANYWAY, &CMonster_ZombieMushroom::AttackAnyway);
	m_mFnAction.emplace(STATE::SET_TARGET, &CMonster_ZombieMushroom::SetTarget);
	m_mFnAction.emplace(STATE::JUMP, &CMonster_ZombieMushroom::Jump);
#pragma endregion

	AnimChange(L"Monster3_IDLE");
	RatioFixByImage(m_AnimKey);

	m_eState = STATE::SET_TARGET;

	m_iHP = 300;
	m_fSpeed = 2.5f;
}

void CMonster_ZombieMushroom::Update(void)
{
	StateUpdate();
	Gravity();

	__super::Update_Rect();
}

void CMonster_ZombieMushroom::LateUpdate(void)
{

}

void CMonster_ZombieMushroom::Render(HDC hDC)
{
	CCharacter::Render(hDC);
	CObj::Render(hDC);

	//카메라 위치(디폴트 -> 플레이어)
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();

	WCHAR text[100];
	_stprintf_s(text, L"[체력] x : %i", m_iHP);
	TextOutW(hDC, m_tInfo.fX - ((int)cameraPos.x - WINCX / 2), m_tInfo.fY - ((int)cameraPos.y - WINCY / 2), text, lstrlen(text));
}

void CMonster_ZombieMushroom::Release(void)
{
}

int CMonster_ZombieMushroom::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL) //누구랑 스킬
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER) //누구꺼 플레이어꺼
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

void CMonster_ZombieMushroom::StateUpdate()
{
	m_mFnAction[m_eState](this);
}

void CMonster_ZombieMushroom::Gravity()
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
		m_fSpeed_Vertical = 0.f;
	}
	else
	{
		m_tInfo.fY -= m_fSpeed_Vertical;
		m_fSpeed_Vertical -= (0.034f * G);
	}


}

void CMonster_ZombieMushroom::Idle()
{
	AnimChange(L"Monster3_IDLE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)300 < GetTickCount64())
	{
		// 메테오 공격
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		// 땅으로 들어가
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CMonster_ZombieMushroom::MoveToEnemy()
{
	AnimChange(L"Monster3_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// 타겟이 있을 때 타겟 쪽으로 이동함
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

		// 범위 안에 들면 패턴을 변경
		if (5 > abs(m_pTarget->Get_Info()->fX - m_tInfo.fX))
		{
			if (m_iPatternRandom >= 7)
			{
				StateChange(STATE::MOVE_TO_ANYWHERE);
			}
			else if (m_iPatternRandom >= 3)
			{
				StateChange(STATE::ATTACK_TAIL);
			}
			else
			{
				StateChange(STATE::ATTACK_ANYWAY);
			}
			return;
		}
	}

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)5000 < GetTickCount64())
	{
		if (m_iPatternRandom >= 7)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		else if (m_iPatternRandom >= 3)
		{
			StateChange(STATE::ATTACK_TAIL);
		}
		else
		{
			StateChange(STATE::ATTACK_ANYWAY);
		}
	}
}

void CMonster_ZombieMushroom::MoveToAnywhere()
{
	AnimChange(L"Monster3_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	if (m_tInfo.fX >= 1720.f || m_tInfo.fX <= 200.f)
	{
		if (m_iPatternRandom >= 7)
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
		else if (m_iPatternRandom >= 3)
		{
			StateChange(STATE::ATTACK_TAIL);
		}
		else
		{
			StateChange(STATE::ATTACK_ANYWAY);
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

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)5000 < GetTickCount64())
	{
		if (m_iPatternRandom >= 7)
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
		else if (m_iPatternRandom >= 3)
		{
			StateChange(STATE::ATTACK_TAIL);
		}
		else
		{
			StateChange(STATE::ATTACK_ANYWAY);
		}
	}
}

void CMonster_ZombieMushroom::Die()
{
	AnimChange(L"Monster3_DIE");

	// 최초 패턴 들어옴.
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

	// 애니메이션 끝나면 다른 패턴으로
	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey] - 1)
	{
		Set_State(DEAD);
	}
}

void CMonster_ZombieMushroom::AttackTail()
{
	AnimChange(L"Monster3_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		dwTailAttackTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// 무작위로 꼬리 공격을 한다.
	if (dwTailAttackTimer + (ULONGLONG)4000 < GetTickCount64())
	{
		dwTailAttackTimer = GetTickCount64();

		/*INFO* pTargetInfo = m_pTarget->Get_Info();
		CObj* pSkill = CObjFactory<CFollowBullet>::Create(pTargetInfo->fX, pTargetInfo->fY, 100.f, 100.f);
		pSkill->SetOwner(this);
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);*/
	}

	// 다음 패턴
	if (dwStateTimer + (ULONGLONG)10000 < GetTickCount64())
	{
		// 메테오 공격
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::JUMP);
		}
		// 땅으로 들어가
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CMonster_ZombieMushroom::AttackAnyway()
{
	AnimChange(L"Monster3_DIE");
	
	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		dwTailAttackTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
	}

	// 무작위로 꼬리 공격을 한다.
	if (dwTailAttackTimer + (ULONGLONG)4000 < GetTickCount64())
	{
		dwTailAttackTimer = GetTickCount64();

		/*INFO* pTargetInfo = m_pTarget->Get_Info();
		CObj* pSkill = CObjFactory<CFollowBullet>::Create(pTargetInfo->fX, pTargetInfo->fY, 100.f, 100.f);
		pSkill->SetOwner(this);
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);*/
	}

	// 다음 패턴
	if (dwStateTimer + (ULONGLONG)10000 < GetTickCount64())
	{
		// 메테오 공격
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::JUMP);
		}
		// 땅으로 들어가
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CMonster_ZombieMushroom::SetTarget()
{
	AnimChange(L"Monster3_MOVE");

	// 최초 패턴 들어옴.
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

	StateChange(STATE::IDLE);
}

void CMonster_ZombieMushroom::Jump()
{
	AnimChange(L"Monster3_DIE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_AnimFrameIndex = 0;
		m_fSpeed_Vertical = 8.f;
		m_bJump = true;
	}
	
	// 다음 패턴
	if (dwStateTimer + (ULONGLONG)5000 < GetTickCount64())
	{
		// 메테오 공격
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		// 땅으로 들어가
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}
