#include "__PCH_Sound.h"


#include "SoundManager.h"
#include "alc.h"
#include "alut.h"
#include "Base/Logger/Logger.h"
#include "xml/XMLTreeNode.h"
#include "Script/ScriptManager.h"
#include "Exceptions/Exception.h"
#include "luabind/luabind.hpp"
#include "Core/Core.h"

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CSoundManager::Done ()
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
void CSoundManager::Release () 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: shutting down GUI");
	
	_finalizeAL();

	LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: offline (ok)");
}

//----------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------
bool CSoundManager::Init ()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: calling initialization");
	m_bIsOk = _initAL();

	if (!m_bIsOk)
	{
		std::string msg_error = "CSoundManager::Init-> Error en la inicializacion de OpenAL";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: online (ok)");
	}

	return m_bIsOk;
}



//-----------GENERAL FUNCTIONS---------------------
void CSoundManager::SetGain (float gain)
{
	alListenerf(AL_GAIN,gain);
}

float CSoundManager::GetGain()
{
	float gain;
	alGetListenerf(AL_GAIN,&gain);
	return gain;
}


void CSoundManager::Reset()
{
	Stop();
	_clear();
}

void CSoundManager::Pause()
{
	ALint state;
	std::vector<tInfoSource>::iterator it;
	m_bPause = !m_bPause;

	for( it = m_Sources.begin(); it != m_Sources.end(); ++it )
	{
		alGetSourcei( (*it).m_uSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING && m_bPause)
		{
			alSourcePause((*it).m_uSource);
		}
		else if (state == AL_PAUSED && !m_bPause)
		{
			alSourcePlay((*it).m_uSource);
		}
	}
}

void CSoundManager::Stop()
{
	ALint state;
	std::vector<tInfoSource>::iterator it;

	for( it = m_Sources.begin(); it != m_Sources.end(); ++it )
	{
		alGetSourcei((*it).m_uSource,AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING || state == AL_PAUSED)
		{
			alSourceStop((*it).m_uSource);
		}
	}
}

void CSoundManager::Update (float delatTime)
{
	std::vector<uint32> l_finished;
	std::map<uint32,CLerpAnimator1D>::iterator it			= m_SourcesFadeInOut.begin();
	std::map<uint32,CLerpAnimator1D>::iterator itEnd	= m_SourcesFadeInOut.end();
	for(; it != itEnd; ++it)
	{
		float value = 0.f;
		if (it->second.Update(delatTime,value) )
		{
			//ha acabado de realizar el FadeIn/Out--> sacar del map
			l_finished.push_back(it->first);
		}
		SetSourceGain(it->first,value);
	}
	
	for(uint32 cont = 0; cont < l_finished.size(); cont++)
	{
		std::map<uint32,CLerpAnimator1D>::iterator it = m_SourcesFadeInOut.find(l_finished[cont]);
		if (it != m_SourcesFadeInOut.end())
		{
			m_SourcesFadeInOut.erase(	it );
		}
	}
	
}

bool CSoundManager::LoadSounds( const std::string &xmlGuiFile)
{
	//Read the xml gui file
	LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: Iniciando el parseo del fichero %s", xmlGuiFile.c_str());
	bool isOK = false;
	std::map<tSoundFile,tIdBuffer > filesLoaded;

	CXMLTreeNode newFile;
	if (!newFile.LoadFile(xmlGuiFile.c_str()))
	{
		std::string msg_error = "CSoundManager::LoadSounds-> Error al leer el archivo con los sonidos a cargar: " + xmlGuiFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		CXMLTreeNode  sounds = newFile["Sounds"];
		if (sounds.Exists())
		{
			int count = sounds.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				CXMLTreeNode pNewNode = sounds(i);

				//Para cada sonido leemos su informacion comun-->
				std::string soundId	= pNewNode.GetPszProperty("soundId", "");
				std::string path		= pNewNode.GetPszProperty("path", "");

				if (soundId.compare("") == 0)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::LoadSounds El tag <soundId> es incorrecto");
				}
				else if (path.compare("") == 0)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::LoadSounds El tag <path> es incorrecto");
				}
				else
				{
					//Buscamos si tenemos registrado el sonido "soundId"
				std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find( soundId );
				if( it == m_Buffers.end() )
				{
					//Si aún no la hemos registrado, la añadimos:
					std::map<tSoundFile,tIdBuffer >::iterator it = filesLoaded.find( path );
					if( it != filesLoaded.end() )
					{
						//Ya hemos cargado el fichero de sonido:
						tIdBuffer idBuffer = (*it).second;
						m_Buffers.insert(std::pair<tAction, tIdBuffer>(soundId, idBuffer) );
					}
					else
					{
						tIdBuffer idBuffer;
						if( _loadSound(path, idBuffer) )
						{
							filesLoaded.insert(std::pair<tAction, tIdBuffer>(path, idBuffer) );
							m_Buffers.insert(std::pair<tAction, tIdBuffer>(soundId, idBuffer) );
							LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager::LoadSounds-> Cargado el sonido [%s,%s]", path.c_str(), soundId.c_str());
						}
						else
						{
							CORE->SetAssetError(ASSET_ERROR_SOUNDFILE);
							LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::LoadSounds Asset Error con el fichero->%s", path.c_str());
						}
					}
				}
				else
				{
					LOGGER->AddNewLog(ELL_WARNING, "CSoundManager::LoadSounds Se ha intentado añadir el soundID->%s ya registrado anteriormente", soundId.c_str());
				}
				}
				
			}
			isOK = true;
		}
		else
		{
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::LoadSounds No se ha podido leer el tag Sounds del fichero ->%s", xmlGuiFile.c_str());
			isOK = false;
		}

	}//END else de if (!newFile.LoadFile(xmlGuiFile.c_str()))


	
	if (isOK)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: Finalizado correctamente el parseo el fichero %s", xmlGuiFile.c_str());
	}

	filesLoaded.clear();
	return isOK;
}


//---------ACTION FUNCTIONS-------------------------------
bool CSoundManager::PlayAction3D (const std::string& action, const Vect3f& position)
{
	if (m_bSoundON)
	{	
		std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);
		if (it==m_Buffers.end())
		{
			std::string description = "Action "+ action+" not found";
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction3D %s", description.c_str());
			return false;
		}
		tIdBuffer idBuffer = (*it).second;
		int index;
		ALfloat ListenerPos[3] = { position.x, position.y, position.z };
		ALfloat ListenerVel[3] = { 0.0, 0.0, 0.0 };
		
		index = _getSource(false);
	
		if (index==-1)
		{
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction3D Error al obtener un source");
			return false;
		}
	
		// Get Listener position and velocity, for maxium gain (2D sound)
		alGetListenerfv (AL_VELOCITY,ListenerVel);
		
		alSourcei (m_Sources[index].m_uSource, AL_BUFFER, idBuffer);
		alSourcef (m_Sources[index].m_uSource, AL_PITCH, 1.0);
		alSourcef (m_Sources[index].m_uSource, AL_GAIN, 1.0);
		alSourcefv(m_Sources[index].m_uSource, AL_POSITION, ListenerPos);
		alSourcefv(m_Sources[index].m_uSource, AL_VELOCITY, ListenerVel);
		alSourcei (m_Sources[index].m_uSource, AL_LOOPING, AL_FALSE);
	
		alSourcePlay(m_Sources[index].m_uSource);
	}
	return true;
}

bool CSoundManager::PlayAction2D (const std::string& action)
{
	if (m_bSoundON)
	{	
		std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);
		if (it==m_Buffers.end())
		{
			std::string description = "Action " + action + " not found";
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction2D %s", description.c_str());
			return false;
		}
		tIdBuffer idBuffer = (*it).second;
		int index;
		ALfloat vector[3] = { 0.0, 0.0, 0.0 };
		
		
		index = _getSource(false);
	
		if (index==-1)
		{
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction2D Error al obtener un source");
			return false;
		}
	
		alSourcei (m_Sources[index].m_uSource, AL_BUFFER, idBuffer);
		alSourcei (m_Sources[index].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef (m_Sources[index].m_uSource, AL_PITCH, 1.0);
		alSourcef (m_Sources[index].m_uSource, AL_GAIN, 1.0);
		alSourcefv(m_Sources[index].m_uSource, AL_POSITION, vector);
		alSourcefv(m_Sources[index].m_uSource, AL_VELOCITY, vector);
		alSourcefv(m_Sources[index].m_uSource, AL_DIRECTION, vector);
		alSourcei (m_Sources[index].m_uSource, AL_LOOPING, AL_FALSE);
			alSourcePlay(m_Sources[index].m_uSource);
	}
	return true;
}


//-----SOURCE FUNCTIONS----------------------------
uint32 CSoundManager::CreateSource()
{
	return _getSource(true);
}

bool CSoundManager::DeleteSource(uint32 source)
{
	if (source < m_Sources.size())
	{
		StopSource(source);
		m_Sources[source].m_bReserved = false;
	}
	return true;
}

bool CSoundManager::PlaySource2D (uint32 source, const std::string& action, bool loop)
{
	if (m_bSoundON)
	{
		if (source < m_Sources.size() && m_Sources[source].m_bReserved)
		{
			ALboolean alloop;
	
			if (loop)
				alloop = AL_TRUE;
			else
				alloop = AL_FALSE;
			
			StopSource(source);
	
			if (m_Buffers.find(action) == m_Buffers.end())
			{
				std::string description = "Action " + action + " not found";
				LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlaySource2D %s", description.c_str());
				return false;
			}
	
			ALfloat vector[3] = { 0.0, 0.0, 0.0 };
			alSourcei (m_Sources[source].m_uSource, AL_BUFFER, m_Buffers[action]);
			alSourcei (m_Sources[source].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);
			alSourcef (m_Sources[source].m_uSource, AL_PITCH, 1.0);
			alSourcef (m_Sources[source].m_uSource, AL_GAIN, 1.0);
			alSourcefv(m_Sources[source].m_uSource, AL_POSITION, vector);
			alSourcefv(m_Sources[source].m_uSource, AL_VELOCITY, vector);
			alSourcefv(m_Sources[source].m_uSource, AL_DIRECTION, vector);
			alSourcei (m_Sources[source].m_uSource, AL_LOOPING, alloop);
			alSourcePlay(m_Sources[source].m_uSource);
	
			return true;
		}	
		else
		{
			std::string description = "Can't play source. Invalid source " + source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
			return false;
		}
	}
	return true;
}

bool CSoundManager::PlaySource3D (uint32 source, const std::string& action, bool loop)
{
	if (m_bSoundON)
	{
		if (source< m_Sources.size() && m_Sources[source].m_bReserved)
		{
			ALboolean alloop;
	
			if (loop)
				alloop = AL_TRUE;
			else
				alloop = AL_FALSE;
			
			StopSource(source);
	
			if (m_Buffers.find(action)==m_Buffers.end())
			{
				std::string description = "Action " + action + " not found";
				LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
				return false;
			}
	
			alSourcei (m_Sources[source].m_uSource, AL_BUFFER, m_Buffers[action]);
			alSourcef (m_Sources[source].m_uSource, AL_PITCH, 1.0);
			alSourcef (m_Sources[source].m_uSource, AL_GAIN, 1.0);
			alSourcei (m_Sources[source].m_uSource, AL_LOOPING, alloop);
			alSourcePlay(m_Sources[source].m_uSource);
			return true;
		}	
		else
		{
			std::string description = "Can't play source. Invalid source " + source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
			return false;
		}
	}
	return true;
}


bool CSoundManager::PauseSource (uint32 source)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		ALint state;
		
		alGetSourcei(m_Sources[source].m_uSource,AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			alSourcePause(m_Sources[source].m_uSource);
		}
		else if (state == AL_PAUSED)
		{
			alSourcePlay(m_Sources[source].m_uSource);
		}		
		return true;
	}
	else
	{
		std::string description = "Can't pause source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
}


bool CSoundManager::StopSource (uint32 source)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		ALint state;
		
		alGetSourcei(m_Sources[source].m_uSource,AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING || state== AL_PAUSED)
		{
			alSourceStop(m_Sources[source].m_uSource);
		}
		return true;
	}
	else
	{
		std::string description = "Can't stop source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
	
}


bool CSoundManager::SetSourcePosition (uint32 source, const Vect3f& inPosition)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		ALfloat pos[3];
		pos[0] = inPosition.x;
		pos[1] = inPosition.y;
		pos[2] = inPosition.z;
		alSourcefv(m_Sources[source].m_uSource, AL_POSITION, pos);
		return true;
	}
	else
	{
		std::string description = "Can't set source position. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
}

bool CSoundManager::GetSourcePosition (uint32 source, Vect3f& outPosition)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		ALfloat pos[3];
		pos[0] = outPosition.x;
		pos[1] = outPosition.y;
		pos[2] = outPosition.z;
		alGetSourcefv(m_Sources[source].m_uSource, AL_POSITION, pos);
		return true;
	}
	else
	{
		std::string description = "Can't get source position. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
}

bool CSoundManager::SetSourceVelocity (uint32 source, const Vect3f& inVelocity)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		ALfloat vel[3];
		vel[0] = inVelocity.x;
		vel[1] = inVelocity.y;
		vel[2] = inVelocity.z;
		alSourcefv(m_Sources[source].m_uSource, AL_VELOCITY, vel);
		return true;
	}
	else
	{
		std::string description = "Can't set source velocity. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
}


bool CSoundManager::GetSourceVelocity (uint32 source, Vect3f& outVelocity)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		ALfloat vel[3];
		vel[0] = outVelocity.x;
		vel[1] = outVelocity.y;
		vel[2] = outVelocity.z;
		alGetSourcefv(m_Sources[source].m_uSource, AL_VELOCITY, vel);
		return true;
	}
	else
	{
		std::string description = "Can't get source velocity. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}	
}

bool CSoundManager::SetSourceGain (uint32 source, float gain)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alSourcef(m_Sources[source].m_uSource, AL_GAIN, gain);
		return true;
	}
	else
	{
		std::string description = "Can't set source gain. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
}


bool CSoundManager::GetSourceGain (uint32 source, float& gain)
{
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alGetSourcef(m_Sources[source].m_uSource, AL_GAIN, &gain);
		return true;
	}
	else
	{
		std::string description = "Can't get source gain. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
}

void CSoundManager::SetListenerPosition (const Vect3f& inPosition)
{
	ALfloat pos[3];
	pos[0] = inPosition.x;
	pos[1] = inPosition.y;
	pos[2] = inPosition.z;
	alListenerfv(AL_POSITION, pos);
}

void CSoundManager::GetListenerPosition (Vect3f& outPosition)
{
	ALfloat pos[3];
	alGetListenerfv(AL_POSITION, pos);
	outPosition.x = pos[0];
	outPosition.y = pos[1];
	outPosition.z = pos[2];
}

void CSoundManager::SetListenerVelocity (const Vect3f& inVelocity)
{
	ALfloat vel[3];
	vel[0] = inVelocity.x;
	vel[1] = inVelocity.y;
	vel[2] = inVelocity.z;
	alListenerfv(AL_VELOCITY, vel);	
}

void CSoundManager::GetListenerVelocity (Vect3f& outVelocity)
{
	ALfloat vel[3];
	alGetListenerfv(AL_VELOCITY, vel);	
	outVelocity.x = vel[0];
	outVelocity.y = vel[1];
	outVelocity.z = vel[2];
}


void CSoundManager::SetListenerOrientation (const Vect3f& inAt, const Vect3f& inUp)
{
	float ori[6];
	ori[0] = inAt.x;
	ori[1] = inAt.y;
	ori[2] = inAt.z;
	ori[3] = inUp.x;
	ori[4] = inUp.y;
	ori[5] = inUp.z;
	alListenerfv(AL_ORIENTATION, ori);	
}

void CSoundManager::GetListenerOrientation (Vect3f& outAt, Vect3f& outUp)
{
	float ori[6];
	alGetListenerfv(AL_ORIENTATION, ori);	
	outAt.x = ori[0];
	outAt.y = ori[1];
	outAt.z	= ori[2];
	outUp.x	= ori[3];
	outUp.y	= ori[4];
	outUp.z	= ori[5];
}

bool CSoundManager::FadeInSource (uint32 source, float totalTime, float finalGain, ETypeFunction type)
{
	bool isOK = false;
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		//Primero de todo miramos que no haya ya un FadeIn/Out del source en cuestion:
		if(m_SourcesFadeInOut.find(source) == m_SourcesFadeInOut.end() )
		{
			CLerpAnimator1D animator;
			animator.SetValues(0.f,finalGain,totalTime,type);
			m_SourcesFadeInOut.insert(std::pair<uint32,CLerpAnimator1D> (source,animator) );
			isOK = true;
		}
		else
		{
			std::string description = "Ya se esta realizando un FadeIn/Out con el source" + source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
			isOK =  false;
		}
	}
	else
	{
		std::string description = "Can't do FadeIn with source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		isOK =  false;
	}
	return isOK;
}

bool CSoundManager::FadeOutSource (uint32 source, float totalTime, ETypeFunction type)
{
	bool isOK = false;
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		//Primero de todo miramos que no haya ya un FadeIn/Out del source en cuestion:
		if(m_SourcesFadeInOut.find(source) == m_SourcesFadeInOut.end() )
		{
			CLerpAnimator1D animator;
			float currentGain = 0.f;
			GetSourceGain(source, currentGain);
			animator.SetValues(currentGain,0.f,totalTime,type);
			m_SourcesFadeInOut.insert(std::pair<uint32,CLerpAnimator1D> (source,animator) );
			isOK = true;
		}
		else
		{
			std::string description = "Ya se esta realizando un FadeIn/Out con el source" + source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
			isOK =  false;
		}
	}
	else
	{
		std::string description = "Can't do FadeOut with source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		isOK =  false;
	}
	return isOK;
}


//---------------PRIVATE FUNCTIONS-------------------------------------------
bool CSoundManager::_initAL()
{
	ALenum error;
	ALCdevice* pDevice;
	ALCcontext* pContext;

	// Get handle to default device.
	pDevice = alcOpenDevice(NULL);

	// Get the device specifier.
	//const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

	// Create audio context.
	pContext = alcCreateContext(pDevice, NULL);

	// Set active context.
	alcMakeContextCurrent(pContext);

	// Check for an error.
	if ((error=alcGetError(pDevice)) != ALC_NO_ERROR)
	{
		std::string description = "Can't create openAL context (" + _getALErrorString(error) + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
	return true;
}

void CSoundManager::_finalizeAL()
{
	_clear();
	
	ALCcontext* pCurContext;
	ALCdevice* pCurDevice;

	// Get the current context.
	pCurContext = alcGetCurrentContext();

	// Get the device used by that context.
	pCurDevice = alcGetContextsDevice(pCurContext);

	// Reset the current context to NULL.
	alcMakeContextCurrent(NULL);

	// Release the context and the device.
	alcDestroyContext(pCurContext);
	alcCloseDevice(pCurDevice);
}

bool CSoundManager::_loadSound (const std::string& file, tIdBuffer& buffer)
{
	// Variables to load into.
	FILE *fd;
	ALenum format;
	ALenum error;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	
	// Load wav data into buffers.
	alGenBuffers(1, &buffer);

	if((error=alGetError()) != AL_NO_ERROR)
	{	
		alDeleteBuffers(1,&buffer);
		std::string description = "Error: Can't create openAL Buffer (" + _getALErrorString(error)  + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;	
	}

	// Check if the file exists
	if ((fd=fopen(file.c_str(),"r"))==NULL)
	{
		alDeleteBuffers(1,&buffer);
		std::string description = "Error: Can't open file " + file;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
	else
	{
		fclose(fd);
	}
	alutLoadWAVFile((ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	if ((error=alGetError()) != AL_NO_ERROR)
	{		
		alDeleteBuffers(1,&buffer);
		std::string description = "Error: Can't load sound file " + file + " (" + _getALErrorString(error)  + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}		
	return true;
}

void CSoundManager::_clear ()
{
	// Delete AL objets
	ALuint aux;	
	std::map<tAction, tIdBuffer>::iterator itBuffer;
	for( itBuffer = m_Buffers.begin(); itBuffer != m_Buffers.end(); ++itBuffer )
	{
		aux = (*itBuffer).second;
		alDeleteBuffers (1,&aux);
	}
	
	std::vector<tInfoSource>::iterator itSource;
	for( itSource = m_Sources.begin(); itSource != m_Sources.end(); ++itSource )
	{
		aux = (*itSource).m_uSource;
		alDeleteSources (1,&aux);
	}
	
	m_Buffers.clear();
	m_Sources.clear();
}

int CSoundManager::_getSource (bool reserved)
{
	// Find a unused source
	uint32 i=0;
	bool found=false;
	ALint state;
	ALenum error;
	
	for( unsigned int i=0; i < m_Sources.size(); i++)
	{
		if (!m_Sources[i].m_bReserved)
		{
			// Check if it is being used
			alGetSourcei(m_Sources[i].m_uSource,AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING && state != AL_PAUSED)
			{
				m_Sources[i].m_bReserved = reserved;
				return i;
			}			
		}
	}
		
	tInfoSource info;
	alGenSources(1,&info.m_uSource);
	if ((error=alGetError())!=AL_NO_ERROR)
	{
		std::string description = "Can't create source (" + _getALErrorString(error) + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		i=-1;
	}
	else
	{
		info.m_bReserved=reserved;
		m_Sources.push_back(info);
		i =(uint32) m_Sources.size() - 1;			
	}
	
	return i;
}

std::string CSoundManager::_getALErrorString( ALenum err)
{
    switch(err)
    {
        case AL_NO_ERROR:
            return std::string("AL_NO_ERROR");
        break;

        case AL_INVALID_NAME:
            return std::string("AL_INVALID_NAME");
						break;
        case AL_INVALID_ENUM:
            return std::string("AL_INVALID_ENUM");
		        break;
        case AL_INVALID_VALUE:
            return std::string("AL_INVALID_VALUE");
		        break;
        case AL_INVALID_OPERATION:
            return std::string("AL_INVALID_OPERATION");
		        break;
        case AL_OUT_OF_MEMORY:
            return std::string("AL_OUT_OF_MEMORY");
		        break;
				default:
						return std::string("JORLS!");
    };
}



void CSoundManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;
	
	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
      // registramos la clase CSoundManager
			class_<CSoundManager>(CScriptRegister::SetClassName("CSoundManager"))
			

      // registramos su constructor
      .def(constructor<>())

     	//-----------GENERAL FUNCTIONS---------------------
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"),
						&CScriptRegister::Help)

      .def(	CScriptRegister::PushFunctionName("loadSounds", "bool", "const std::string& xmlSoundsFile", 
						"Carga todos los sonidos a partir de un fichero .xml"),
						&CSoundManager::LoadSounds)

			.def(	CScriptRegister::PushFunctionName("reset", "void", "void",
						"Hace un reset de todos los sonidos cargados por el fichero .xml"),
						&CSoundManager::Reset)

			.def(	CScriptRegister::PushFunctionName("pause", "void", "void", 
						"Hace un pause de todos los sonidos que se estab reproduciendo actualmente"),
						&CSoundManager::Pause)

			.def(	CScriptRegister::PushFunctionName("stop", "void", "void",
						"Hace un stop de todos los sonidos que se estan reproducioendo actualmente"),										
						&CSoundManager::Stop)

			.def(	CScriptRegister::PushFunctionName("soundOn", "void", "void",
						"Activa el sistema de audio"),
						&CSoundManager::SoundOn)

			.def(	CScriptRegister::PushFunctionName("soundOff", "void", "void", 
						"Desactiva el sistema de audio"),								
						&CSoundManager::SoundOff)

			.def(	CScriptRegister::PushFunctionName("setGain", "void", "float gain", 
						"Setea el volumen general [0.f-1.f]"),								
						&CSoundManager::SetGain)

			.def(	CScriptRegister::PushFunctionName("getGain", "float", "void", 
						"Obtiene el volumen general [0.f-1.f]"),								
						&CSoundManager::GetGain)
			
			//---------ACTION FUNCTIONS-------------------------------
			.def(	CScriptRegister::PushFunctionName("playAction2D", "bool", "const std::string& action", 
						"Hace un play 2d de un audio mediante su id"),						
						&CSoundManager::PlayAction2D)

			.def(	CScriptRegister::PushFunctionName("playAction3D", "bool", "const std::string& action, const Vect3f& position",
						"Hace un play 3d de un audio mediante su id"),						
						&CSoundManager::PlayAction3D)

				//-----SOURCE FUNCTIONS----------------------------
			.def(	CScriptRegister::PushFunctionName("createSource", "uint32", "void", 
						"Crea un source y obtiene su id"),						
						&CSoundManager::CreateSource)

			.def(	CScriptRegister::PushFunctionName("deleteSource", "bool", "uint32",
						"Elimina un source mediante su id"),						
						&CSoundManager::DeleteSource)

			.def(	CScriptRegister::PushFunctionName("playSource2D", "bool", "uint32 source, const std::string& action, bool loop", 
						"Hace un play 2d de un sonido en un source determinado"),						
						&CSoundManager::PlaySource2D)

			.def(	CScriptRegister::PushFunctionName("playSource3D", "bool", "uint32 source, const std::string& action, bool loop", 
						"Hace un play 3d de un sonido en un source determinado"),						
						&CSoundManager::PlaySource3D)

			.def(	CScriptRegister::PushFunctionName("pauseSource", "bool", "uint32 source", 
						"Hace un pause de un source determinado"),						
						&CSoundManager::PauseSource)

			.def(	CScriptRegister::PushFunctionName("stopSource", "bool", "uint32 source", 
						"Hace un stop de un source determinado"),							
						&CSoundManager::StopSource)

			.def(	CScriptRegister::PushFunctionName("setSourceGain", "bool", "uint32 source, float inGain", 
						"Setea el volumen de reproducción de un source determinado"),					
						&CSoundManager::SetSourceGain)

			.def(	CScriptRegister::PushFunctionName("getSourceGain", "bool", "uint32 source, float inGain", 
						"Obtiene el volumen de reproducción de un source determinado"),					
						&CSoundManager::GetSourceGain)

			.def(	CScriptRegister::PushFunctionName("setSourcePosition", "bool", "uint32 source, const Vect3f& inPosition", 
						"Setea la posición de un source determinado"),			
						&CSoundManager::SetSourcePosition)

			.def(	CScriptRegister::PushFunctionName("getSourcePosition", "bool", "uint32 source, Vect3f& outPosition", 
						"Obtiene la posición de un source determinado"),			
						&CSoundManager::GetSourcePosition)

			.def(	CScriptRegister::PushFunctionName("setSourceVelocity", "bool", "uint32 source, const Vect3f& inVelocity", 
						"Setea la velocidad de un source determinado"),			
						&CSoundManager::SetSourceVelocity)

			.def(	CScriptRegister::PushFunctionName("getSourceVelocity" ,"bool", "uint32 source, Vect3f& outVelocity", 
						"Obtiene la velocidad de un source determinado"),			
						&CSoundManager::GetSourceVelocity)


			//-----LISTENER FUNCTIONS-------------------------
			.def(	CScriptRegister::PushFunctionName("setListenerPosition", "void", "const Vect3f& inPosition", 
						"Setea la posición del listener"),		
						&CSoundManager::SetListenerPosition)

			.def(	CScriptRegister::PushFunctionName("getListenerPosition", "void", "Vect3f& outPosition", 
						"Obtiene la posición del listener"),		
						&CSoundManager::GetListenerPosition)

			.def(	CScriptRegister::PushFunctionName("setListenerVelocity", "void", "const Vect3f& inVelocity", 
						"Setea la velocidad del listener"),		
						&CSoundManager::SetListenerVelocity)
	
			.def(	CScriptRegister::PushFunctionName("getListenerVelocity", "void", "Vect3f& outVelocity", 
						"Obtiene la velocidad del listener"),		
						&CSoundManager::GetListenerVelocity)

			.def(	CScriptRegister::PushFunctionName("setListenerOrientation", "void", "const Vect3f& inAt, const Vect3f& inUp", 
						"Setea la orientación del listener"),	
						&CSoundManager::SetListenerOrientation)

			.def(	CScriptRegister::PushFunctionName("getListenerOrientation", "void", "Vect3f& outAt, Vect3f& outUp", 
						"Obtiene la orientación del listener"),	
						&CSoundManager::GetListenerOrientation)
    ];
}