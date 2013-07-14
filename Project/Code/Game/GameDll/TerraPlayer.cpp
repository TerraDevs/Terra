#include "StdAfx.h"
#include "Game.h"
#include "GameCVars.h"
#include "IDebugHistory.h"
#include "TerraPlayer.h"
#include "TerraPlayerInput.h"
#include "TerraMovementController.h"
#include "HUD/UIManager.h"
#include "IHardwareMouse.h"
#include "Cry_GeoIntersect.h"
#include "Utility/StringUtils.h"
#include <IViewSystem.h>

CTerraPlayer::CTerraPlayer()
{
}

CTerraPlayer::~CTerraPlayer()
{
	m_playerInput.reset();

	gEnv->pEntitySystem->RemoveEntity(m_pAimCursor->GetId());
}

bool CTerraPlayer::Init(IGameObject * pGameObject)
{
	if (!CActor::Init(pGameObject))
		return false;

	Revive(true);

	return true;
}

void CTerraPlayer::PostInit( IGameObject * pGameObject )
{
	CActor::PostInit(pGameObject);

	ResetAnimGraph();

	if(gEnv->bMultiplayer)
		GetGameObject()->SetUpdateSlotEnableCondition( this, 0, eUEC_WithoutAI );
	else if (!gEnv->bServer)
		GetGameObject()->SetUpdateSlotEnableCondition( this, 0, eUEC_VisibleOrInRange );

	m_pDebug = gEnv->pGameFramework->GetIPersistantDebug();

	//Init aim cursor
	SEntitySpawnParams cursorParams;
	cursorParams.sName		= "playerAimCursor";
	cursorParams.sLayerName	= "playerLayer";
	cursorParams.nFlags		= ENTITY_FLAG_NO_SAVE | ENTITY_FLAG_SPAWNED;
	cursorParams.pClass		= gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	m_pAimCursor			= gEnv->pEntitySystem->SpawnEntity(cursorParams);

	if(m_pAimCursor != NULL)
	{
		IMaterial* pMat = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("material_default");
		m_pAimCursor->SetMaterial(pMat);
		m_pAimCursor->LoadGeometry(0, "objects/default/primitive_sphere.cgf");
		m_pAimCursor->SetScale(Vec3(0.2f, 0.2f, 0.2f));
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

void CTerraPlayer::ResetAnimGraph()
{
	ICharacterInstance* pCharacter	= GetEntity()->GetCharacter(0);
	if (pCharacter)
		pCharacter->EnableStartAnimation(true);

	if (m_pAnimatedCharacter)
	{
		IAnimationGraphState* pAGState = m_pAnimatedCharacter->GetAnimationGraphState();
		if (pAGState != NULL)
		{
			pAGState->SetInput( m_inputItem, "nw" );
			m_pAnimatedCharacter->GetAnimationGraphState()->SetInput( m_inputSignal, "none" );
			m_pAnimatedCharacter->GetAnimationGraphState()->SetInput( m_inputWaterLevel, 0 );
		}
		
		m_pAnimatedCharacter->SetParams( m_pAnimatedCharacter->GetParams().ModifyFlags( eACF_ImmediateStance, 0 ) );
	}

	SetStance(STANCE_STAND);
}

void CTerraPlayer::SetParams(SmartScriptTable &rTable,bool resetFirst)
{
	CActor::SetParams(rTable, resetFirst);

	CScriptSetGetChain params(rTable);
	params.GetValue("speedMultiplier",m_params.speedMultiplier);
}

void CTerraPlayer::PrePhysicsUpdate()
{
	float frametime = gEnv->pTimer->GetFrameTime();

	if(m_playerInput.get())
		m_playerInput->PreUpdate();

	//Move player//////////////////////////
	SActorFrameMovementParams	moveParams;
	SCharacterMoveRequest		moveRequest;

	if(m_pMovementController)
		m_pMovementController->Update(frametime, moveParams); //Modifies moveParams to contain rotation, velocity, etc.

	Vec3 toAimCursor		= m_pAimCursor->GetWorldPos() - GetEntity()->GetWorldPos();
	Ang3 playerRotation		= GetEntity()->GetWorldAngles();
	float toAimRotationZ	= cry_atan2f(toAimCursor.y, toAimCursor.x);
	moveRequest.rotation	= Quat::CreateRotationZ(toAimRotationZ - playerRotation.z - gf_PI * 0.5f);
	moveRequest.velocity	= moveParams.desiredVelocity;
	moveRequest.type		= ECharacterMoveType::eCMT_Normal;

	m_pAnimatedCharacter->AddMovement(moveRequest); //Applies movement

	if(m_pMovementController)
		m_pMovementController->PostUpdate(frametime);
	///////////////////////////////////////

	UpdateDebug();
}

void CTerraPlayer::UpdateDebug()
{
	if(!g_pGameCVars->pl_debug_movement)
		return;

	m_pDebug->Reset();
	m_pDebug->Begin("TestAddPersistentText2D",false);

	string positionString	= "Position: " +
								ToString(GetEntity()->GetWorldPos().x) + ", " +
								ToString(GetEntity()->GetWorldPos().y) + ", " +
								ToString(GetEntity()->GetWorldPos().z);

	string rotationString	= "Rotation: " +
								ToString(GetEntity()->GetWorldAngles().x) + ", " +
								ToString(GetEntity()->GetWorldAngles().y) + ", " +
								ToString(GetEntity()->GetWorldAngles().z);

	Vec3 toAimCursor		= m_pAimCursor->GetWorldPos() - GetEntity()->GetWorldPos();
	float toAimCursorAng	= cry_atan2f(toAimCursor.y, toAimCursor.x);
	string toCursorString	= "ToAimCursor: " + ToString(toAimCursorAng);

	m_pDebug->AddDirection(GetEntity()->GetWorldPos(), 1.0f, toAimCursor, ColorF(1.0f, 0.0f, 0.0f), 0.5f);
	m_pDebug->Add2DText(positionString.c_str(), 1.0f, ColorF(1.0f, 1.0f, 1.0f), 1.0f);
	m_pDebug->Add2DText(rotationString.c_str(), 1.0f, ColorF(1.0f, 1.0f, 1.0f), 1.0f);
	m_pDebug->Add2DText(toCursorString.c_str(), 1.0f, ColorF(1.0f, 1.0f, 1.0f), 1.0f);
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

void CTerraPlayer::UpdateView(SViewParams& viewParams)
{
	viewParams.fov			= 0.5;
	viewParams.position		= GetEntity()->GetWorldPos() + CAM_OFFSET;
	viewParams.rotation		= Quat::CreateRotationXYZ(CAM_ROTATION);

	//Update HUD
	static CUIManager* pManager = CUIManager::GetInstance();
	if (pManager)
		pManager->ProcessViewParams(viewParams);
}

void CTerraPlayer::PostUpdateView(SViewParams& viewParams)
{
	//Update aim cursor
	float	cursorX, cursorY;
	float	mouseX_3d, mouseY_3d, mouseZ_3d;

	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&cursorX, &cursorY);
	gEnv->pRenderer->UnProjectFromScreen(cursorX, gEnv->pRenderer->GetHeight() - cursorY, 0, &mouseX_3d, &mouseY_3d, &mouseZ_3d);

	float	rayRange	= gEnv->p3DEngine->GetMaxViewDistance();
	Vec3	camPos		= gEnv->pRenderer->GetCamera().GetPosition();
	Vec3	rayDir		= (Vec3(mouseX_3d, mouseY_3d, mouseZ_3d) - camPos).GetNormalizedSafe();

	Ray mouseRay(camPos, rayDir);
	Plane groundPlane(Vec3(0.0f, 0.0f, 1.0f), -GetEntity()->GetPos().z);
	Vec3 hitPos;
	Intersect::Ray_Plane(mouseRay, groundPlane, hitPos);

	m_pAimCursor->SetPos(hitPos);
}