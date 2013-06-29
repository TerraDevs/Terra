#include "StdAfx.h"
#include "TerraPlayerInput.h"
#include "TerraPlayer.h"
#include "Game.h"
#include "IPlayerInput.h"
#include "GameActions.h"

CTerraPlayerInput::CTerraPlayerInput(CTerraPlayer *pPlayer):m_pPlayer(pPlayer)
{
	//this makes the input class hook up to the input feeder
	m_pPlayer->GetGameObject()->CaptureActions(this);
}

CTerraPlayerInput::~CTerraPlayerInput()
{
}

void CTerraPlayerInput::PreUpdate()
{
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
	CryLogAlways("%s | %d", action, activationMode);
};