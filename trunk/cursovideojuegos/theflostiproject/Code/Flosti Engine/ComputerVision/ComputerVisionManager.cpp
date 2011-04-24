#include "__PCH_ComputerVision.h"


#include "ComputerVisionManager.h"
#include "Base/Logger/Logger.h"
#include "Script/ScriptManager.h"
#include "Exceptions/Exception.h"
#include "luabind/luabind.hpp"

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CComputerVisionManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CComputerVisionManager::Release () 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CComputerVisionManager:: shutting down OpenCV");
	
	if(m_pImage!=0)
	{		
		cvReleaseImage(&m_pImage);
	}

	if( m_pImageForRender!=0)
	{
		cvReleaseImage(&m_pImageForRender);
	}

	if (m_pFrame!=0)
	{
		cvReleaseImage(&m_pFrame);
	}

	if( m_pCapture!=0)
	{
		cvReleaseCapture(&m_pCapture);
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CComputerVisionManager:: offline (ok)");
}

//----------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------
bool CComputerVisionManager::Init ()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CComputerVisionManager:: calling initialization");
	
	if (!m_bIsOk)
	{
		//inicialize camera
		if( !(m_pCapture = cvCaptureFromCAM( 0 )) ) // 0 means first webcam, -1 means any webcam
		{
			m_bIsOk = false;
		}
		else
		{
			///A sample frame is acquired.
			m_pFrame = cvQueryFrame( m_pCapture );
			m_bIsOk = (m_pFrame!=NULL);
			
			if (m_bIsOk)
			{
				m_pImageForRender = cvCreateImage( cvGetSize(m_pFrame), IPL_DEPTH_8U, 3 );
				m_pImageForRender->origin = m_pFrame->origin;
				m_pImage = cvCreateImage( cvGetSize(m_pFrame), IPL_DEPTH_8U, 1 );
				m_pImage->origin = m_pFrame->origin;
			}			
		}
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CComputerVisionManager:: OpenCV is already initializated");
	}
		

	if (!m_bIsOk)
	{
		std::string msg_error = "CComputerVisionManager::Init Error en la inicializacion de OpenCV";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CComputerVisionManager:: online (ok)");
	}

	return m_bIsOk;
}

uint8 * CComputerVisionManager::GetImageForRender( void ) 
{
	return (uint8*)m_pImageForRender->imageDataOrigin;
}

/*
Public function to get the size of the image.
*/
void CComputerVisionManager::GetSizeImage(uint32 &w, uint32 &h)
{
	w = m_pFrame->width;
	h = m_pFrame->height;
}

/*
Function to acquire new frames from the camera and convert them to black and white for further processing.
*/
bool CComputerVisionManager::AcquireNewFrame()
{
	m_pFrame = NULL;
	m_pFrame = cvQueryFrame( m_pCapture ); //a new image is acquired from the webcam. frame should not be deleted nor modified.
	if  (m_pFrame==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CComputerVisionManager:: Error in AcquireNewFrame");
		return false;
	}
	cvFlip(m_pFrame,m_pFrame,0);
	cvFlip(m_pFrame,m_pFrame,1);
	m_pImageForRender = m_pFrame;

	return true;
}

void CComputerVisionManager::Update (float elapsedTime)
{
	if (m_bIsOk)
	{
		m_fCountTime += elapsedTime;
		if ( m_fCountTime > m_fTimeToNewAcquire )
		{
			m_fCountTime = 0.f;
			AcquireNewFrame();
		}
	}
}

//----------------------------------------------------------------------------
// LUA BIND
//----------------------------------------------------------------------------
void CComputerVisionManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;

	// ahora registramos lo que querramos
	module(l_pLUAState)
		[
			// registramos la clase CComputerVisionManager
			class_<CComputerVisionManager>(CScriptRegister::SetClassName("CComputerVisionManager"))
			
			// registramos su constructor
			.def(constructor<>())

			//-----------GENERAL FUNCTIONS---------------------
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)
			
			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"),
						&CScriptRegister::Help)
		];
}