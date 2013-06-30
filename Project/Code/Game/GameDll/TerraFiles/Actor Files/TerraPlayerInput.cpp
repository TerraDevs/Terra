#include "StdAfx.h"
#include "TerraPlayerInput.h"
#include "TerraPlayer.h"
#include "Game.h"
#include "IPlayerInput.h"
#include "GameActions.h"

CTerraPlayerInput::CTerraPlayerInput(CTerraPlayer *pPlayer):
	m_pPlayer(pPlayer),
	m_DeltaMovement(0)
{
	m_pPlayer->GetGameObject()->CaptureActions(this);
}

CTerraPlayerInput::~CTerraPlayerInput()
{
}

void CTerraPlayerInput::PreUpdate()
{
	CMovementRequest request;

	request.AddDeltaMovement(m_DeltaMovement.normalized());
	m_pPlayer->GetMovementController()->RequestMovement(request);
}

void CTerraPlayerInput::Update()
{
}

void CTerraPlayerInput::PostUpdate()
{
}

void CTerraPlayerInput::SetState( const SSerializedPlayerInput& input )
{
	GameWarning("CTerraPlayerInput::SetState called: should never happen");
}

void CTerraPlayerInput::GetState( SSerializedPlayerInput& input )
{
}

void CTerraPlayerInput::Reset()
{
}

void CTerraPlayerInput::DisableXI(bool disabled)
{
}

uint32 CTerraPlayerInput::GetMoveButtonsState() const
{
	return 0;
}

uint32 CTerraPlayerInput::GetActions() const
{
	return 0;
}

void CTerraPlayerInput::OnAction(const ActionId& action, int activationMode, float value)
{
	CryLogAlways("%s | %d | %f", action, activationMode, value);

	if(action == "moveforward")
		OnActionMoveForward(activationMode, value);
	else if(action == "moveback")
		OnActionMoveBack(activationMode, value);
	else if(action == "moveleft")
		OnActionMoveLeft(activationMode, value);
	else if(action == "moveright")
		OnActionMoveRight(activationMode, value);
}

void CTerraPlayerInput::OnActionMoveForward(int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.y	-= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.y	+= 1.0f;
}

void CTerraPlayerInput::OnActionMoveBack(int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.y	+= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.y	-= 1.0f;
}

void CTerraPlayerInput::OnActionMoveLeft(int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.x	+= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.x	-= 1.0f;
}

void CTerraPlayerInput::OnActionMoveRight(int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.x	-= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.x	+= 1.0f;
}