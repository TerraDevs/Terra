#include "StdAfx.h"
#include "TerraMovementController.h"

//here i have just looked at the original functions and copied the things that looks very basic, it's just a fraction of the original class.
CTerraMovementController::CTerraMovementController( CTerraPlayer * pPlayer )
{

}

void CTerraMovementController::Reset()
{

}

bool CTerraMovementController::Update( float frameTime, SActorFrameMovementParams& params )
{
	params.lookTarget = Vec3(-1,-1,-1);
	params.aimTarget = Vec3(-1,-1,-1);

	return true;
}

bool CTerraMovementController::GetStats(SStats& stats)
{
	stats.idle = true;
	return true;
}

void CTerraMovementController::PostUpdate( float frameTime )
{
}

void CTerraMovementController::Release()
{

}

void CTerraMovementController::Serialize(TSerialize &ser)
{

}

bool CTerraMovementController::RequestMovement( CMovementRequest& request )
{
	return true;
}

bool CTerraMovementController::GetStanceState( const SStanceStateQuery& query, SStanceState& state )
{
	state.aimDirection = Vec3(1,0,0);
	state.animationBodyDirection = Vec3(1,0,0);
	state.entityDirection = Vec3(1,0,0);
	state.eyeDirection = Vec3(1,0,0);
	state.eyePosition = Vec3(0,0,0);
	state.fireDirection = Vec3(1,0,0);
	state.lean = 0;
	state.pos = Vec3(0,0,0);
	state.upDirection = Vec3(0,0,1);
	state.weaponPosition = Vec3(0,0,0);

	return true;
}
