#include "__PCH_Core.h"

#include "FlostiEngine.h"
#include "Core.h"
#include "Input/InputManager.h"
#include "Input/ActionToInput.h"
#include "PhysX/PhysicsManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Script/ScriptManager.h"
#include "Logger/Logger.h"
#include "Graphics/Materials/EffectManager.h"
#include "Graphics/Textures/TextureManager.h"
#include "Console.h"
#include "GUI/GUIManager.h"
#include "Network/NetworkManager.h"
#include "Sound/SoundManager.h"
#include "Graphics/AviPlayerManager.h"
#include "ComputerVision/ComputerVisionManager.h"
#include "Process.h"
#include "Exceptions/Exception.h"
#include "Language/LanguageManager.h"
#include "luabind/luabind.hpp"

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CCore::Done ()
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
void CCore::Release ()
{
	CHECKED_DELETE(m_pInputManager);
	CHECKED_DELETE(m_pActionToInput);
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pPhysicsManager);
	CHECKED_DELETE(m_pScriptManager);
	CHECKED_DELETE(m_pTextureManager);
	CHECKED_DELETE(m_pGUIManager);
	CHECKED_DELETE(m_pNetworkManager);
	CHECKED_DELETE(m_pSoundManager);
	CHECKED_DELETE(m_pAviPlayerManager);
	CHECKED_DELETE(m_pComputerVisionManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pFontManager);

	if (m_pEffectManager)
	{
		m_pEffectManager->Destroy();
	}
	CHECKED_DELETE(m_pEffectManager);
}


bool CCore::Init(	HWND hwnd, CFlostiEngine* app, CConsole* console, const SEngineInitParams& params)
{

	m_pApplication = app;
	m_pConsole = console;
	assert(m_pConsole);
	assert(m_pApplication);
	
		//----Init Script Manager
	m_pScriptManager = new CScriptManager();
	assert(m_pScriptManager);
	m_bIsOk = m_pScriptManager->Init();
	

	//----Init Render Manager
	if (m_bIsOk)
	{
		m_pRenderManager = new CRenderManager();
		assert(m_pRenderManager);
		m_pScriptManager->AddNewScriptRegister(m_pRenderManager);
		m_bIsOk = m_pRenderManager->Init(hwnd, params.m_bFullScreen, params.m_ScreenResolution.x, params.m_ScreenResolution.y);
	}

	//----Init Font Manager
	if (m_bIsOk)
	{
		m_pFontManager = new CFontManager();
		assert(m_pFontManager);
		m_pScriptManager->AddNewScriptRegister(m_pFontManager);
		m_bIsOk = m_pFontManager->Init(m_pRenderManager);
		m_pFontManager->LoadTTFs(params.m_sInitFontsXML);
	}

	//----Init Input Manager
	if (m_bIsOk)
	{
		m_pInputManager = new CInputManager();
		assert(m_pInputManager);
		m_pScriptManager->AddNewScriptRegister(m_pInputManager);
		Vect2i screenResolution;
		uint32 w,h;
		m_pRenderManager->GetWidthAndHeight(w, h);
		screenResolution.x = w;
		screenResolution.y = h;
		m_bIsOk = m_pInputManager->Init(hwnd, screenResolution, params.m_bExclusiveModeinMouse);
	}

	//----Init Action To Input
	if (m_bIsOk)
	{
		m_pActionToInput = new CActionToInput();
		assert(m_pActionToInput);
		m_bIsOk = m_pActionToInput->Init(m_pInputManager);
		if (params.m_sAction2InputXML != "")
		{
			m_pActionToInput->LoadXML(params.m_sAction2InputXML);
			m_pActionToInput->PrintToLogger();
		}
	}
	


	//----Init Physics Manager
	if (m_bIsOk)
	{
		m_pPhysicsManager = new CPhysicsManager();
		assert(m_pPhysicsManager);
		m_pScriptManager->AddNewScriptRegister(m_pPhysicsManager);
		if (params.m_bInitPhysX)
		{
			m_bIsOk = m_pPhysicsManager->Init();
		}
	}

	//----Init Sound Manager
	if (m_bIsOk)
	{
		m_pSoundManager = new CSoundManager();
		assert(m_pSoundManager);
		m_pScriptManager->AddNewScriptRegister(m_pSoundManager);
		m_bIsOk = m_pSoundManager->Init();
		if(m_bIsOk && (params.m_sInitSoundsXML.compare("") != 0) )
		{
			m_pSoundManager->LoadSounds(params.m_sInitSoundsXML);
		}
	}

	//----Init AviPlayer Manager
	if (m_bIsOk)
	{
		m_pAviPlayerManager = new CAviPlayerManager();
		assert(m_pAviPlayerManager);
		m_pScriptManager->AddNewScriptRegister(m_pAviPlayerManager);
		m_bIsOk = m_pAviPlayerManager->Init(m_pRenderManager);
		if(m_bIsOk && (params.m_sInitAvisXML.compare("") != 0) )
		{
			m_pAviPlayerManager->LoadAvis(params.m_sInitAvisXML);
		}
	}


	//----Init Network Manager
	if (m_bIsOk)
	{
		m_pNetworkManager = new CNetworkManager();
		assert(m_pNetworkManager);
		m_pScriptManager->AddNewScriptRegister(m_pNetworkManager);
		m_pNetworkManager->SetWindow(hwnd);
	}

	// Init Effects
	if(m_bIsOk)
	{
		m_pEffectManager = new CEffectManager;
		assert(m_pEffectManager);
		m_pScriptManager->AddNewScriptRegister(m_pEffectManager);
		m_pEffectManager->Init();
	}

	// Init Texture Manager
	if (m_bIsOk)
	{
		m_pTextureManager = new CTextureManager();
		assert(m_pTextureManager);
		m_pScriptManager->AddNewScriptRegister(m_pTextureManager);
    m_pTextureManager->Init();
	}

	// Init Computer Vision Manager
	if (m_bIsOk)
	{
		m_pComputerVisionManager = new CComputerVisionManager();
		assert(m_pComputerVisionManager);
		m_pScriptManager->AddNewScriptRegister(m_pComputerVisionManager);
		if (params.m_bInitComputerVision)
		{
			m_bIsOk = m_pComputerVisionManager->Init();
		}
	}

	// Init Language Manager
	if (m_bIsOk)
	{
		m_pLanguageManager = new CLanguageManager();
		assert(m_pLanguageManager);
		std::vector<std::string>::const_iterator it			= params.m_vLanguageXML.begin();
		std::vector<std::string>::const_iterator itEnd	= params.m_vLanguageXML.end();
		while (it != itEnd)
		{
			m_pLanguageManager->SetXmlFile(*it);
			++it;
		}
		m_pLanguageManager->LoadXMLs();
		m_pLanguageManager->SetCurrentLanguage(params.m_sCurrentLanguage);
		
	}
	

	//Init GUI Manager
	if (m_bIsOk)
	{
		Vect2i screenResolution;
		uint32 w,h;
		m_pRenderManager->GetWidthAndHeight(w, h);
		screenResolution.x = w;
		screenResolution.y = h;
		m_pGUIManager = new CGUIManager(screenResolution);
		m_pScriptManager->AddNewScriptRegister(m_pGUIManager);
		assert(m_pGUIManager);
		m_pGUIManager->SetVisiblePointerMouse(params.m_bDrawPointerMouse);
		if (params.m_sInitGuiXML.compare("") != 0)
		{
			m_bIsOk = m_pGUIManager->Init(params.m_sInitGuiXML);
		}
	}

	if (!m_bIsOk)
	{
		Release();
		std::string msg_error = "CCore::Init-> Error al inicializar los managers" ;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		m_pScriptManager->AddNewScriptRegister(this);
	}
	
	return m_bIsOk;
}

void CCore::Render ()
{
  if (m_pRenderManager->IsOk())
  {
    if (m_pPhysicsManager->IsOk())
    {
      m_pPhysicsManager->DebugRender(m_pRenderManager);
    }
  }
}


void CCore::RenderAssetErrors ()
{
	m_AssetErrors.Render(m_pRenderManager, m_pFontManager);
}
void CCore::SetAssetError (TypeOfAssetError error)
{
	m_AssetErrors.SetAssetError(error);
}

void CCore::ResetAssetErrors ()
{
	m_AssetErrors.ResetAssetErrors();
}

void CCore::Update (float elapsedTime)
{
	if (m_pInputManager->IsOk())
	{
		m_pInputManager->Update();
	}

	if (m_pPhysicsManager->IsOk())
	{
		m_pPhysicsManager->Update(elapsedTime);
	}	

	if (m_pGUIManager->IsOk())
	{
		m_pGUIManager->Update(elapsedTime);
	}

	if (m_pNetworkManager->IsOk())
	{
		m_pNetworkManager->Update();
	}

	if (m_pAviPlayerManager->IsOk())
	{
		m_pAviPlayerManager->Update(elapsedTime);
	}
	
	if (m_pComputerVisionManager->IsOk())
	{
		m_pComputerVisionManager->Update(elapsedTime);
	}

	if (m_pSoundManager->IsOk())
	{
		m_pSoundManager->Update(elapsedTime);
	}

	UpdateTimers(elapsedTime);

	m_AssetErrors.Update(elapsedTime);
	
}

void CCore::Finished ()
{
	m_pProcess->Finished();
}

void CCore::ResetCurrentProcess ()
{
	m_pApplication->ResetCurrentProcess();
}

void CCore::ChangeProcess (const std::string& newProcess)
{
	m_pApplication->ChangeProcess(newProcess);
}

bool CCore::SaveLogsInFile ()
{
	 return LOGGER->SaveLogsInFile();
}


void CCore::SetAutoComplete (const std::vector<std::string>& vector)
{
	if (m_pConsole)
	{
		m_pConsole->SetAutoComplete(vector);
	}
	else
	{
		//Error message:
		 LOGGER->AddNewLog(ELL_ERROR, "CCore::SetAutoComplete puntero a console nulo!!");
	}
}

bool CCore::IsEndTimer (const std::string& id, float timeOut)
{
	bool isEnd = true;
	tTimerMap::iterator it = m_Timers.find(id);
	if (it != m_Timers.end())
	{
		//Existe el timer--> por lo tanto aun no ha finalizado:
		isEnd = false;
	}
	else
	{
		//Si no existe lo creamos y retornamos true
		m_Timers.insert(std::pair<std::string,float>(id,timeOut));
	}
	return isEnd;
}
void CCore::UpdateTimers(float elapsedTime)
{
	tTimerMap::iterator it		= m_Timers.begin();
	tTimerMap::iterator itEnd = m_Timers.end();
	while (it != itEnd)
	{
		it->second -= elapsedTime;
		if (it->second < 0.f)
		{
			//Ya ha pasado el tiempo-> lo eliminamos
			m_Timers.erase( it );
			//Si hubieran dos timers a eliminar no pasa nada, en el siguiente loop ya lo eliminaremos.
			break; 
			
		}
		it++;
	}
}
void CCore::SetLinesPerPageInLogger(uint32 lines)
{
	if (m_pApplication!=NULL)
	{
		m_pApplication->SetLinesPerPageInLogger(lines);
	}
}

void CCore::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;
	
	
	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
			def("getCore", &CCore::GetSingletonPtr),

      // registramos la clase CCore
			class_<CCore>(CScriptRegister::SetClassName("CCore"))
			
      // registramos su constructor
      .def(constructor<>())
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"), 
						&CScriptRegister::Help)

      .def(	CScriptRegister::PushFunctionName("getInput","CInputManager*","void",
						"Obtiene puntero al inputManager"), 
						&CCore::GetInputManager)

			.def(	CScriptRegister::PushFunctionName("getRender", "CRenderManager*", "void",
						"Obtiene puntero al renderManager"), 
						&CCore::GetRenderManager)

			.def(	CScriptRegister::PushFunctionName("getPhysic", "CPhysicManager*", "void",
						"Obtiene puntero al physicManager"), 
						&CCore::GetPhysicManager)

			.def(CScriptRegister::PushFunctionName("getScript", "CScriptManager*", "void",
						"Obtiene puntero al scriptManager"), 
						&CCore::GetScriptManager)

			.def(	CScriptRegister::PushFunctionName("getTexture" ,"CTextureManager*", "void",
						"Obtiene puntero al textureManager"),	
						&CCore::GetTextureManager)

			.def(	CScriptRegister::PushFunctionName("getEffect", "CEffectManager*", "void",
						"Obtiene puntero al effectManager"),	
						&CCore::GetEffectManager)

			.def(	CScriptRegister::PushFunctionName("getGui", "CGUIManager*", "void",
						"Obtiene puntero al guiManager"), 
						&CCore::GetGUIManager)

			.def(	CScriptRegister::PushFunctionName("getSound", "CSoundManager*", "void",
						"Obtiene puntero al soundManager"), 
						&CCore::GetSoundManager)

			.def(	CScriptRegister::PushFunctionName("getAviPlayer", "CAviPlayerManager*", "void", 
						"Obtiene puntero al aviPlayerManager"), 
						&CCore::GetAviPlayerManager)

			.def(	CScriptRegister::PushFunctionName("getNetwork", "CNetworkManager*","void",
						"Obtiene puntero al networkManager"), 
						&CCore::GetNetworkManager)

			.def(	CScriptRegister::PushFunctionName("getComputerVision","CComputerVisionManager*","void",
						"Obtiene puntero al computerVisionManager"), 
						&CCore::GetComputerVisionManager)

			.def(	CScriptRegister::PushFunctionName("finished", "void", "void",
						"Nos informa si el proceso ha finalizado"), 
						&CCore::Finished)

			.def(	CScriptRegister::PushFunctionName("isEndTimer", "bool", "const std::string&, float",
						"Informa si un timer ha finalizado su cuenta atras"), &CCore::IsEndTimer)			

			.def(	CScriptRegister::PushFunctionName("saveLogsInFile", "bool", "void",
						"Guarda si es posible el log en un fichero"), 
						&CCore::SaveLogsInFile)			

      .def(	CScriptRegister::PushFunctionName("setAssetError" "void", "TypeOfAssetError",
						"Advierte por pantalla un error con un asset determinado"), 
						&CCore::SetAssetError)			

      .def(	CScriptRegister::PushFunctionName("resetAssetErrors", "void", "void",
						"Resetea las advertencias de assets que se estaban mostrando por pantalla"), 
						&CCore::ResetAssetErrors)			

			.def(	CScriptRegister::PushFunctionName("changeProcess", "void", "const std::string&",
						"Hace un deinit del proceso actual y un init del proceso a cambiar"), 
						&CCore::ChangeProcess)

			.def(	CScriptRegister::PushFunctionName("resetCurrentProcess", "void", "void",
						"Hace un deinit y un init del proceso actual"),	
						&CCore::ResetCurrentProcess)

			.def(	CScriptRegister::PushFunctionName("setLinesPerPageInLogger", "void", "void",
						"Setea la cantidad de lineas en el render del log"),	
						&CCore::SetLinesPerPageInLogger)
						
    ];
}

