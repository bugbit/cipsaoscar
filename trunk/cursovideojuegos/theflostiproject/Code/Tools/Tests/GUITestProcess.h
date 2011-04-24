//----------------------------------------------------------------------------------
// CGUITestProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento del modulo GUI.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_TEST_PROCESS_H_
#define INC_GUI_TEST_PROCESS_H_

//---Engine Includes-----
#include "Core/Process.h"
#include "Core/Core.h"
//-----------------------

//--Forward Declaration--
class CRenderManager;
class CObject3D;  
class CInputManager;
class CButton;
class CCheckButton;
class CStaticText;
class CSlider;
class CEditableTextBox;
class CImage;
class CProgressBar;
class CDialogBox;
class CRadioBox;
//-----------------------

class CGUITestProcess: public CProcess
{
public:
	//---Init and End protocols
	CGUITestProcess(const std::string& processName):	CProcess(processName),
																										m_pObject3D(NULL),
																										m_yaw(0.f) {}
	virtual ~CGUITestProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	virtual bool				Start								()  {m_bStart = true; return true;}

	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fm);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fm, float fps);
	//--------------------------------------------------------------



private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

	void								UpdateInputActions	(CInputManager* inputManager);

private:

	CObject3D*				m_pObject3D;

	CButton*					m_pButton;
	CButton*					m_pButton2;
	CCheckButton*			m_pCheckButton;
	CStaticText*			m_pStaticText;
	CSlider*					m_pSlider;
	CImage*						m_pImage;
	CImage*						m_pImage2;
	CEditableTextBox*	m_pEditableTextBox;
	CProgressBar*			m_pProgressBar;
	CDialogBox*				m_pDialogBox;
	CRadioBox*				m_pRadioBox;
	
	float							m_yaw;
};

static CGUITestProcess* GetGuiTest() {return static_cast<CGUITestProcess*>(CORE->GetProcess());}

#endif //INC_GUI_TEST_PROCESS_H_