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

	if(m_moveRequest.HasDeltaRotation())
		params.deltaAngles += m_moveRequest.GetDeltaRotation();

	if(m_moveRequest.HasDeltaMovement())
		params.desiredVelocity += m_moveRequest.GetDeltaMovement() * m_pPlayer->GetStanceInfo(m_pPlayer->GetStance())->normalSpeed;

	if(m_moveRequest.HasPseudoSpeed())
		m_pPlayer->GetAnimationGraphState()->SetInput(m_inputPseudoSpeed, m_moveRequest.GetPseudoSpeed());
	else
		m_pPlayer->GetAnimationGraphState()->SetInput(m_inputPseudoSpeed, params.desiredVelocity.GetLength());

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

	UpdateMovementState(m_currentMovementState);
}

void CTerraMovementController::Release()
{
}

void CTerraMovementController::BindInputs(IAnimationGraphState* pAGState)
{
	m_inputPseudoSpeed = pAGState->GetInputId("PseudoSpeed");
}

void CTerraMovementController::Serialize(TSerialize &ser)
{
}

bool CTerraMovementController::RequestMovement(CMovementRequest& request)
{
	if(request.HasDeltaRotation())
		m_moveRequest.AddDeltaRotation(request.GetDeltaRotation());

	if(request.HasDeltaMovement())
		m_moveRequest.AddDeltaMovement(request.GetDeltaMovement());

	if(request.HasPseudoSpeed())
		m_moveRequest.SetPseudoSpeed(request.GetPseudoSpeed());

	return true;
}

void CTerraMovementController::UpdateMovementState(SMovementState& state)
{
	Vec3 vPlayerPos			= m_pPlayer->GetEntity()->GetWorldPos();
	state.fireTarget		= m_pPlayer->m_pAimCursor->GetWorldPos();
	state.eyePosition		= vPlayerPos + WEAPON_OFFSET;
	state.eyeDirection		= (state.fireTarget - state.eyePosition).normalized();
	state.aimDirection		= state.eyeDirection;
	state.entityDirection	= state.eyeDirection;
	state.fireDirection		= state.eyeDirection;
	state.weaponPosition	= state.eyePosition;
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