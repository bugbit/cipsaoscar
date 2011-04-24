#include "__PCH_Graphics.h"

#include "AviPlayerManager.h"
#include "AviPlayer.h"
#include "Logger/Logger.h"
#include "XML/XMLTreeNode.h"
#include "Script/ScriptManager.h"
#include "Exceptions/Exception.h"
#include "Graphics/RenderManager.h"
#include "Graphics/Textures/TextureManager.h"
#include "Core/Core.h"
#include "luabind/luabind.hpp"

bool CAviPlayerManager::Init (CRenderManager* rm)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CAviPlayerManager::Init calling initialization");
	m_bIsOk = false;
	m_pRM = rm;
	if (m_pRM != NULL)
	{
		m_bIsOk = true;
	}

	if (!m_bIsOk)
	{
		std::string msg_error = "CAviPlayerManager::Init-> Error en la inicializacion";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}

	return m_bIsOk;
}

void CAviPlayerManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CAviPlayerManager::Release	()
{
	std::map<std::string, CAviPlayer*>::iterator it			= m_AviPlayers.begin();  	
	std::map<std::string, CAviPlayer*>::iterator itEnd	= m_AviPlayers.end();  	
	for(; it != itEnd; ++it)
	{
		CAviPlayer * aviPlayer = it->second;
		CHECKED_DELETE(aviPlayer);
	}
	m_AviPlayers.clear();
}

void CAviPlayerManager::Update (float deltaTime)
{
	std::map<std::string, CAviPlayer*>::iterator it			= m_AviPlayers.begin();  	
	std::map<std::string, CAviPlayer*>::iterator itEnd	= m_AviPlayers.end();  	
	for(; it != itEnd; ++it)
	{
		CAviPlayer * aviPlayer = it->second;
		
		aviPlayer->Update(deltaTime);
	}
}

void CAviPlayerManager::UnLoadAvis ()
{
	Release();
}

bool CAviPlayerManager::LoadAvis(const std::string& xmlAviFile)
{
	//Read the xml avi file
	LOGGER->AddNewLog(ELL_INFORMATION, "CAviPlayerManager:: Iniciando el parseo del fichero %s", xmlAviFile.c_str());
	bool isOK = false;
	
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(xmlAviFile.c_str()))
	{
		std::string msg_error = "CAviPlayerManager::LoadAvis-> Error al leer el archivo con los avis a cargar: " + xmlAviFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		CXMLTreeNode  avis = newFile["Avis"];
		if (avis.Exists())
		{
			int count = avis.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				CXMLTreeNode pNewNode = avis(i);

				//Para cada avi leemos su informacion comun-->
				std::string aviId	= pNewNode.GetPszProperty("aviId", "");
				std::string path		= pNewNode.GetPszProperty("path", "");

				if (aviId.compare("") == 0)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::LoadAvis El tag <aviId> es incorrecto");
				}
				else if (path.compare("") == 0)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::LoadAvis El tag <path> es incorrecto");
				}
				else
				{
					//Buscamos si tenemos registrado el sonido "aviId"
					std::map<std::string, CAviPlayer*>::iterator it = m_AviPlayers.find( aviId );
					if( it == m_AviPlayers.end() )
					{
						//Si aún no la hemos registrado, la añadimos:
						CAviPlayer* aviPlayer = new CAviPlayer(m_pRM);
						assert(aviPlayer);
						if (aviPlayer->Load(path))
						{
							m_AviPlayers.insert(std::pair<std::string, CAviPlayer*>(aviId,aviPlayer));
							LOGGER->AddNewLog(ELL_INFORMATION, "CAviPlayerManager::LoadAvis-> Cargado el avi [%s,%s]", path.c_str(), aviId.c_str());
						}
						else
						{
							CORE->SetAssetError(ASSET_ERROR_AVIFILE);
							LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::LoadAvis Asset Error con el fichero->%s", path.c_str());
						}
						
					}
					else
					{
						LOGGER->AddNewLog(ELL_WARNING, "CAviPlayerManager::LoadAvis Se ha intentado añadir el aviID->%s ya registrado anteriormente", aviId.c_str());
					}
				}
				
			}
			isOK = true;
		}
		else
		{
			LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::LoadAvis No se ha podido leer el tag Avis del fichero ->%s", xmlAviFile.c_str());
			isOK = false;
		}

	}//END else de if (!newFile.LoadFile(xmlAviFile.c_str()))


	if (isOK)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CAviPlayerManager:: Finalizado correctamente el parseo el fichero %s", xmlAviFile.c_str());
	}

	return isOK;
}


void CAviPlayerManager::SetAviTexture (const std::string& aviName, CTexture* texture)
{
	std::map<std::string, CAviPlayer*>::iterator  it = m_AviPlayers.find(aviName);
	if (it != m_AviPlayers.end())
	{
		//Existe el avi
		CAviPlayer* aviPlayer = it->second;
		aviPlayer->SetTexture(texture);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::SetAviTexture-> No existe el video: %s", aviName.c_str());
		CORE->SetAssetError(ASSET_ERROR_AVIFILE);
		texture = CORE->GetTextureManager()->GetDefualtTexture();
	}
}

void CAviPlayerManager::PlayAvi(const std::string& aviName)
{
  std::map<std::string, CAviPlayer*>::iterator  it = m_AviPlayers.find(aviName);
	if (it != m_AviPlayers.end())
	{
		//Existe el avi
		CAviPlayer* aviPlayer = it->second;
		aviPlayer->Play();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::PlayAvi-> No existe el video: %s", aviName.c_str());
	}
}

void CAviPlayerManager::StopAvi(const std::string& aviName)
{
   std::map<std::string, CAviPlayer*>::iterator  it = m_AviPlayers.find(aviName);
	if (it != m_AviPlayers.end())
	{
		//Existe el avi
		CAviPlayer* aviPlayer = it->second;
		aviPlayer->Stop();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::StopAvi-> No existe el video: %s", aviName.c_str());
	}
}

void CAviPlayerManager::PauseAvi(const std::string& aviName)
{
   std::map<std::string, CAviPlayer*>::iterator  it = m_AviPlayers.find(aviName);
	if (it != m_AviPlayers.end())
	{
		//Existe el avi
		CAviPlayer* aviPlayer = it->second;
		aviPlayer->Pause();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CAviPlayerManager::PauseAvi-> No existe el video: %s", aviName.c_str());
	}
}


void CAviPlayerManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;
	
	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
      // registramos la clase CAviPlayerManager
			class_<CAviPlayerManager>(CScriptRegister::SetClassName("CAviPlayerManager"))
			
      // registramos su constructor
      .def(constructor<>())

     	//-----------GENERAL FUNCTIONS---------------------
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"), 
						&CScriptRegister::Help)

			.def(	CScriptRegister::PushFunctionName("loadAvis", "bool", "const std::string&"
						"Funcion para cargar de un fichero .xml todos los avis a cargar"), 
						&CAviPlayerManager::LoadAvis)
			.def(	CScriptRegister::PushFunctionName("playAvi", "void", "const std::string&", 
						"Hace Play del avi pasado como argumento y empieza dsd el princio"), 
						&CAviPlayerManager::PlayAvi)

			.def(	CScriptRegister::PushFunctionName("stopAvi", "void", "const std::string&", 
						"Hace un Stop del avi pasado como argumento y deja el avi al principio"), 
						&CAviPlayerManager::StopAvi)

			.def(	CScriptRegister::PushFunctionName("pauseAvi", "void", "const std::string&", 
						"Hace un pause del avi pasado como argumento (para volver a visionar otra vez pause"), 
						&CAviPlayerManager::PauseAvi)

			.def(	CScriptRegister::PushFunctionName("setAviTexture", "void ", "const std::string&, CTexture*", 
						"Setea una textura a un avi cuyo identificador se pasa como arg."), 
						&CAviPlayerManager::SetAviTexture)
    ];
}