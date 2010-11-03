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

class CQuakeGameProcess: public CProcess
{
public:
	//---Init and End protocols
	CQuakeGameProcess(const std::string& processName):	CProcess(processName),
																										m_PelotaData(NULL),
																										m_PlayerData(NULL),
																										m_EnemyData(NULL),
																										m_SpeedPlayer(15.f),
																										m_Pelota(NULL),
																										m_Player(NULL),
																										m_Enemy(NULL),
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


private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------
	void								UpdatePlayer		   (float elapsedTime);
	//void								UpdatePruebaItems   (float elapsedTime);

private:
	CArena																	m_Arena;
	//CASEObject															m_PruebaItemASE;
	//std::vector	<SPRUEBAITEM *>								m_PruebaItems;
	std::vector <SPRUEBASHUT *>									m_PruebaShut;
	float																		m_SpeedPlayer;

	void								RenderQuake					(CRenderManager* renderManager);

	// Prueba PhysX
	CQuakePhysicsData												*m_PelotaData;
	CQuakePhysicsData												*m_PlayerData;
	CQuakePhysicsData												*m_EnemyData;
	CPhysicActor														*m_Pelota;
	CPhysicController												*m_Player;
	CPhysicController												*m_Enemy;
	int																			mPCX;
	int																			mPCDiff;
};

static CQuakeGameProcess* GetQuakeGame() {return static_cast<CQuakeGameProcess*>(CCore::GetSingletonPtr()->GetProcess());}

#endif //INC_AVI_TEST_PROCESS_H_