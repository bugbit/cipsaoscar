//----------------------------------------------------------------------------------
// CQuakeGameProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento del reproductor de videos AVI.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_QUAKE_GAME_PROCESS_H_
#define INC_QUAKE_GAME_PROCESS_H_

//---Engine Includes----
#include "Core/Core.h"
#include "Core/Process.h"
#include "Math/Matrix44.h"
#include "Graphics/ASEObject/ASEObject.h"
#include "PhysX/PhysicTriggerReport.h"

//---Game Includes-------
#include "Arena.h"

//-----------------------

//--Forward Declaration--
class CRenderManager;
class CObject3D;  
class CInputManager;
class CAviPlayer;
class CTexture;
class CPhysicActor;
class CPhysicController;
class CQuakePhysicsData;
class CPhysicSphericalJoint;
class CQuakePlayerInput;
//-----------------------

//struct SPRUEBAITEM
//{
//	Vect3f Position;
//	Mat44f Mat;
//	float Angle;
//};

struct SPRUEBASHUT
{
	std::string msg;
	Vect3f pos;
};

class CQuakeGameProcess: public CProcess, public CPhysicTriggerReport 
{
public:
	//---Init and End protocols
	CQuakeGameProcess(const std::string& processName):	CProcess(processName),
																										m_PelotaData(NULL),
																										m_EnemyData(NULL),
																										m_SpeedPlayer(15.f),
																										m_Pelota(NULL),
																										m_Enemy(NULL),
																										m_Trigger(NULL),
																										m_TriggerData(NULL),
																										m_ActorPruebaShut(NULL),
																										m_ActorPruebaShutData(NULL),
																										m_ActorPruebaJoint(NULL),
																										m_ActorPruebaJointData(NULL),
																										m_PruebaJoint(NULL),
																										mPCX(0),
																										mPCDiff(1)
																										{
																										}
	virtual ~CQuakeGameProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	virtual bool				Start								()  {m_bStart = true; return true;}

	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, float fps);
	//--------------------------------------------------------------

	//----CPhysicTriggerReport Interface------------------------------------------------------
	virtual void				OnEnter							(CPhysicUserData* trigger1, CPhysicUserData* other_shape);
	virtual void				OnLeave							(CPhysicUserData* trigger1, CPhysicUserData* other_shape);


private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------
	//void								UpdatePruebaItems   (float elapsedTime);

private:
	CArena																	m_Arena;
	CASEObject															m_PruebaItemASE;
	//std::vector	<SPRUEBAITEM *>								m_PruebaItems;
	std::vector <SPRUEBASHUT *>									m_PruebaShut;
	float																		m_SpeedPlayer;
	std::vector<CQuakePlayerInput *>				m_PlayerInputs;

	void								RenderQuake					(CRenderManager* renderManager);

	// Prueba PhysX
	CQuakePhysicsData												*m_PelotaData;
	CQuakePhysicsData												*m_EnemyData;
	CPhysicActor														*m_Pelota;
	CPhysicController												*m_Enemy;
	CPhysicActor														*m_Trigger;
	CQuakePhysicsData												*m_TriggerData;
	CPhysicActor														*m_ActorPruebaShut;
	CQuakePhysicsData												*m_ActorPruebaShutData;
	CPhysicActor														*m_ActorPruebaJoint;
	CQuakePhysicsData												*m_ActorPruebaJointData;
	CPhysicSphericalJoint										*m_PruebaJoint;
	int																			mPCX;
	int																			mPCDiff;
	std::string															mStrTrigger;
	
	void																		ReleasePlayerInputs();
	void																		UpdatePlayerInputs(float elapsedTime);
};

static CQuakeGameProcess* GetQuakeGame() {return static_cast<CQuakeGameProcess*>(CCore::GetSingletonPtr()->GetProcess());}

#endif //INC_AVI_TEST_PROCESS_H_