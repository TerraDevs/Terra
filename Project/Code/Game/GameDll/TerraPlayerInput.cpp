#include "StdAfx.h"
#include "TerraPlayerInput.h"
#include "TerraPlayer.h"
#include "Game.h"
#include "IPlayerInput.h"
#include "GameActions.h"
#include "GameCVars.h"
#include "Utility/StringUtils.h"
#include "IHardwareMouse.h"
#include "Weapon.h"

CTerraPlayerInput::CTerraPlayerInput(CTerraPlayer *pPlayer):
	m_pPlayer(pPlayer),
	m_DeltaMovement(0)
{
	m_pPlayer->GetGameObject()->CaptureActions(this);
	m_pDebug = gEnv->pGameFramework->GetIPersistantDebug();
}

CTerraPlayerInput::~CTerraPlayerInput()
{
}

void CTerraPlayerInput::PreUpdate()
{
	CMovementRequest request;

	//Make a movement request
	if(!m_DeltaMovement.IsZero())
	{
		request.AddDeltaMovement(m_DeltaMovement.normalized());
		m_pPlayer->GetMovementController()->RequestMovement(request);
	}
}

void CTerraPlayerInput::Update()
{
	UpdateDebug();
}

void CTerraPlayerInput::UpdateDebug()
{
	//Draw debugging graphics
	if(g_pGameCVars->pl_debug_input)
	{
		m_pDebug->Reset();
		m_pDebug->Begin("TestAddPersistentText2D",false);

		string movementString = "DeltaMovement: " +
								ToString(m_DeltaMovement.x) + ", " +
								ToString(m_DeltaMovement.y) + ", " +
								ToString(m_DeltaMovement.z);

		float	cursorX, cursorY;
		gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&cursorX, &cursorY);

		string cursorPosString = "CursorPos: " + ToString(cursorX) + ", " + ToString(cursorY);

		m_pDebug->Add2DText(cursorPosString.c_str(), 1.0f, ColorF(1.0f, 1.0f, 1.0f), 1);
		m_pDebug->Add2DText(movementString.c_str(), 1.0f, ColorF(1.0f, 1.0f, 1.0f), 1);
	}
}

void CTerraPlayerInput::PostUpdate()
{
	m_DeltaMovement.zero();
}

void CTerraPlayerInput::SetState(const SSerializedPlayerInput& input)
{
	GameWarning("CTerraPlayerInput::SetState called: should never happen");
}

void CTerraPlayerInput::GetState(SSerializedPlayerInput& input)
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
	if(action == "moveforward")
		OnActionMoveForward(action, activationMode, value);
	else if(action == "moveback")
		OnActionMoveBack(action, activationMode, value);
	else if(action == "moveleft")
		OnActionMoveLeft(action, activationMode, value);
	else if(action == "moveright")
		OnActionMoveRight(action, activationMode, value);
	else if(action == "mouse_movex")
		OnActionMouseMoveX(action, activationMode, value);
	else if(action == "mouse_movey")
		OnActionMouseMoveY(action, activationMode, value);
	else if(action == "attack1")
		OnActionAttack1(action, activationMode, value);
}

void CTerraPlayerInput::OnActionMoveForward(const ActionId& action, int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.y	-= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.y	+= 1.0f;
}

void CTerraPlayerInput::OnActionMoveBack(const ActionId& action, int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.y	+= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.y	-= 1.0f;
}

void CTerraPlayerInput::OnActionMoveLeft(const ActionId& action, int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.x	+= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.x	-= 1.0f;
}

void CTerraPlayerInput::OnActionMoveRight(const ActionId& action, int activationMode, float value)
{
	if(activationMode == 2)
		m_DeltaMovement.x	-= 1.0f;
	else if(activationMode == 1)
		m_DeltaMovement.x	+= 1.0f;
}

void CTerraPlayerInput::OnActionMouseMoveX(const ActionId& action, int activationMode, float value)
{
}

void CTerraPlayerInput::OnActionMouseMoveY(const ActionId& action, int activationMode, float value)
{
}

void CTerraPlayerInput::OnActionAttack1(const ActionId& action, int activationMode, float value)
{
	CWeapon* pWeapon = m_pPlayer->GetWeapon(m_pPlayer->GetCurrentItemId());
	pWeapon->OnAction(m_pPlayer->GetEntityId(), action, activationMode, value);
}