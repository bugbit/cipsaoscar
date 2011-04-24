#include "__PCH_Core.h"

#include "FlostiEngine.h"
#include "Process.h"
#include "Core.h"
#include "Exceptions/Exception.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Camera.h"
#include "Input/InputManager.h"
#include "Input/ActionToInput.h"
#include "Logger/Logger.h"
#include "Console.h"
#include "Network/NetworkManager.h"
#include "xml/XMLTreeNode.h"
#include "Script/ScriptManager.h"
#include "GUI/GuiManager.h"
#include "Language/LanguageManager.h"


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CFlostiEngine::Done ()
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
void CFlostiEngine::Release ()
{
	//Primero eliminamos el proceso y despues el Core, ya que puede haber algun elemento que necesite el Core para eliminar memoria reservada.
	//(por ejemplo el PhysicManager es requerido por los actores creados para ser eliminados)
	std::vector<CProcess*>::const_iterator it			= m_pProcesses.begin();
	std::vector<CProcess*>::const_iterator itEnd	= m_pProcesses.end();
	for(; it != itEnd; ++it)
	{
		CProcess* process = *it;
		CHECKED_DELETE(process);
	}
	CHECKED_DELETE(m_pCore);
	CHECKED_DELETE(m_pConsole);

	//Lo ultimo a eliminar
	CLogger* logger = CLogger::GetSingletonPtr();
	delete logger;
}

bool CFlostiEngine::LoadInitParams (const std::string& xmlFile)
{
	new CLogger();

	LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Iniciando la lectua del archivo de configuracion --> %s", xmlFile.c_str());

	CXMLTreeNode parser;
	if (!parser.LoadFile(xmlFile.c_str()))
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams->Error al intentar leer el archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	
	CXMLTreeNode  m = parser["ScreenResolution"];
	if (m.Exists())
	{
		uint32 width = m.GetIntProperty("width",m_sInitParams.m_ScreenResolution.x);
		uint32 height = m.GetIntProperty("height",m_sInitParams.m_ScreenResolution.y);
		m_sInitParams.m_ScreenResolution.x = width;
		m_sInitParams.m_ScreenResolution.y = height;
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Resolucion de la aplicacion(%d,%d)", width, height);
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams->Error al intentar leer el tag <ScreenResolution> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["WindowsPosition"];
	if (m.Exists())
	{
		m_sInitParams.m_WindowsPosition = m.GetVect2iProperty("position",m_sInitParams.m_WindowsPosition);
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Posición de la win32 (%d,%d)", m_sInitParams.m_WindowsPosition.x, m_sInitParams.m_WindowsPosition.y);
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams->Error al intentar leer el tag <WindowsPosition> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Rendermode"];
	if (m.Exists())
	{
		m_sInitParams.m_bFullScreen = m.GetBoolProperty("fullscreenMode",m_sInitParams.m_bFullScreen);
		if (m_sInitParams.m_bActivateConsole)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: FullScreen Mode ");
		}
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Windowed Mode");
		}
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams->Error al intentar leer el tag <Rendermode> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());		
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Console_Logger"];
	if (m.Exists())
	{
		m_sInitParams.m_bActivateConsole = m.GetBoolProperty("activateConsole",m_sInitParams.m_bActivateConsole);
		if (m_sInitParams.m_bActivateConsole)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Consola activada");
		}
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Consola desactivada");
		}
		
		std::string logLevel = m.GetPszProperty("Log_Level", "INFORMATION");
		if(logLevel.compare("INFORMATION") == 0)
		{
			LOGGER->SetLogLevel(ELL_INFORMATION);
		}
		if(logLevel.compare("WARNING") == 0)
		{
			LOGGER->SetLogLevel(ELL_WARNING);
		}
		if(logLevel.compare("ERROR") == 0)
		{
			LOGGER->SetLogLevel(ELL_ERROR);
		} 
		if(logLevel.compare("NONE") == 0)
		{
			LOGGER->SetLogLevel(ELL_NONE);
		}
		

		m_sInitParams.m_uLinesPerPageInLogger = m.GetIntProperty("linesPerPageInLogger",m_sInitParams.m_uLinesPerPageInLogger);

		m_sInitParams.m_bActivateLogger = m.GetBoolProperty("activateLogger",m_sInitParams.m_bActivateLogger);
		SetLinesPerPageInLogger(m_sInitParams.m_uLinesPerPageInLogger);
		if (m_sInitParams.m_bActivateLogger)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Logger activado");
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: %d Lines Per Page in Logger", m_sInitParams.m_uLinesPerPageInLogger);
		}
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Logger desactivado");
		}

	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams->Error al intentar leer el tag <Console_Logger> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Mouse"];
	if (m.Exists())
	{
		m_sInitParams.m_bExclusiveModeinMouse = m.GetBoolProperty("exclusiveModeinMouse", m_sInitParams.m_bExclusiveModeinMouse);
		if (m_sInitParams.m_bExclusiveModeinMouse)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Exclusive mode in mouse");
			m_sInitParams.m_bDrawPointerMouse = m.GetBoolProperty("drawPointerMouse", m_sInitParams.m_bDrawPointerMouse);
		}
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Nonexclusive mode in mouse");
			m_sInitParams.m_bDrawPointerMouse = false;
		}
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Mouse> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["ActionToInput"];
	if (m.Exists())
	{
		m_sInitParams.m_sAction2InputXML = m.GetPszProperty("fileXML", m_sInitParams.m_sInitFontsXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <ActionToInput> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Fonts"];
	if (m.Exists())
	{
		m_sInitParams.m_sInitFontsXML = m.GetPszProperty("fontsXML", m_sInitParams.m_sInitFontsXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Fonts> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Languages"];
	if (m.Exists())
	{
		/*<Languages default="spanish">
			<Language languageXML="./Data/Language/spanish.xml"/>
			<Language languageXML="./Data/Language/catalan.xml"/>
			<Language languageXML="./Data/Language/english.xml"/>
		</Languages>*/
		m_sInitParams.m_sCurrentLanguage = m.GetPszProperty("default", "spanish");
		int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i)
    {
			//for each lanaguage
			std::string XMLFile = m(i).GetPszProperty("languageXML", "");
			m_sInitParams.m_vLanguageXML.push_back(XMLFile);
		}

	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Fonts> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}


	m = parser["GUI"];
	if (m.Exists())
	{
		m_sInitParams.m_sInitGuiXML = m.GetPszProperty("initGuiXML", "./Data/GUI/initGui.xml");
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: El fichero con la configuracion de la GUI es-->%s", m_sInitParams.m_sInitGuiXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <GUI> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Sound"];
	if (m.Exists())
	{
		m_sInitParams.m_sInitSoundsXML = m.GetPszProperty("soundXML", "./Data/Sound/sound.xml");
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: El fichero con los sonidos a cargar es-->%s", m_sInitParams.m_sInitSoundsXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Sound> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Avi"];
	if (m.Exists())
	{
		m_sInitParams.m_sInitAvisXML = m.GetPszProperty("aviXML", "./Data/AviFiles/AviFiles.xml");
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: El fichero con los avis a cargar es-->%s", m_sInitParams.m_sInitAvisXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Avi> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	

	m = parser["PhysX"];
	if (m.Exists())
	{
		m_sInitParams.m_bInitPhysX = m.GetBoolProperty("initPhysX", false);
		if (m_sInitParams.m_bInitPhysX)
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion inicializara physx", m_sInitParams.m_sInitGuiXML.c_str());
		else
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion no inicializara physx", m_sInitParams.m_sInitGuiXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <PhysX> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["ComputerVision"];
	if (m.Exists())
	{
		m_sInitParams.m_bInitComputerVision = m.GetBoolProperty("initComputerVision", false);
		if (m_sInitParams.m_bInitComputerVision)
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion inicializara computerVision", m_sInitParams.m_sInitGuiXML.c_str());
		else
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion no inicializara computerVision", m_sInitParams.m_sInitGuiXML.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <ComputerVision> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["InitScripts"];
	if (m.Exists())
	{
		/* 
		<InitScripts>
    	<Script action="_core          = getCore();"/>
    	<Script action="_inputM        = getCore():getInput();"/>
    	<Script action="_renderM       = getCore():getRender();"/>
		</InitScripts>
		*/
		int count = m.GetNumChildren();
		m_sInitParams.m_sInitScript = "";
    for (int i = 0; i < count; ++i)
    {
			//for each Script
			std::string action = m(i).GetPszProperty("action", "");
			m_sInitParams.m_sInitScript += action;
			if (action.compare("") != 0)
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion ejecutara el script de inicializacion: %s", action.c_str());
			}
		}
		if (m_sInitParams.m_sInitScript.compare("") == 0)
				LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion no inicializara ningun script de incializacion");
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Script> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m = parser["Process"];
	if (m.Exists())
	{
		m_sInitParams.m_sProcess = m.GetPszProperty("initProcess", "");
		if (m_sInitParams.m_sInitScript.compare("") == 0)
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion ejecutara por defecto el primer proceso registrado");
		else
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: La aplicacion ejecutara por defecto el proceso: %s", m_sInitParams.m_sProcess.c_str());
	}
	else
	{
		std::string msg_error = "CFlostiEngine::LoadInitParams-> Error al intentar leer el tag <Process> del archivo de configuracion: " + xmlFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}


	//---DEBUG logRender:
	std::string kaka;
	kaka = "1_2_3_4_5_6_7_8_9_10_11_12_13_14_15_16_17_18_19_20_21_22_23_24_25_26_27_28_29_30_31_32_33_34_35_36_37_38_39_40_41_42_43_44_45_46_47_48_49_50_51_52_53_54_55_56_57_58_59_60_61_62_63_64_65_66_67_68_69_70_71_72_73_74_75_76_77_78_79_80_81_82_83_84_85_86_87_88_89_90_91_92_93_94_95_96_97_98_99_100_101_102_103_104_105_106_107_108_109_110_111_112_113_114_115_116_117_118_119_120_121_122_123_124_125_126_127_128_129_130_131_132_133_134";
	LOGGER->AddNewLog(ELL_INFORMATION, kaka.c_str());
	//---

	LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Finalizada la lectua del archivo de configuracion --> %s", xmlFile.c_str());

	return true;
}

bool CFlostiEngine::Init (std::vector<CProcess*>& processes, HWND hwnd)
{
	m_pProcesses = processes;
	m_pCore = new CCore();
	assert(m_pCore);
	m_pConsole = new CConsole();
	assert(m_pConsole);
	m_bIsOk = m_pCore->Init(	hwnd, this, m_pConsole, m_sInitParams);

	if (m_bIsOk)
	{
		if (m_sInitParams.m_bActivateConsole)
		{
			m_bIsOk = m_pConsole->Init(m_pCore->GetScriptManager());
		}
		
		if (m_bIsOk)
		{
			//Primero miramos que como minimo haya regitrado un proceso:
			if (m_pProcesses.size()== 0)
			{
				std::string msg_error = "CFlostiEngine::Init-> Como minimo se debe registrar un proceso!";
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
				throw CException(__FILE__, __LINE__, msg_error);
			}

			//Miramos que no haya ningun puntero a proceso nulo registrado:
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine::Init-> Se han registrado los siguientes procesos:");
			std::vector<CProcess*>::const_iterator it			= m_pProcesses.begin();
			std::vector<CProcess*>::const_iterator itEnd	= m_pProcesses.end();
			for(; it != itEnd; ++it)
			{
				CProcess* process = *it;
				if (!process)
				{
					std::string msg_error = "CFlostiEngine::Init-> Se ha registrado un puntero a proceso nulo!";
					LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
					throw CException(__FILE__, __LINE__, msg_error);
				}
				LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine::Init-> [Registrado proceso: %s]",process->GetProcessName().c_str());
			}

			//Finalmente vamos a encontrar el proceso que ha de inicializarse, de no encontrarse se pilla el primero a sakete:
			it		= m_pProcesses.begin();
			itEnd	= m_pProcesses.end();

			bool found = false;
			for(; it != itEnd; ++it)
			{
				CProcess * process = *it;
				if (process->GetProcessName().compare(m_sInitParams.m_sProcess) == 0)
				{
					m_pCurrentProcess = process;
					found = true;
				}
			}

			if (!found)
			{
				LOGGER->AddNewLog(ELL_WARNING, "CFlostiEngine::Init-> No se ha encontrado el init process: %s", m_sInitParams.m_sProcess.c_str());
				m_pCurrentProcess = m_pProcesses[0];
			}
			LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine::Init-> Se ha seteado como currentProcess: %s", m_pCurrentProcess->GetProcessName().c_str());
			m_bIsOk = m_pCurrentProcess->Init();
		}
	}

	if (m_bIsOk)
	{
		CScriptManager* scriptM = m_pCore->GetScriptManager();
		std::vector<CProcess*>::const_iterator it			= m_pProcesses.begin();
		std::vector<CProcess*>::const_iterator itEnd	= m_pProcesses.end();
		bool found = false;
		for(; it != itEnd; ++it)
		{
			CProcess * process = *it;
			scriptM->AddNewScriptRegister(process);
			
		}
		scriptM->RegisterFunctions(m_sInitParams.m_sInitScript);
		m_pCore->SetProcess(m_pCurrentProcess);
	}

	if (!m_bIsOk)
	{
		Release();
	}

	return m_bIsOk;
}

void CFlostiEngine::MsgProc (WPARAM wp, LPARAM lp)
{
	CNetworkManager* pNet = m_pCore->GetNetworkManager();
	if (pNet->IsOk())
	{
		pNet->MsgProc(wp, lp);
	}
}



void CFlostiEngine::ResetCurrentProcess ()
{
	m_pCurrentProcess->Done();
	if (!m_pCurrentProcess->Init())
	{
		std::string msg_error = "CFlostiEngine::ResetCurrentProcess-> Error al iniciar de nuevo el proceso: " + m_pCurrentProcess->GetProcessName();
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}		
}

void CFlostiEngine::ChangeProcess (const std::string& newProcess)
{
	//Primero de todo vamos a buscar si realmente existe el proceso newProcess:
	bool found = false;
	CProcess* process = NULL;
	std::vector<CProcess*>::const_iterator it			= m_pProcesses.begin();
	std::vector<CProcess*>::const_iterator itEnd	= m_pProcesses.end();
	for(; it != itEnd; ++it)
	{
		 process = *it;
		if (process->GetProcessName().compare(newProcess) == 0)
		{
			//Lo hemos encontrado
			found = true;
			break;
		}
	}

	if (found)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine::ChangeProcess-> Hemos cambiado del proceso [%s] al proceso [%s]",m_pCurrentProcess->GetProcessName().c_str(), newProcess.c_str());
		m_pCurrentProcess->Done();
		m_pCurrentProcess = process;
		if (!m_pCurrentProcess->Init())
		{	
			std::string msg_error = "CFlostiEngine::ChangeProcess-> Error al iniciar el nuevo el proceso a ejecutar: " + m_pCurrentProcess->GetProcessName();
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			throw CException(__FILE__, __LINE__, msg_error);
		}
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine::ChangeProcess-> El proceso [%s] al que se quiere cambiar no esta registrado", newProcess.c_str());	
	}
}


bool CFlostiEngine::Exit()
{
	return m_pCurrentProcess->HasFinished();
}

void CFlostiEngine::Render()
{
	CRenderManager* renderManager = m_pCore->GetRenderManager();
	CCamera* camera = m_pCurrentProcess->GetCamera();

	if(m_bScreenshot)
	{
		renderManager->Screenshot_BeginRendering();
		renderManager->SetupMatrices(camera);
		{
			RenderScene(renderManager);
		}
		renderManager->Screenshot_EndRendering();
		m_bScreenshot = false;
	}

	renderManager->BeginRendering();
	renderManager->SetupMatrices(camera);
	{
		RenderScene(renderManager);
	}
	renderManager->EndRendering();
}

void CFlostiEngine::RenderScene (CRenderManager* renderManager)
{
	//---Render the scene---
	CFontManager* fm = m_pCore->GetFontManager();
	// >>>> INIT
	m_pCore->Render();

	m_pCurrentProcess->RenderScene(renderManager, fm);

	
	// >>>_2D	
	renderManager->DisbaleZBuffering();
	renderManager->EnableAlphaBlend();

	m_pCurrentProcess->RenderScene2D(renderManager, fm);
	if (m_pCore->GetGUIManager()->IsOk()) 
	{
		m_pCore->GetGUIManager()->Render(renderManager, fm);
	}

	renderManager->DisbaleAlphaBlend();
	renderManager->EnableZBuffering();
	//<<<<_2D

		
	m_pCurrentProcess->RenderSceneOverGUI(renderManager, fm);


	// >>>_2D
	renderManager->DisbaleZBuffering();
	renderManager->EnableAlphaBlend();

	if (m_pCore->GetGUIManager()->IsOk()) 
	{
		m_pCore->GetGUIManager()->RenderPointerMouse(renderManager, fm);
	}
	

	m_pCore->RenderAssetErrors();

	//Draw Text Info
	if (m_bDrawDebugInf)
	{
		uint32 finalPosY = m_pCurrentProcess->RenderDebugInfo(renderManager, fm, m_Timer.GetFPS());
		m_pCurrentProcess->EndRenderDebugInfo(renderManager, finalPosY);
		m_pCurrentProcess->RenderDebugInfo(renderManager, fm, m_Timer.GetFPS());

		if (m_sInitParams.m_bActivateConsole)
		{
			m_pConsole->Render(renderManager, fm);
		}

		if (m_sInitParams.m_bActivateLogger)
		{	
			m_LogRender.Render(renderManager, fm);
		}	
	}

	renderManager->DisbaleAlphaBlend();
	renderManager->EnableZBuffering();
	//// <<<_2D	
 //>>>>> END
}

void CFlostiEngine::Update()
{
	m_Timer.Update();
	float elapsedTime = m_Timer.GetElapsedTime();
	m_pCore->Update(elapsedTime);
	
	if (m_sInitParams.m_bActivateConsole)
	{
		if (!m_pConsole->IsActive())
		{
			m_pCurrentProcess->Update(elapsedTime);
		}
		m_pConsole->Update(elapsedTime);
	}
	else
	{
		m_pCurrentProcess->Update(elapsedTime);
	}

	m_LogRender.Update(elapsedTime);

	
	

	UpdateInputActions();
}

void CFlostiEngine::UpdateInputActions()
{
	CInputManager*im = m_pCore->GetInputManager();
	CActionToInput* action2Input = m_pCore->GetActionToInput();

	if (m_sInitParams.m_bActivateLogger)
	{

		if( action2Input->DoAction(ACTION_LOGGER) )
		{
			m_LogRender.SetVisible(!m_LogRender.GetVisible());
		}

		if( action2Input->DoAction(ACTION_SCREENSHOT) )
		{
			Screenshot();
		}

		if( action2Input->DoAction(ACTION_SAVE_LOGS_IN_FILE) )
		{
			m_pCore->SaveLogsInFile();
		}

		if( action2Input->DoAction(ACTION_LOGRENDER_PAGEDOWN) )
		{
			m_LogRender.PageDown();
		}

		if( action2Input->DoAction(ACTION_LOGRENDER_PAGEUP) )
		{
			m_LogRender.PageUp();
		}

		if( action2Input->DoAction(ACTION_LOGRENDER_PREVLINE) )
		{
			m_LogRender.PrevLine();
		}
		
		if( action2Input->DoAction(ACTION_LOGRENDER_NEXTLINE) )
		{
			m_LogRender.NextLine();
		}

		if( action2Input->DoAction(ACTION_RELOAD_TTFS) )
		{
			m_pCore->GetFontManager()->ReloadTTFs();
		}

		if( action2Input->DoAction(ACTION_RELOAD_LANGUAGES) )
		{
			m_pCore->GetLanguageManager()->LoadXMLs();
		}

		if( action2Input->DoAction(ACTION_RELOAD_ACTIONS) )
		{
			action2Input->ReloadXML();
		}
	}
	

	if( action2Input->DoAction(ACTION_CONSOLE) )
	{
		if (m_sInitParams.m_bActivateConsole)	
		{
			m_pConsole->SetActive( !m_pConsole->IsActive() );
		}
	}
}

