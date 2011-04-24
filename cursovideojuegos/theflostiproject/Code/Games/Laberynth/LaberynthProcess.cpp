#include "__PCH_Laberynth.h"


//---Engine Includes----
#include "Input/InputManager.h"
#include "Sound/SoundManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Script/ScriptManager.h"
#include "Logger/Logger.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "luabind/luabind.hpp"
#include "xml/XMLTreeNode.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "PhysX/PhysicASELoader.h"
#include "PhysX/PhysicCookingMesh.h"
//----------------------

//---Game Includes-------
#include "LaberynthProcess.h"
#include "GameLogicLaberynth.h"
#include "LaberynthBoard.h"
//Inputs...
#include "LaberynthBoardInput.h"
#include "KeyboardBoardInput.h"
#include "ComputerVisionBoardInput.h"
//EndGameConditions...
#include "EndGameCondition.h"
#include "VictoryOrLossEndGameCondition.h"
#include "TimeOutEndGameCondition.h"
//-----------------------

CLaberynthProcess::CLaberynthProcess(const std::string& processName)
: CProcess(processName)
, m_pObject3D(NULL)
, m_pGameLogicLaberynth(NULL)
, m_bInShow(true)
, m_pASEBall(NULL)
, m_pASEBaseBoard(NULL)
, m_fBallRadius(2.f)
, m_bInBoardSettings(false)
{
	m_sShow_CameraPos.m_fYaw		= 1.06f;
	m_sShow_CameraPos.m_fPitch	= -1.00f;
	m_sShow_CameraPos.m_fZoom		= 184.f;

	m_sGame_CameraPos.m_fYaw		= 1.06f;
	m_sGame_CameraPos.m_fPitch	= -1.32f;
	m_sGame_CameraPos.m_fZoom		= 184.f;

	m_sSettings_CameraPos.m_fYaw			= 1.18f;
	m_sSettings_CameraPos.m_fPitch		= -0.91f;
	m_sSettings_CameraPos.m_fZoom			= 244.f;
	m_sSettings_CameraPos.m_Position	= Vect3f(-70.68f,Y_SETTINGS,31.88f);
}

//----------------------------------------
// -- Initialize
//----------------------------------------
bool CLaberynthProcess::Init ()
{
	CProcess::m_bIsOk = false;
	m_pObject3D = new CObject3D(Vect3f(0.f,0.f,0.f), m_sShow_CameraPos.m_fYaw, m_sShow_CameraPos.m_fPitch);
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;
	m_pCamera = new CThPSCamera(1.f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_pObject3D,m_sShow_CameraPos.m_fZoom);

	LoadLevels("./Data/Laberynth_Levels/levels.xml");


	if (m_pObject3D && m_pCamera) 
	{
		CProcess::m_bIsOk = true;
	}

	if (CProcess::m_bIsOk)
	{
		//Configurar el board para la pantalla de settings de la GUI:
		m_BoardForSettings = new CLaberynthBoard(	m_pASEBaseBoard, m_pASEBall, m_fBallRadius,
																							m_Levels[0].m_pASEBoard, 
																							m_Levels[0].m_sPhysXBoardName,
																							m_Levels[0].m_GoalPosition,
																							m_Levels[0].m_StartPosition, true);
		CProcess::m_bIsOk = (m_BoardForSettings!=NULL);

		m_BoardInputForSettings = new CKeyboardBoardInput(m_BoardForSettings, m_sGameSettings.m_fForceRJ1, m_sGameSettings.m_fForceRJ2);
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

//----------------------------------------
// -- Free memory
//----------------------------------------
void CLaberynthProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);
	CHECKED_DELETE(m_pGameLogicLaberynth);

	CHECKED_DELETE(m_BoardForSettings);
	CHECKED_DELETE(m_BoardInputForSettings);

		

	if (m_pASEBaseBoard)
	{
		m_pASEBaseBoard->CleanUp();
		delete m_pASEBaseBoard;
		m_pASEBaseBoard = NULL;
	}

	if (m_pASEBall)
	{
		m_pASEBall->CleanUp();
		delete m_pASEBall;
		m_pASEBall = NULL;
	}


	std::vector<SLevel>::iterator it = m_Levels.begin();
	std::vector<SLevel>::iterator itEnd = m_Levels.end();
	for(; it != itEnd; ++it)
	{
		CASEObject* ase = it->m_pASEBoard;
		if (ase)
		{
			ase->CleanUp();
			delete ase;
			ase = NULL;
		}	
	}
	m_Levels.clear();
}

bool CLaberynthProcess::End () 
{
	CHECKED_DELETE(m_pGameLogicLaberynth);
	return true;
}


bool CLaberynthProcess::Start (bool show) 
{
	//Primero de todo reiniciamos el juego por si hubiera una partida ya creada
	CHECKED_DELETE(m_pGameLogicLaberynth);

	CProcess::m_bIsOk = false;
	CProcess::m_bStart = true;
	LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess:: Inicializando nueva partida");

	//----Cargamos el tablero con el laberinto a jugar (level)
	LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess:: -------Game Settings---");

	//A partir del level vamos a cargar el modelo geometrico y fisico de este
	CLaberynthBoard * board = NULL;
	if (m_sGameSettings.m_uLevel> m_Levels.size()-1)
	{
		CProcess::m_bIsOk = false;
		LOGGER->AddNewLog(ELL_ERROR, "CLaberynthProcess:: Error al intentar iniciar un level: %d no existente en el xml de configuracion", m_sGameSettings.m_uLevel);
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess::Start-> Creado Board con level %d", m_sGameSettings.m_uLevel);
		board = new CLaberynthBoard(	m_pASEBaseBoard, m_pASEBall, m_fBallRadius,
																	m_Levels[m_sGameSettings.m_uLevel].m_pASEBoard, 
																	m_Levels[m_sGameSettings.m_uLevel].m_sPhysXBoardName,
																	m_Levels[m_sGameSettings.m_uLevel].m_GoalPosition,
																	m_Levels[m_sGameSettings.m_uLevel].m_StartPosition);
		CProcess::m_bIsOk = (board!=NULL);
	}
 

	//----Configuramos los boardInputs
	CComputerVisionBoardInput *computerVisionBoardInput = NULL;
	CKeyboardBoardInput *keyboardInput = NULL;
	std::vector<CLaberynthBoardInput*> vecBoardInput;
	if (CProcess::m_bIsOk)
	{
		keyboardInput = new CKeyboardBoardInput(board, m_sGameSettings.m_fForceRJ1, m_sGameSettings.m_fForceRJ2);
		assert(keyboardInput);
		LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess::Start-> Anyadido el KeyboardBoardInput");
		vecBoardInput.push_back(keyboardInput);

		if (m_sGameSettings.m_bUsingComputerVision)
		{
			computerVisionBoardInput = new CComputerVisionBoardInput(board, m_sGameSettings.m_fForceRJ1, m_sGameSettings.m_fForceRJ2);
			assert(computerVisionBoardInput );
			LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess::Start-> Anyadido el ComputerVisionBoardInput");
			vecBoardInput.push_back(computerVisionBoardInput );
			CProcess::m_bIsOk = (keyboardInput!= 0) && (computerVisionBoardInput != 0);
		}	
		else
		{
			CProcess::m_bIsOk = (keyboardInput!= 0);
		}
	}
	
	//----Configuramos las condiciones de finalizacion
	std::vector<CEndGameCondition*> vecEndGameConditions;
	if (CProcess::m_bIsOk)
	{
		CVictoryOrLossEndGameCondition * victoryorLossEndGameCondition = new CVictoryOrLossEndGameCondition();
		assert(victoryorLossEndGameCondition);
		vecEndGameConditions.push_back(victoryorLossEndGameCondition);
		LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess::Start-> Anyadimos  VictoryorLossEndGameCondition");

		if (m_sGameSettings.m_bTimeOutEndCondition)
		{
			CTimeOutEndGameCondition * timeOutEndGameCondition = new CTimeOutEndGameCondition(200.f);
			assert(timeOutEndGameCondition);
			vecEndGameConditions.push_back(timeOutEndGameCondition);
			LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess::Start-> Anyadimos  timeOutEndGameCondition");
			CProcess::m_bIsOk = (timeOutEndGameCondition!= 0);
		}

		CProcess::m_bIsOk = (victoryorLossEndGameCondition!= 0) && CProcess::m_bIsOk;
	}
		

	//----Finalmente creamos una nueva partida
	if (CProcess::m_bIsOk)
	{
		m_pGameLogicLaberynth = new CGameLogicLaberynth(board,vecBoardInput,vecEndGameConditions);
		assert(m_pGameLogicLaberynth);
		if(m_pGameLogicLaberynth)
		{
			CProcess::m_bIsOk = true;
			LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess:: ----- END Game Settings----");
		}
	}
	
	if (CProcess::m_bIsOk)
	{	
		if (m_bInBoardSettings)
		{
			m_pObject3D->SetPitch(m_sSettings_CameraPos.m_fPitch);
			m_pObject3D->SetYaw(m_sSettings_CameraPos.m_fYaw);
			m_pObject3D->SetPosition(m_sSettings_CameraPos.m_Position);
			static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sSettings_CameraPos.m_fZoom);
		}
		else if (show)
		{
			//Si hacemos un simple start (show = true) dejamos la camara rotar alrededor del tablero hasta que el jugador pulse la tecla "S"
			m_bInShow = true;
			m_pObject3D->SetPitch(m_sShow_CameraPos.m_fPitch);
			m_pObject3D->SetYaw(m_sShow_CameraPos.m_fYaw);
			m_pObject3D->SetPosition(m_sShow_CameraPos.m_Position);
			static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sShow_CameraPos.m_fZoom);
			//Finalmente reproducimos musica de inicio
			CORE->GetSoundManager()->PlayAction2D("initGame");
		}		
		else
		{
			//Si hacemos un restart de partida (show = false) ponemos la camara en modo jugar
			m_bInShow = false;
			m_pObject3D->SetPitch(m_sGame_CameraPos.m_fPitch);
			m_pObject3D->SetYaw(m_sGame_CameraPos.m_fYaw);
			m_pObject3D->SetPosition(m_sGame_CameraPos.m_Position);
			static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sGame_CameraPos.m_fZoom);
			
		}
	}

	if (!CProcess::m_bIsOk)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CLaberynthProcess:: Error en la inicializacion del proceso");
		Release();
	}
	return CProcess::m_bIsOk;
}


void CLaberynthProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	if (CProcess::m_bStart)
	{
		if (m_bInShow && !m_bInBoardSettings)
		{
			//pintamos el texto de ke tiene que pulsar la tecla "S" para empezar
			fm->DrawDefaultText(400,400,colBLACK,"PRESS S TO START");
		}
		if (m_pGameLogicLaberynth)
		{
			m_pGameLogicLaberynth->RenderScene(renderManager, fm, m_bInBoardSettings);
		}
	}
}

void CLaberynthProcess::RenderSceneOverGUI (CRenderManager* renderManager, CFontManager* fm)
{
	if (m_bInBoardSettings)
	{
		m_BoardForSettings->RenderScene(renderManager, fm );
	}
}

uint32 CLaberynthProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{

	uint32 posY = 0;
	uint32 posX = m_PosRenderDebugInfo.x;
	posY = CProcess::RenderDebugInfo(renderManager, fm, fps);
	if (m_bRenderDebugInfo)
	{
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Yaw del objeto3d atachado a la camara: %f", m_pObject3D->GetYaw());
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Pitch del objeto3d atachado a la camara: %f", m_pObject3D->GetPitch());
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Pos del objeto3d atachado a la camara: (%f,%f,%f)", m_pObject3D->GetPosition().x, m_pObject3D->GetPosition().y, m_pObject3D->GetPosition().z);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Zoom de la camara: %f", static_cast<CThPSCamera*>(m_pCamera)->GetZoom());
	}
	return posY;
}


void CLaberynthProcess::SetLevel (const std::string& levelName)
{
	bool existLevel = false;
	for(uint32 cont = 0; cont < m_Levels.size(); cont++)
	{
		if (m_Levels[cont].m_sLevelName == levelName)
		{
			m_sGameSettings.m_uLevel = cont;
			existLevel = true;
		}
	}

	if (!existLevel)
	{
		m_sGameSettings.m_uLevel = 0;	
		LOGGER->AddNewLog(ELL_WARNING, "CLaberynthProcess::SetLevel Se ha intentado leer un nivel que no registrado--> %s", levelName.c_str());
	}
	
}


void CLaberynthProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);
	CInputManager* inputManager = CORE->GetInputManager();

	/*if ( m_bInBoardSettings)
	{
		UpdateInputActions(inputManager);
	}*/

	if (CProcess::m_bStart && !m_bInBoardSettings)
	{
		if (m_bInShow)
		{
			m_pObject3D->SetYaw(m_pObject3D->GetYaw()+elapsedTime*0.1f);
			if (inputManager->IsUpDown(IDV_KEYBOARD, KEY_S))
			{
				m_bInShow = false;
				m_pObject3D->SetPitch(m_sGame_CameraPos.m_fPitch);
				m_pObject3D->SetYaw(m_sGame_CameraPos.m_fYaw);
				static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sGame_CameraPos.m_fZoom);
			}
		}
		else
		{
			UpdateInputActions(inputManager);

			if (m_pGameLogicLaberynth)
			{
				m_pGameLogicLaberynth->Update(elapsedTime);
			}
		}
	}
	
	if (m_bInBoardSettings)
	{
		m_BoardForSettings->UpdateScene(elapsedTime);
		m_BoardInputForSettings->UpdateInputs();
	}
}

std::string CLaberynthProcess::GetLevel ()
{
	assert(m_Levels.size());
	return m_Levels[m_sGameSettings.m_uLevel].m_sLevelName;
}

bool CLaberynthProcess::LoadLevels(const std::string& xmlFile)
{
	CXMLTreeNode NewXML;

	LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess:: Iniciando la lectua del archivo de configuracion de levels--> %s", xmlFile.c_str());

	CXMLTreeNode parser;
	if (!parser.LoadFile(xmlFile.c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CLaberynthProcess:: Error al leer el archivo de configuracion de levels %s", xmlFile.c_str());
		return false;
	}

	CXMLTreeNode  m = parser["LEVELS"];
	if (m.Exists())
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		assert(physicManager);
		CRenderManager* rm = CORE->GetRenderManager();
		assert(rm);
		CASETextureManager::GetInstance()->SetTexturePath("./Data/Textures/");
		
		//-----Board Base----
		std::string meshBoardBase = m.GetPszProperty("boardBase_meshFileName");
		std::string physxBoardBase = m.GetPszProperty("boardBase_physxFileName");
		m_pASEBaseBoard = new CASEObject();
		assert(m_pASEBaseBoard);
		m_pASEBaseBoard->Load(meshBoardBase.c_str(), rm, false);
		LoadOrCreatePhysXMesh(meshBoardBase,physxBoardBase,"BoardBase");

		//-----Ball----
		std::string meshBall = m.GetPszProperty("ball_meshFileName");
		std::string physxBall = m.GetPszProperty("ball_physxFileName");
		m_pASEBall = new CASEObject();
		assert(m_pASEBall);
		m_pASEBall->Load(meshBall.c_str(), rm, false);
		LoadOrCreatePhysXMesh(meshBall,physxBall,"Ball");

		m_fBallRadius =  m.GetFloatProperty("ballRadius");

		int count = m.GetNumChildren();
		for (int i = 0; i < count; ++i)
		{
			CXMLTreeNode pNewNode = m(i);

			SLevel newLevel;

			//Para cada Level leemos su informacion comun-->
			newLevel.m_sLevelName				= pNewNode.GetPszProperty("levelName");
			std::string meshFileName		= pNewNode.GetPszProperty("meshFileName");
			newLevel.m_sPhysXBoardName	= pNewNode.GetPszProperty("physxFileName");
			newLevel.m_fTimeOut					= pNewNode.GetFloatProperty("timeOut");
			newLevel.m_GoalPosition.x		= pNewNode.GetFloatProperty("posXGoal");
			newLevel.m_GoalPosition.z		= pNewNode.GetFloatProperty("posZGoal");
			newLevel.m_StartPosition.x	= pNewNode.GetFloatProperty("posXStart");
			newLevel.m_StartPosition.z	= pNewNode.GetFloatProperty("posZStart");
			


			LOGGER->AddNewLog(ELL_INFORMATION, "CLaberynthProcess:: Nuevo Level cargado con tiempo limite: %f, meshFileName: %s, physXFileName: %s y goalPosition: (%f,%f) startPosition: (%f,%f)",
																	newLevel.m_fTimeOut, meshFileName.c_str(), newLevel.m_sPhysXBoardName.c_str(),
																	newLevel.m_GoalPosition.x, newLevel.m_GoalPosition.z,
																	newLevel.m_StartPosition.x, newLevel.m_StartPosition.z);


			
			newLevel.m_pASEBoard = new CASEObject();
			newLevel.m_pASEBoard->Load(meshFileName.c_str(), rm, false);
			//Como nombre para la malla fisica utilizamos el nombre mismo del fichero .BIN que contiene (o contendra) la malla fisica:
			LoadOrCreatePhysXMesh(meshFileName,newLevel.m_sPhysXBoardName,newLevel.m_sPhysXBoardName);

			//Finalmente guardamos en nuevo level en el vector
			m_Levels.push_back(newLevel);
		}
	}
	
	LOGGER->AddNewLog(ELL_INFORMATION, "CFlostiEngine:: Finalizada la lectua del archivo de configuracion --> %s", xmlFile.c_str());

	return true;
}

void CLaberynthProcess::SetInBoardSettings (bool inSettings)
{
	m_bInBoardSettings = inSettings;

	if (m_bInBoardSettings)
	{
		m_pObject3D->SetPitch(m_sSettings_CameraPos.m_fPitch);
		m_pObject3D->SetYaw(m_sSettings_CameraPos.m_fYaw);
		m_pObject3D->SetPosition(m_sSettings_CameraPos.m_Position);
		static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sSettings_CameraPos.m_fZoom);
	}
	else if (m_bInShow)
	{
		//Si show = true dejamos la camara rotar alrededor del tablero hasta que el jugador pulse la tecla "S"
		m_pObject3D->SetPitch(m_sShow_CameraPos.m_fPitch);
		m_pObject3D->SetYaw(m_sShow_CameraPos.m_fYaw);
		m_pObject3D->SetPosition(m_sShow_CameraPos.m_Position);
		static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sShow_CameraPos.m_fZoom);
	}		
	else
	{
		//Si hacemos un restart de partida (show = false) ponemos la camara en modo jugar
		m_pObject3D->SetPitch(m_sGame_CameraPos.m_fPitch);
		m_pObject3D->SetYaw(m_sGame_CameraPos.m_fYaw);
		m_pObject3D->SetPosition(m_sGame_CameraPos.m_Position);
		static_cast<CThPSCamera*>(m_pCamera)->SetZoom(m_sGame_CameraPos.m_fZoom);
	}
}

void CLaberynthProcess::LoadOrCreatePhysXMesh(	const std::string& aseFileName, 
																								const std::string& binFileName, 
																								const std::string& physXMeshName )
{
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);

	FILE * pFile;
	pFile = fopen (binFileName.c_str(),"r");
	if (pFile==NULL)
	{
		//Si no existe el fichero BIN lo creamos
		std::vector<uint32> faces;
		std::vector<Vect3f> vertices;
		CPhysicASELoader::ReadMeshFromASE(aseFileName,vertices, faces);
		physicManager->GetCookingMesh()->SavePhysicMesh(vertices, faces, binFileName);
	}
	else
	{
		fclose (pFile);
	}
	physicManager->GetCookingMesh()->CreatePhysicMesh(binFileName, physXMeshName);
}
void CLaberynthProcess::SetForceRJ1 (float force)
{
	m_sGameSettings.m_fForceRJ1 = force;
	m_BoardInputForSettings->SetForceRJ1(m_sGameSettings.m_fForceRJ1);
	if (m_pGameLogicLaberynth)
	{
		m_pGameLogicLaberynth->SetForceRJ1(m_sGameSettings.m_fForceRJ1);
	}

}

void CLaberynthProcess::SetForceRJ2	(float force)
{
	m_sGameSettings.m_fForceRJ2 = force;
	m_BoardInputForSettings->SetForceRJ2(m_sGameSettings.m_fForceRJ2);
	if (m_pGameLogicLaberynth)
	{
		m_pGameLogicLaberynth->SetForceRJ2(m_sGameSettings.m_fForceRJ2);
	}
}

void CLaberynthProcess::UpdateInputActions	(CInputManager* inputManager)
{
	//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	if( inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT) )
	{
		if (deltaMouse.x != 0)
		{
			float deltaX = deltaMouse.x * 0.01f;
			float yaw = m_pObject3D->GetYaw();
			m_pObject3D->SetYaw(yaw+deltaX);
		}
		if (deltaMouse.y != 0)
		{
			float deltaY = deltaMouse.y * 0.01f;
			float pitch = m_pObject3D->GetPitch();
			m_pObject3D->SetPitch(pitch+deltaY);
		}
	}

	if (deltaMouse.z != 0)
	{
		float deltaZ = deltaMouse.z * 0.1f;
		float increment = 1.f;
		if (inputManager->IsDown(IDV_KEYBOARD, KEY_RCTRL) || inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL))
		{
			increment = 0.1f;
		}
		static_cast<CThPSCamera*>(m_pCamera)->AddZoom(-deltaZ*increment);
	}
	//- Si se presiona el boton del medio y se mueve el mouse, la camara se desplaza por el plano (X,Z) segun su yaw
	if( inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_MIDDLE) )
	{
		if (deltaMouse.y != 0)
		{
			//Segun su yaw directamente
			float delatY = deltaMouse.y * 0.1f;
			float yaw = m_pObject3D->GetYaw();
			Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
			m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*delatY);
		}
		if (deltaMouse.x != 0)
		{
			//Perpendicularmente a su yaw. Realizamos un strafe
			float deltaX = deltaMouse.x * 0.1f;
			float yaw = m_pObject3D->GetYaw()+ePI2f;
			Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
			//nos desplazamos a una velocidad de 1unidad x segundo
			m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*deltaX);
		}
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
}


//-----------------ScriptManager------------------------------
void CLaberynthProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getGameLaberynth", GetGameLaberynth),
		
		// registramos la clase CLaberynthProcess
		class_<CLaberynthProcess>(CScriptRegister::SetClassName("CLaberynthProcess"))
		

		// registramos su constructor
		.def(constructor<const std::string&>())

		// registramos sus funciones
		.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

		.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
					"Muestra todas las funciones de esta clase"),
					&CScriptRegister::Help)

		.def(	CScriptRegister::PushFunctionName("start", "bool", "bool show", 
					"Inicia la ejecución del nivel"),									
					&CLaberynthProcess::Start)

		.def(	CScriptRegister::PushFunctionName("endGame", "bool", "void", 
					"Finaliza la ejecución del nivel actual"),										
					&CLaberynthProcess::End)

		.def(	CScriptRegister::PushFunctionName("getUsingComputerVision", "bool", "void", 
					"Obtiene si actualmente esta activo o no la utilización de VC"),	
					&CLaberynthProcess::GetUsingComputerVision)

		.def(	CScriptRegister::PushFunctionName("getTimeOutEndCondition", "bool", "void", 
					"Obtiene si esta seteado o no la finalizacon por tiempo"),	
					&CLaberynthProcess::GetTimeOutEndCondition)

		.def(	CScriptRegister::PushFunctionName("getLevel", "std::string", "void", 
					"Obtiene el identificador del nivel que esta puesto como actual"),							
					&CLaberynthProcess::GetLevel)

		.def(	CScriptRegister::PushFunctionName("setUsingComputerVision", "void", "bool flag", 
					"Setea el uso o no de la vision por computador"),	
					&CLaberynthProcess::SetUsingComputerVision)

		.def(	CScriptRegister::PushFunctionName("setTimeOutEndCondition", "void", "bool flag", 
					"Setea el uso o no de la finalización de nivel por tiempo"),
					&CLaberynthProcess::SetTimeOutEndCondition)

		.def(	CScriptRegister::PushFunctionName("setLevel", "void", "const std::string& level", 
					"Setea el nivel a jugar en la proximo partida"),								
					&CLaberynthProcess::SetLevel)

		.def(	CScriptRegister::PushFunctionName("setForceRJ1", "void", "float", 
					"Setea la fuerza del motor 1"),					
					&CLaberynthProcess::SetForceRJ1)

		.def(	CScriptRegister::PushFunctionName("setForceRJ2", "void", "float",		
					"Setea la fuerza del motor 2"),					
					&CLaberynthProcess::SetForceRJ2)

		.def(	CScriptRegister::PushFunctionName("getForceRJ1", "float", "void", 
					"Obtiene la fuerza del motor 1"),
					&CLaberynthProcess::GetForceRJ1)

		.def(	CScriptRegister::PushFunctionName("getForceRJ2", "float", "void", 
					"Obtiene la fuerza del motor 2"),
					&CLaberynthProcess::GetForceRJ2)	
		
		.def(	CScriptRegister::PushFunctionName("setInBoardSettings", "void", "bool", 
					"Setea si se encuentra en modo settings de board"),
					&CLaberynthProcess::SetInBoardSettings)	
					
    ];
}