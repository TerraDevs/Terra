#include "StdAfx.h"
#include "Game.h"
#include "GameCVars.h"
#include "TerraPlayer.h"
#include "TerraPlayerInput.h"
#include "TerraMovementController.h"
#include <IViewSystem.h>

CTerraPlayer::CTerraPlayer()
{
}

CTerraPlayer::~CTerraPlayer()
{
	mPlayerInput.reset();
}

//All these init functions seems to bee needed, the code is copied fron the Player.cpp class, and i don't know what does what,
//but when all this is here you get 'UpdateView' (camera updatr function), Update (seems to be the normal update function) and the 'OnAction' (the input feeding function) functions working.
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
	GetGameObject()->SetUpdateSlotEnableCondition( this, 0, eUEC_WithoutAI );
}

void CTerraPlayer::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	CActor::Update(ctx,updateSlot);
	
	if (mPlayerInput.get())
		mPlayerInput->Update();
	else
	{
		mPlayerInput.reset( new CTerraPlayerInput(this) );
	}

	//just to check that this works
	ColorF white(1, 1, 1);

	auto debug = gEnv->pGameFramework->GetIPersistantDebug();
	debug->Reset();
	debug->Begin("TestAddPersistentText2D",false);
	debug->Add2DText("Test", 6, white, 5);
}

IActorMovementController * CTerraPlayer::CreateMovementController()
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
	if (mPlayerInput.get())
		mPlayerInput->GetMemoryUsage(s);
}

void CTerraPlayer::UpdateView(SViewParams &viewParams)
{
	viewParams.fov = 0.5;
	viewParams.position = Vec3(1000, 1000, 25);
	viewParams.rotation = Quat::CreateRotationVDir(Vec3(1,0,0), 0.0f);
}