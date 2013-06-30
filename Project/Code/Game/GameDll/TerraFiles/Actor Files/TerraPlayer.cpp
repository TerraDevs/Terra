#include "StdAfx.h"
#include "Game.h"
#include "GameCVars.h"
#include "IDebugHistory.h"
#include "TerraPlayer.h"
#include "TerraPlayerInput.h"
#include "TerraMovementController.h"
#include "HUD/UIManager.h"
#include <IViewSystem.h>

CTerraPlayer::CTerraPlayer():
	m_pDebugHistoryManager(NULL)
{
}

CTerraPlayer::~CTerraPlayer()
{
	m_playerInput.reset();

	if(m_pDebugHistoryManager != NULL)
	{
		m_pDebugHistoryManager->Release();
		delete m_pDebugHistoryManager;
	}
}

bool CTerraPlayer::Init(IGameObject * pGameObject)
{
	if (!CActor::Init(pGameObject))
		return false;

	Revive(true);

	if(IEntityRenderProxy* pProxy = (IEntityRenderProxy*)GetEntity()->GetProxy(ENTITY_PROXY_RENDER))
	{
		if(IRenderNode* pRenderNode = pProxy->GetRenderNode())
			pRenderNode->SetRndFlags(ERF_REGISTER_BY_POSITION,true);
	}

	return true;
}

void CTerraPlayer::PostInit( IGameObject * pGameObject )
{
	CActor::PostInit(pGameObject);

	ResetAnimGraph();

	if(gEnv->bMultiplayer)
	{
		GetGameObject()->SetUpdateSlotEnableCondition( this, 0, eUEC_WithoutAI );
	}
	else if (!gEnv->bServer)
	{
		GetGameObject()->SetUpdateSlotEnableCondition( this, 0, eUEC_VisibleOrInRange );
	}
}

void CTerraPlayer::InitClient(int channelId )
{
	GetGameObject()->InvokeRMI(CActor::ClSetSpectatorMode(), CActor::SetSpectatorModeParams(GetSpectatorMode(), 0), eRMI_ToClientChannel|eRMI_NoLocalCalls, channelId);
	
	CActor::InitClient(channelId);
}

void CTerraPlayer::InitLocalPlayer()
{
	CActor::InitLocalPlayer();

	GetGameObject()->SetUpdateSlotEnableCondition(this, 0, eUEC_WithoutAI);
}

void CTerraPlayer::PrePhysicsUpdate()
{
	float frametime = gEnv->pTimer->GetFrameTime();

	if(m_playerInput.get())
		m_playerInput->PreUpdate();

	//Move player//////////////////////////
	SActorFrameMovementParams moveParams;

	if(m_pMovementController)
		m_pMovementController->Update(frametime, moveParams); //Modifies moveParams to contain rotation, velocity, etc.

	m_moveRequest.rotation	= Quat::CreateRotationXYZ(moveParams.deltaAngles);
	m_moveRequest.velocity	= moveParams.desiredVelocity;
	m_moveRequest.type		= ECharacterMoveType::eCMT_Normal;

	m_pAnimatedCharacter->AddMovement(m_moveRequest); //Applies movement

	m_moveRequest.velocity.zero();

	if(m_pMovementController)
		m_pMovementController->PostUpdate(frametime);
	///////////////////////////////////////

	UpdateDebug();
}

void CTerraPlayer::UpdateDebug()
{
	if(!g_pGameCVars->pl_debug_movement)
		return;

	if(m_pDebugHistoryManager == NULL)
		m_pDebugHistoryManager = g_pGame->GetIGameFramework()->CreateDebugHistoryManager();
}

void CTerraPlayer::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	CActor::Update(ctx,updateSlot);
	
	if (m_playerInput.get())
		m_playerInput->Update();
	else
		m_playerInput.reset(new CTerraPlayerInput(this));
}

void CTerraPlayer::ProcessEvent(SEntityEvent& event)
{
	switch(event.event)
	{
	case ENTITY_EVENT_PREPHYSICSUPDATE: PrePhysicsUpdate(); break;
	}
}

IActorMovementController* CTerraPlayer::CreateMovementController()
{
	return new CTerraMovementController(this);
}

void CTerraPlayer::SerializeXML( XmlNodeRef& node, bool bLoading )
{
}

void CTerraPlayer::SetAuthority( bool auth )
{
}

void CTerraPlayer::GetMemoryUsage(ICrySizer * s) const
{
	s->Add(*this);
	CActor::GetActorMemoryStatistics(s);
	if (m_playerInput.get())
		m_playerInput->GetMemoryUsage(s);
}

void CTerraPlayer::UpdateView(SViewParams &viewParams)
{
	viewParams.fov			= 0.5;
	viewParams.position		= GetEntity()->GetWorldPos() + CAM_OFFSET;
	viewParams.rotation		= Quat::CreateRotationXYZ(CAM_ROTATION);

	//Update HUD
	static CUIManager* pManager = CUIManager::GetInstance();
	if (pManager)
		pManager->ProcessViewParams(viewParams);
}