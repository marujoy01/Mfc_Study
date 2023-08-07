#include "stdafx.h"
#include "AStarMgr.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CAStarMgr)

CAStarMgr::CAStarMgr()
{
}


CAStarMgr::~CAStarMgr()
{
	Release();
}

void CAStarMgr::AStar_Start(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	Release();

	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	if (vecTile.empty())
		return;

	m_iStartIdx = Get_TileIdx(vStart);

	int iGoalIdx = Get_TileIdx(vGoal);

	if (0 > m_iStartIdx || 0 >> iGoalIdx || vecTile.size() <= (size_t)m_iStartIdx || vecTile.size() <= (size_t)iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	if (true == Make_Route(m_iStartIdx, iGoalIdx))
		Make_BestList(m_iStartIdx, iGoalIdx);
}

bool CAStarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	m_BestList.push_front(vecTile[iGoalIdx]);

	int	iRouteIdx = vecTile[iGoalIdx]->iParentIdx;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		m_BestList.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIdx;
	}

	return false;
}

bool CAStarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_Adj();

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIdx);

	for (auto& iter : vecAdj[iStartIdx])
	{
		if (iGoalIdx == iter->iIndex)
		{
			iter->iParentIdx = iStartIdx;
			return true;
		}

		if (false == Check_Open(iter->iIndex) &&
			false == Check_Close(iter->iIndex))
		{
			iter->iParentIdx = iStartIdx;
			m_OpenList.push_back(iter->iIndex);
		}

	}

	if (m_OpenList.empty())
		return false;

	int	iOriginStart = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, iOriginStart](int iDst, int iSrc)
	{
		D3DXVECTOR3	vPCost1 = vecTile[iOriginStart]->vPos - vecTile[iDst]->vPos;
		D3DXVECTOR3	vPCost2 = vecTile[iOriginStart]->vPos - vecTile[iSrc]->vPos;

		D3DXVECTOR3	vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[iDst]->vPos;
		D3DXVECTOR3	vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[iSrc]->vPos;

		float	fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float	fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});
	
	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAStarMgr::Release()
{
	m_CloseList.clear();
	m_OpenList.clear();

	m_BestList.clear();
}

int CAStarMgr::Get_TileIdx(const D3DXVECTOR3 & vPos)
{
	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	if (vecTile.empty())
		return -1;
	
	for (size_t index = 0; index < vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
			return index;
	}

	return -1;
}

bool CAStarMgr::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	if (vecTile.empty())
		return false;

	D3DXVECTOR3		vPoint[4]{

		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f },
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f }

	};

	D3DXVECTOR3		vDir[4]{

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4]{

		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	};

	D3DXVECTOR3		vMouseDir[4]{

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]

	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}

	return true;
}

bool CAStarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iIndex == iCloseIdx)
			return true;
	}

	return false;
}

bool CAStarMgr::Check_Open(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iIndex == iOpenIdx)
			return true;
	}

	return false;
}
