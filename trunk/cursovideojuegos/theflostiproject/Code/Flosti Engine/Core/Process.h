//----------------------------------------------------------------------------------
// CProcess class
// Author: Enric Vergara
//
// Description:
// Clase abstracta encargada de ofrecer la interfaz comun de todo proceso generico, los diferentes
// juegos que programemos seran clases derivadas de esta, como tambien las aplicacions viewer, tests, etc.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PROCESS_H_
#define INC_PROCESS_H_

#include <string>
#include "Script/ScriptRegister.h"
#include "Math/LerpAnimator1D.h"

//--Forward Declaration--
class CRenderManager;
class CFontManager;
class CCamera;
//-----------------------

class CProcess:public CScriptRegister
{
public:
	//---Init and End protocols
	CProcess(const std::string& processName): m_bIsOk(false), m_bRenderDebugInfo(false), m_sProcessName(processName), 
																						m_PosRenderDebugInfo(350,5), m_pCamera(NULL), m_bStart(false), 
																						m_bFinished(false) {}
	virtual ~CProcess(void) {Done();}
	

	void	              Done	              ();
	bool	              IsOk	              () const { return m_bIsOk; }
	virtual bool	      Init	              () = 0;
	bool								HasFinished					() {return m_bFinished;}
	void								Finished						() {m_bFinished = true;}							

	//----CScriptRegister interface-------------------
	virtual void				RegisterFunctions		(CScriptManager* scriptManager) {;}


	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager) = 0;
	virtual void				RenderScene2D				(CRenderManager* renderManager, CFontManager* fontManager) {;}
	virtual void				RenderSceneOverGUI	(CRenderManager* renderManager, CFontManager* fontManager) {;}
	

	void								SetProcessName			(const std::string& name) {m_sProcessName = name;}
	const std::string&	GetProcessName			() const {return m_sProcessName;}

	virtual CCamera*		GetCamera						() const {return m_pCamera;}
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fontManager, float fps);
	void								EndRenderDebugInfo	(CRenderManager* renderManager, uint32 finalPosY);


protected:
	virtual void				Release             () = 0;

protected:
	bool				        m_bIsOk;			      // Initialization boolean control
	bool								m_bFinished;
	bool								m_bStart;
	std::string					m_sProcessName;
	bool								m_bRenderDebugInfo;
	Vect2i							m_PosRenderDebugInfo;
	CCamera*						m_pCamera;
	CLerpAnimator1D			m_LerpAnimator1D;
};

#endif //INC_PROCESS_H_