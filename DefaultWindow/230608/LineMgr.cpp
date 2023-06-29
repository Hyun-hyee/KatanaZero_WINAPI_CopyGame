#include "stdafx.h"
#include "LineMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& iter : m_LineList)
	{
		iter->Render(hDC);
	}
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), CDeleteObj());
	m_LineList.clear();
}

void CLineMgr::Add_Line(LINEPOINT _Lpoint, LINEPOINT _Rpoint)
{
	m_LineList.push_back(new CLine(_Lpoint, _Rpoint));
}

bool CLineMgr::CollisionLine(INFO& _rInfo, float* _pY)
{
	if (m_LineList.empty())
		return false;

	list<CLine*>		TargetList;
	CLine* pTarget = nullptr;
	// 가장 아래에 있는 점에 대한 높이 길이
	float	fBottomY = -INFINITY;
	float	fHeight = 0.f;// _rInfo.fCY * 0.5f;

	// 특정 라인의 x축 사이에 있는 라인을 찾기
	for (auto& line : m_LineList)
	{
		if (line->Get_Info().tLpoint.fX <= _rInfo.fX && line->Get_Info().tRpoint.fX > _rInfo.fX ||
			line->Get_Info().tRpoint.fX <= _rInfo.fX && line->Get_Info().tLpoint.fX > _rInfo.fX)
		{
			TargetList.push_back(line);

			// 여기서 fBaseH는 초기 높이를 가장 크게 설정하기위함으로
			// 중심좌표 바로 아래의 점에 대한 계산은 아님.
			if (fBottomY < line->Get_Info().tLpoint.fY - fHeight)
			{
				fBottomY = line->Get_Info().tLpoint.fY - fHeight;
				pTarget = line;
			}

			if (fBottomY < line->Get_Info().tRpoint.fY - fHeight)
			{
				fBottomY = line->Get_Info().tRpoint.fY - fHeight;
				pTarget = line;
			}
		}
	}

	// 아래에 해당하는 라인을 저장한 리스트를 체크한다.
	if (TargetList.empty())
		return false;

	// 뽑은 리스트 중에서 가장 가까운 라인을 찾아 걸러낸다.
	for (auto& line : TargetList)
	{
		float	x1 = line->Get_Info().tLpoint.fX;
		float	x2 = line->Get_Info().tRpoint.fX;

		float	y1 = line->Get_Info().tLpoint.fY - fHeight;
		float	y2 = line->Get_Info().tRpoint.fY - fHeight;

		float	fTargetY = ((y2 - y1) / (x2 - x1)) * (_rInfo.fX - x1) + y1;
		float	fPrevTargetY = ((y2 - y1) / (x2 - x1)) * (_rInfo.fPrevX - x1) + y1;

		// 걸러내는 코드
		if (fTargetY <= fBottomY && fTargetY >= _rInfo.fY)
		{
			fBottomY = fTargetY;

			pTarget = line;
		}

		else if (fPrevTargetY <= fBottomY && fPrevTargetY > fTargetY &&
			(_rInfo.fY > y1 && _rInfo.fY < y2) || (_rInfo.fY > y2 && _rInfo.fY < y1))
		{
			fBottomY = fTargetY;

			pTarget = line;
		}
	}

	if (!pTarget)
		return false;


	//추가) 타겟 라인이 캐릭터 높이와 차이가 크면 false
	if (abs(pTarget->Get_Info().tLpoint.fY - _rInfo.fY) > _rInfo.fCY)
		return false;


	float	x1 = pTarget->Get_Info().tLpoint.fX;
	float	x2 = pTarget->Get_Info().tRpoint.fX;

	float	y1 = pTarget->Get_Info().tLpoint.fY - fHeight;
	float	y2 = pTarget->Get_Info().tRpoint.fY - fHeight;

	// 비율 * 변환할 너비 + 선의 원점 y값
	*_pY = ((y2 - y1) / (x2 - x1)) * (_rInfo.fX - x1) + y1;

	return true;
}

void CLineMgr::Change_Scene()
{
	Release();
}
