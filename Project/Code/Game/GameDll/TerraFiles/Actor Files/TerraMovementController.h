#ifndef __TERRAMOVEMENTCONTROLLER_H__
#define __TERRAMOVEMENTCONTROLLER_H__

#pragma once

#include "TerraPlayer.h"
#include "TimeValue.h"
#include "GameUtils.h"

class CTerraMovementController : public IActorMovementController
{
public:
	CTerraMovementController( CTerraPlayer * pPlayer );
	inline virtual ~CTerraMovementController() {}

	virtual void Reset();
	virtual bool Update( float frameTime, SActorFrameMovementParams& params );
	virtual bool GetStats(SStats& stats);
	virtual void PostUpdate( float frameTime );
	virtual void Release();
	virtual void BindInputs( IAnimationGraphState * ) {}
	virtual void Serialize(TSerialize &ser);

	// Description:
	//    Request some movement; 
	//    If the request cannot be fulfilled, returns false, and request
	//    is updated to be a similar request that could be fulfilled
	//    (calling code is then free to inspect this, and call RequestMovement
	//    again to set a new movement)
	virtual bool RequestMovement( CMovementRequest& request );
	virtual void GetMovementState( SMovementState& state )
	{
		state = m_currentMovementState;
	};
	// Description:
	//    Returns the description of the stance as if the specified stance would be set right now.
	//		If the parameter 'defaultPose' is set to false, the current aim and look target info is used to
	//		calculate the stance info, else a default pose facing along positive Y-axis is returned.
	//		Returns false if the description cannot be queried.
	virtual bool GetStanceState( const SStanceStateQuery& query, SStanceState& state );

	virtual inline Vec2 GetDesiredMoveDir() const { return Vec2(0,1); }

	// Orientation deviation is only taken into account when it's over the angleThreshold (degrees) & it has
	// been like that for timeThreshold seconds.
	virtual void SetDeviatedOrientationThresholds(CTimeValue timeThreshold, float angleThreshold) {}

protected:
	
	SMovementState m_currentMovementState;
};

#endif