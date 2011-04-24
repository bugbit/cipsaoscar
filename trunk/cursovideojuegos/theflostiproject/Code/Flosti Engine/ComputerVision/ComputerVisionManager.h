//----------------------------------------------------------------------------------
// CComputerVisionManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary computer vision functions.
// It internally uses the OpenCV library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_COMPUTER_VISION_MANAGER_H_
#define INC_COMPUTER_VISION_MANAGER_H_

#include "Script/ScriptRegister.h"

//---Forward Declarations---
class CScriptManager;
//--------------------------

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"


class CComputerVisionManager:public CScriptRegister
{
public:
	//----Init and End protocols------------------------------------
	CComputerVisionManager(): m_bIsOk(false), m_fTimeToNewAcquire(1.f), m_fCountTime(0.f), m_pFrame(NULL), 
														m_pImage(NULL), m_pImageForRender(NULL), m_pCapture(NULL) {}
	virtual ~CComputerVisionManager() {Done();}  

	bool					Init							();
	void					Done							();
	bool					IsOk							() const { return m_bIsOk; }
	//---------------------------------------------------------------

	//----CComputerVisionManager interface-------------------
	void					Update						(float elapsedTime);
	uint8*				GetImageForRender	() ;
	void					GetSizeImage			(uint32 &w, uint32 &h);

	//----CScriptRegister interface-------------------
	virtual void	RegisterFunctions	(CScriptManager* scriptManager);


private:
	void					Release						();
	bool					AcquireNewFrame		();

private:	
	bool				m_bIsOk;							// Initialization boolean control
	float				m_fCountTime;
	float				m_fTimeToNewAcquire;	
	//Captura 	de la imatge
	IplImage*		m_pFrame;						//Pointer to webcam frames.
	IplImage*		m_pImage;						//Current image from the webcam.
	IplImage*		m_pImageForRender;
	CvCapture*	m_pCapture;					//Pointer to capture structure.

};

#endif // INC_COMPUTER_VISION_MANAGER_H_
