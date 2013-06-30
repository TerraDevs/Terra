#ifndef __TERRAPLAYERINPUT_H__
#define __TERRAPLAYERINPUT_H__

#pragma once

#include "IActionMapManager.h"
#include "IPlayerInput.h"
#include "TerraPlayer.h"
#include "DebugHelper.h"

class CPlayer;
struct SPlayerStats;

class CTerraPlayerInput : public IPlayerInput, public IActionListener
{
public:

	CTerraPlayerInput(CTerraPlayer *pPlayer);
	~CTerraPlayerInput();

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	virtual void SetState( const SSerializedPlayerInput& input );
	virtual void GetState( SSerializedPlayerInput& input );

	virtual void Reset();
	virtual void DisableXI(bool disabled);

	virtual EInputType GetType() const { return PLAYER_INPUT; }

	virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

	virtual uint32 GetMoveButtonsState() const;
	virtual uint32 GetActions() const;
	
	virtual void OnAction(const ActionId& action, int activationMode, float value);
	virtual void OnActionMoveForward(int activationMode, float value);
	virtual void OnActionMoveBack(int activationMode, float value);
	virtual void OnActionMoveLeft(int activationMode, float value);
	virtual void OnActionMoveRight(int activationMode, float value);
	virtual void OnActionMouseMoveX(int activationMode, float value);
	virtual void OnActionMouseMoveY(int activationMode, float value);

protected:
	Vec3			m_DeltaMovement;
	Ang3			m_DeltaRotation;
	CTerraPlayer*	m_pPlayer;
};

#endif