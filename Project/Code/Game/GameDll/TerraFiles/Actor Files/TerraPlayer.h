#ifndef __TERRAPLAYER_H__
#define __TERRAPLAYER_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "Actor.h"

#define CAM_OFFSET Vec3(0, 0, 10)

struct IPlayerInput;

class CTerraPlayer : public CActor
{
public:
	CTerraPlayer();
	~CTerraPlayer();

	virtual bool Init( IGameObject * pGameObject );
	virtual void PostInit( IGameObject * pGameObject );
	virtual void InitClient(int channelId );
	virtual void InitLocalPlayer();
	virtual void PrePhysicsUpdate();
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot);
	virtual void ProcessEvent(SEntityEvent& event);

	IActorMovementController * CreateMovementController();
	
	void SerializeXML( XmlNodeRef& node, bool bLoading );
	void SetAuthority( bool auth );
	void GetMemoryUsage(ICrySizer * s) const;

	void UpdateView(SViewParams &viewParams);

	IPlayerInput* GetPlayerInput() const {return m_playerInput.get();}

protected:
	std::auto_ptr<IPlayerInput> m_playerInput;
	SCharacterMoveRequest		m_moveRequest;
};

#endif