//----------------------------------------------------------------------------------
// CConsoleAndLoggerProcess class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga la aplicacion Laberynth Game
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LABERYNTH_PROCESS_H_
#define INC_LABERYNTH_PROCESS_H_

//---Engine Includes---
#include "Core/Process.h"
#include "Core/Core.h"
//---------------------

//--Forward Declaration--
//Engine:
class CRenderManager;
class CObject3D;  
class CInputManager;
class CASEObject;
//Game:
class CGameLogicLaberynth;
class CLaberynthBoard;
class CLaberynthBoardInput;
//-----------------------


//---Definicion de nuevos tipos------------------------------------
struct SLevel
{
	SLevel::SLevel(): m_fTimeOut(0.f), m_GoalPosition(v3fZERO), m_StartPosition(v3fZERO),
										m_sPhysXBoardName("./"), m_sLevelName(""), m_pASEBoard(NULL) {}
	std::string	m_sLevelName;
	float				m_fTimeOut;
	Vect3f			m_GoalPosition;
	Vect3f			m_StartPosition;
	std::string	m_sPhysXBoardName;
	CASEObject*	m_pASEBoard;

};

struct SGameSettings
{
	SGameSettings::SGameSettings(): m_bUsingComputerVision(false), 
																	m_bTimeOutEndCondition(true),
																	m_uLevel(0),
																	m_fForceRJ1(0.2f),
																	m_fForceRJ2(0.2f)
																	{}

	float		m_fForceRJ1;
	float		m_fForceRJ2;
	bool		m_bUsingComputerVision;
	bool		m_bTimeOutEndCondition;
	uint32	m_uLevel;
};

struct CameraPosition
{
	CameraPosition::CameraPosition(): m_fYaw(0.f), m_fPitch(0.f), m_fZoom(0.f), m_Position(v3fZERO) {}
	float		m_fYaw;
	float		m_fPitch;
	float		m_fZoom;
	Vect3f	m_Position;
};
//-----------------------------------------------------------------



class CLaberynthProcess: public CProcess
{
public:
	//---Init and End protocols
	CLaberynthProcess(const std::string& processName);
	virtual ~CLaberynthProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions				(CScriptManager* scriptManager);


	//----CProcess Interface---------------------------------------
	virtual bool	      Init	             			();
	
	//---Update and Render function
	virtual void				Update	           			(float elapsedTime);
	virtual void				RenderScene							(CRenderManager* renderManager, CFontManager* fm);
	virtual void				RenderSceneOverGUI			(CRenderManager* renderManager, CFontManager* fm);
	virtual uint32			RenderDebugInfo					(CRenderManager* renderManager, CFontManager* fm, float fps);
	//--------------------------------------------------------------
	
	bool								Start										(bool show);
	void								SetInBoardSettings			(bool inSettings); 

	//---Get and Set Functions----
	bool								GetUsingComputerVision	() const										{return m_sGameSettings.m_bUsingComputerVision;}
	bool								GetTimeOutEndCondition	() const										{return m_sGameSettings.m_bTimeOutEndCondition;}
	std::string					GetLevel								();
	float								GetForceRJ1							() const										{return m_sGameSettings.m_fForceRJ1;}
	float								GetForceRJ2							() const										{return m_sGameSettings.m_fForceRJ2;}
	
	void								SetUsingComputerVision	(bool flag)									{m_sGameSettings.m_bUsingComputerVision = flag;}
	void								SetTimeOutEndCondition	(bool flag)									{m_sGameSettings.m_bTimeOutEndCondition = flag;}
	void								SetLevel								(const std::string& level);
	void								SetForceRJ1							(float force);								
	void								SetForceRJ2							(float force);								

	//-----------------------------

	bool								End											();

private:
	//----CProcess Interface---------------------------------------
	virtual void				Release									();
	//-------------------------------------------------------------

	void								UpdateInputActions			(CInputManager* inputManager);
	bool								LoadLevels							(const std::string& xmlFile);
	void								LoadOrCreatePhysXMesh		(	const std::string& aseFileName, 
																								const std::string& binFileName, 
																								const std::string& physXMeshName );

private:
	CObject3D*						m_pObject3D;
	SGameSettings					m_sGameSettings;
	CGameLogicLaberynth*	m_pGameLogicLaberynth;
	std::vector<SLevel>		m_Levels;
	CASEObject*						m_pASEBaseBoard;
	CASEObject*						m_pASEBall;
	float									m_fBallRadius;
	CameraPosition				m_sShow_CameraPos;
	CameraPosition				m_sGame_CameraPos;
	CameraPosition				m_sSettings_CameraPos;
	bool									m_bInShow;

	//---For BoardSettings
	bool									m_bInBoardSettings;
	CLaberynthBoard*			m_BoardForSettings;
	CLaberynthBoardInput*	m_BoardInputForSettings;
};

static CLaberynthProcess* GetGameLaberynth() {return static_cast<CLaberynthProcess*>(CORE->GetProcess());}

#endif //INC_LABERYNTH_PROCESS_H_