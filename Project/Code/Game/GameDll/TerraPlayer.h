#ifndef __TERRAPLAYER_H__
#define __TERRAPLAYER_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "Actor.h"

#define CAM_OFFSET		Vec3(0, -10, 10)
#define CAM_ROTATION	Ang3(DEG2RAD(-45), 0, 0)

struct IPlayerInput;
struct IDebugHistoryManager;

class CTerraPlayer : public CActor
{
	friend class CTerraPlayerInput;
	friend class CTerraMovementController;

public:
	CTerraPlayer();
	~CTerraPlayer();

	virtual bool Init( IGameObject * pGameObject );
	virtual void PostInit( IGameObject * pGameObject );
	virtual void InitClient(int channelId );
	virtual void InitLocalPlayer();
	virtual void PrePhysicsUpdate();
	virtual void UpdateDebug();
	virtual void UpdateAnimation();
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot);
	virtual void UpdateView(SViewParams &viewParams);
	virtual void PostUpdateView(SViewParams& viewParams);
	virtual void ProcessEvent(SEntityEvent& event);

	IActorMovementController * CreateMovementController();
	
	void SerializeXML( XmlNodeRef& node, bool bLoading );
	void SetAuthority( bool auth );
	void GetMemoryUsage(ICrySizer * s) const;

	IPlayerInput*	GetPlayerInput() const {return m_playerInput.get();}
	IEntity*		GetAimCursor() const {return m_pAimCursor;}

protected:
	std::auto_ptr<IPlayerInput> m_playerInput;
	IPersistantDebug*			m_pDebug;
	IEntity*					m_pAimCursor;

	IAnimationGraph::InputID	m_AnimInput_Stance;
	IAnimationGraph::InputID	m_AnimInput_Action;
	IAnimationGraph::InputID	m_AnimInput_Signal;
	IAnimationGraph::InputID	m_AnimInput_Item;
	IAnimationGraph::InputID	m_AnimInput_UsingLookIK;
	IAnimationGraph::InputID	m_AnimInput_Aiming;
	IAnimationGraph::InputID	m_AnimInput_VehicleName;
	IAnimationGraph::InputID	m_AnimInput_VehicleSeat;
	IAnimationGraph::InputID	m_AnimInput_DesiredTurnSpeed;
public: IAnimationGraph::InputID	m_AnimInput_PseudoSpeed;
};

#endif