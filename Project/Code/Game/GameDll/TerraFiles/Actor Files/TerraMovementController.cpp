#include "StdAfx.h"
#include "TerraMovementController.h"

CTerraMovementController::CTerraMovementController(CTerraPlayer * pPlayer)
{
	m_pPlayer = pPlayer;
}

void CTerraMovementController::Reset()
{
	m_moveRequest.RemoveDeltaMovement();
}

bool CTerraMovementController::Update(float frameTime, SActorFrameMovementParams& params)
{
	if(!m_pPlayer)
		return false;

	if(m_moveRequest.HasDeltaMovement())
		params.desiredVelocity += m_moveRequest.GetDeltaMovement();

	return true;
}

bool CTerraMovementController::GetStats(SStats& stats)
{
	stats.idle = true;

	return true;
}

void CTerraMovementController::PostUpdate(float frameTime)
{
	m_moveRequest.RemoveDeltaMovement();
}

void CTerraMovementController::Release()
{
}

void CTerraMovementController::Serialize(TSerialize &ser)
{
}

bool CTerraMovementController::RequestMovement(CMovementRequest& request)
{
	if(request.HasDeltaMovement())
		m_moveRequest.AddDeltaMovement(request.GetDeltaMovement());

	return true;
}

bool CTerraMovementController::GetStanceState(const SStanceStateQuery& query, SStanceState& state)
{
	IEntity* pEntity = m_pPlayer->GetEntity();

	state.aimDirection = Vec3(1,0,0);
	state.animationBodyDirection = Vec3(1,0,0);
	state.entityDirection = Vec3(1,0,0);
	state.eyeDirection = Vec3(1,0,0);
	state.eyePosition = Vec3(0,0,0);
	state.fireDirection = Vec3(1,0,0);
	state.lean = 0;

	Vec3 pos	= query.position.IsZero() ? pEntity->GetWorldPos() : query.position;
	state.pos	= pos;

	state.upDirection = Vec3(0,0,1);
	state.weaponPosition = Vec3(0,0,0);

	return true;
}