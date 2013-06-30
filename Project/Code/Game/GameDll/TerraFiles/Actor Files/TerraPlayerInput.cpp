#include "StdAfx.h"
#include "TerraPlayerInput.h"
#include "TerraPlayer.h"
#include "Game.h"
#include "IPlayerInput.h"
#include "GameActions.h"
#include "GameCVars.h"

CTerraPlayerInput::CTerraPlayerInput(CTerraPlayer *pPlayer):
	m_pPlayer(pPlayer),
	m_DeltaMovement(0),
	m_DeltaRotation(0.0f, 0.0f, 0.0f)
{
	m_pPlayer->GetGameObject()->CaptureActions(this);

	DEBUG_DRAW_INIT(0.0f);
}

CTerraPlayerInput::~CTerraPlayerInput()
{
}

void CTerraPlayerInput::PreUpdate()
{
	CMovementRequest request;

	//Make a movement request
	//request.AddDeltaRotation(m_DeltaRotation);
	m_DeltaRotation.Set(0.0f, 0.0f, 0.0f);

	request.AddDeltaMovement(m_DeltaMovement.normalized());
	m_pPlayer->GetMovementController()->RequestMovement(request);
}

void CTerraPlayerInput::Update()
{
	//Draw debugging graphics
	if(g_pGameCVars->pl_debug_input)
	{
		DEBUG_DRAW_ANG3(m_DeltaRotation);
		DEBUG_DRAW_VEC3(m_DeltaMovement);

		/*IPersistantDebug* debug = gEnv->pGameFramework->GetIPersistantDebug();
		debug->Reset();
		debug->Begin("TestAddPersistentText2D",false);
		debug->Add2DText(, 1, ColorF(1.0f, 1.0f, 1.0f), 5);*/
	}
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
	else if(action == "mouse_movex")
		OnActionMouseMoveX(activationMode, value);
	else if(action == "mouse_movey")
		OnActionMouseMoveY(activationMode, value);
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

void CTerraPlayerInput::OnActionMouseMoveX(int activationMode, float value)
{
	m_DeltaRotation.z += value;
}

void CTerraPlayerInput::OnActionMouseMoveY(int activationMode, float value)
{
	m_DeltaRotation.x += value;
}