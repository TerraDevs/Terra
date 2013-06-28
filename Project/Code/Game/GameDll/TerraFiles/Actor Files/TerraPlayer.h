#ifndef __TERRAPLAYER_H__
#define __TERRAPLAYER_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "Actor.h"

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
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot);

	IActorMovementController * CreateMovementController();
	
	void SerializeXML( XmlNodeRef& node, bool bLoading );
	void SetAuthority( bool auth );
	void GetMemoryUsage(ICrySizer * s) const;

	void UpdateView(SViewParams &viewParams);

	IPlayerInput* GetPlayerInput() const {return mPlayerInput.get();}

protected:
	std::auto_ptr<IPlayerInput> mPlayerInput;
};

#endif