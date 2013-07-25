#ifndef __TERRAPLAYER_H__
#define __TERRAPLAYER_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Actor.h"

#define CAM_OFFSET		Vec3(0, -10, 10)
#define CAM_ROTATION	Ang3(DEG2RAD(-45), 0, 0)
#define WEAPON_OFFSET	Vec3(0, 0, 1);

struct IPlayerInput;
struct IDebugHistoryManager;

class CTerraPlayer : public CActor
{
	friend class CTerraPlayerInput;
	friend class CTerraMovementController;

public:
	CTerraPlayer();
	~CTerraPlayer();

	bool Init(IGameObject * pGameObject);
	void PostInit(IGameObject * pGameObject);
	void InitClient(int channelId);
	void InitLocalPlayer();
	void Revive(bool fromInit);

	void ResetAnimGraph();
	void BindInputs(IAnimationGraphState*);
	void SetParams(SmartScriptTable &rTable,bool resetFirst);

	void PrePhysicsUpdate();
	void UpdateDebug();
	void Update(SEntityUpdateContext& ctx, int updateSlot);
	void UpdateAimIK();
	void UpdateView(SViewParams &viewParams);
	void PostUpdateView(SViewParams& viewParams);

	void ProcessEvent(SEntityEvent& event);

	IActorMovementController * CreateMovementController();
	
	void SerializeXML(XmlNodeRef& node, bool bLoading);
	void SetAuthority(bool auth);
	void GetMemoryUsage(ICrySizer * s) const;

	IPlayerInput*		GetPlayerInput() const		{return m_playerInput.get();}
	IEntity*			GetAimCursor() const		{return m_pAimCursor;}
	SActorStats*		GetActorStats() 			{return &m_stats;}
	const SActorStats*	GetActorStats() const		{return &m_stats;}
	SActorParams*		GetActorParams()			{return &m_params;}

protected:
	std::auto_ptr<IPlayerInput> m_playerInput;
	IPersistantDebug*			m_pDebug;
	IEntity*					m_pAimCursor;
	SActorParams				m_params;
	SActorStats					m_stats;

	IAnimationGraph::InputID	m_inputItem;
	IAnimationGraph::InputID	m_inputAiming;
	IAnimationGraph::InputID	m_inputUsingLookIK;
};

#endif