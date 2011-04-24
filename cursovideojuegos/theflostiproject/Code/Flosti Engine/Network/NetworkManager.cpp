#include "__PCH_Network.h"

#include "NetworkManager.h"
#include "NetworkObserver.h"
#include "NetworkMessage.h"
#include "Logger/Logger.h"
#include "Exceptions/Exception.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"

//----------------------------------------
// -- Initialize
//----------------------------------------
bool CNetworkManager::Init (ENetworkMode _eMode, int _iPort, const char* _pszIP, UINT _uMaxPkgSize, UINT _uMaxClients)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (init) Initializing Network.");

	m_bIsOk      = m_hWndMain != NULL;

	if (m_bIsOk)
	{
		m_uMaxSize          = _uMaxPkgSize;
		m_SendDataBuffer    = new char[m_uMaxSize + g_uMessageSize];
		m_ReceiveDataBuffer = new char[m_uMaxSize + g_uMessageSize];
		m_iPort             = _iPort;
		m_Mode              = _eMode;
		m_ClMax             = _uMaxClients;

		if (_pszIP) sprintf_s(m_pIP, "%s", _pszIP);

		WORD wVersion;
		wVersion = MAKEWORD(2,0);

		WSADATA wsaData;
		UINT    nEvents = 0;
		int     nRes;

		// startup WinSock
		if ((nRes = WSAStartup(wVersion, &wsaData)) !=0 )
		{
			if (nRes == WSAEINPROGRESS)
				LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: Blocking WinSock app is running!.");
			else if (nRes == WSAVERNOTSUPPORTED)
				LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: WinSock version not supported!.");
			else if (nRes == WSASYSNOTREADY)
				LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: Network subsystem not ready!.");
			else if (nRes == WSAEPROCLIM)
				LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: WinSock too busy for another task!.");
			m_bIsOk = false;
		}
	}

	if (m_bIsOk)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (init) WinSock 2.0 initialized");

		// create SockectObject as server listening to port
		if (m_Mode == ENM_SERVER)
		{ 
			if (!CreateServer(&m_pSockObj))
			{
				LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: CreateServer failed!.");
				m_bIsOk = false;
			}
		}
		// create SocketObject as client sending data to IP::Port
		else if (m_Mode == ENM_CLIENT)
		{
			if (strcmp(m_pIP, "") == 0)
				sprintf_s(m_pIP, "LOCALHOST");
			if (!CreateClient(&m_pSockObj))
			{
				LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: CreateClient failed!.");
				m_bIsOk = false;
			}
		}
		else
			m_bIsOk = false;
	}

	if (!m_bIsOk)
	{
		Release();
		std::string msg_error = "CNetworkManager::Init-> Error en la inicializacion del networkManager";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (init) Initialized (online and ready).");

		// We initialize the messages buffer
		m_Message.pData = m_ReceiveDataBuffer;
	}

	return m_bIsOk;
}

//----------------------------------------
// -- Finalize
//----------------------------------------
void CNetworkManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------
// -- Free memory
//----------------------------------------
void CNetworkManager::Release ()
{
	if (m_Mode == ENM_SERVER)
	{
		for (int i = 0; i < m_ClCount; ++i)
		{
			shutdown(m_Clients[i].skToClient, SD_BOTH);
			closesocket(m_Clients[i].skToClient);
			m_Clients[i].skToClient = INVALID_SOCKET;
		}
	}

	m_Observers.clear();

	if (m_pSockObj)
	{
		delete m_pSockObj;
		m_pSockObj = NULL;
	}

	if (m_SendDataBuffer)
	{
		delete [] m_SendDataBuffer;
		m_SendDataBuffer = NULL;
	}

	if (m_ReceiveDataBuffer)
	{
		delete [] m_ReceiveDataBuffer;
		m_ReceiveDataBuffer = NULL;
	}

	WSACleanup();

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (init) Offline (ok)");
}

//----------------------------------------
// -- RegisterObserver
//----------------------------------------
void CNetworkManager::RegisterObserver (INetworkObserver* _pObs)
{
	tObserversVector::const_iterator It;
	const tObserversVector::const_iterator ItEnd(m_Observers.end());
	for (It = m_Observers.begin(); It != ItEnd; ++It)
	{
		if ((*It) == _pObs)
			return;
	}

	m_Observers.push_back(_pObs);
}

//----------------------------------------
// -- UnregisterObserver
//----------------------------------------
void CNetworkManager::UnregisterObserver (INetworkObserver* _pObs)
{
	tObserversVector::iterator It;
	const tObserversVector::const_iterator ItEnd(m_Observers.end());
	for (It = m_Observers.begin(); It != ItEnd; ++It)
	{
		if ((*It) == _pObs)
		{
			m_Observers.erase(It);
			break;
		}
	}
}

//----------------------------------------
// -- CreateClient
//----------------------------------------
bool CNetworkManager::CreateClient (CSocketObject **ppSkObject)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateClient) Starting a new client...");
	(*ppSkObject) = new CSocketObject;

	if (!(*ppSkObject))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: (CreateClient) Unable to construct the socket!");
		return false;
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateClient) Socket constructed.");

	// 1. step: create a socket object
	if (!(*ppSkObject)->CreateSocket())
		return false;

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateClient) (CreateClient) Socket created.");


	if (m_pIP == NULL)
	{
		gethostname(m_pIP, 10);
	}

	// 2. step: try to connect to the server
	if (!(*ppSkObject)->Connect(m_pIP, m_iPort))
		return false;

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateClient) Socket connected to server.");


	UINT uEvents = 0;
	uEvents |= FD_READ | FD_CLOSE;

	// 3. step: notification from windows WM_ about incoming events
	if (WSAAsyncSelect( (*ppSkObject)->GetSocket(), m_hWndMain, WM_CLIENT, uEvents) == SOCKET_ERROR)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: (CreateClient) WSAAsyncSelect(WM_ZFXCLIENT) failed!");
		m_pSockObj->Disconnect();
		return false;
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateClient) Socket connection done.");

	return true;
}

//----------------------------------------
// -- CreateServer
//----------------------------------------
bool CNetworkManager::CreateServer (CSocketObject **ppSkObject)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateServer) Starting a new server...");

	(*ppSkObject) = new CSocketObject;

	if (!(*ppSkObject))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: (CreateServer) Unable to construct the socket!.");
		return false;
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateServer) Socket constructed.");


	// 1. step: create a socket object
	if (!(*ppSkObject)->CreateSocket())
		return false;

	//GUIManager::GetInstance()->SetNewLineInLogger("[CNetworkManager] (CreateServer) Socket created.");

	// 2. step: name the socket cause it's a server
	if (!(*ppSkObject)->Bind(m_iPort))
		return false;

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateServer) Socket binded.");


	// 3. step: listen for incoming connection requests
	if (!(*ppSkObject)->Listen())
		return false;

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateServer) Socket listening.");

	UINT uEvents = 0;
	uEvents |= FD_READ | FD_WRITE | FD_CONNECT | FD_ACCEPT | FD_CLOSE;

	// 4. step: notification from windows WM_ about incoming events
	if (WSAAsyncSelect((*ppSkObject)->GetSocket(), m_hWndMain, WM_SERVER, uEvents) == SOCKET_ERROR)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: (CreateServer) WSAAsyncSelect(WM_ZFXSERVER) failed!");
		m_pSockObj->Disconnect();
		return false;
	}

	// initialize all client slots as invalid
	for (int i = 0; i < MAX_CLIENT_SLOTS; ++i)
	{
		m_Clients[i].skToClient = INVALID_SOCKET;
		m_Clients[i].uID = 0;
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateServer) Server creation done.");
	return true;
}

//----------------------------------------
// -- SendToClients
//----------------------------------------
bool CNetworkManager::SendToClients (const CNetworkMessage* pPkg_)
{
	bool bSentOk = true;
	int iBytes   = 0;
	UINT uSize   = g_uMessageSize + pPkg_->uLength;

	// security checks
	if (m_Mode != ENM_SERVER) return false;
	if (uSize > m_uMaxSize) return false;

	// serialize data in order to send from one mem segment
	memcpy(m_SendDataBuffer, pPkg_, g_uMessageSize);
	memcpy(m_SendDataBuffer + g_uMessageSize, pPkg_->pData, pPkg_->uLength);

	// now go send it actually
	for (UINT i = 0; i < (UINT)m_ClCount; ++i)
	{
		if (m_Clients[i].skToClient != INVALID_SOCKET)
		{
			// We only send to the players except the sender
			if (m_Clients[i].uID != pPkg_->uSender)
			{
				iBytes = m_pSockObj->Send(m_SendDataBuffer, uSize, m_Clients[i].skToClient);

				// if failed report but go on with next clients
				if ( (iBytes == SOCKET_ERROR) || (iBytes < (int)uSize) )
					bSentOk = false;
			}
		}
	}
	return bSentOk;
}

//----------------------------------------
// -- SendToClient
//----------------------------------------
bool CNetworkManager::SendToClient (const CNetworkMessage* pPkg_, UINT _uClientID)
{
	bool bSentOk = true;
	int iBytes = 0;
	UINT uSize = g_uMessageSize + pPkg_->uLength;

	// security checks
	if (m_Mode != ENM_SERVER) return false;
	if (uSize > m_uMaxSize) return false;

	// serialize data in order to send from one mem segment
	memcpy(m_SendDataBuffer, pPkg_, g_uMessageSize);
	memcpy(m_SendDataBuffer + g_uMessageSize, pPkg_->pData, pPkg_->uLength);

	// now go send it actually
	for (UINT i = 0; i < (UINT)m_ClCount; ++i)
	{
		if ( (m_Clients[i].skToClient != INVALID_SOCKET) && (m_Clients[i].uID == _uClientID) )
		{
			iBytes = m_pSockObj->Send(m_SendDataBuffer, uSize, m_Clients[i].skToClient);

			// if failed report but go on with next clients
			if ( (iBytes == SOCKET_ERROR) || (iBytes < (int)uSize) )
				bSentOk = false;
		}
	}

	return bSentOk;
}

//----------------------------------------
// -- SendToServer
//----------------------------------------
bool CNetworkManager::SendToServer (const CNetworkMessage* _pPkg)
{
	int iBytes = 0;
	UINT uSize = g_uMessageSize + _pPkg->uLength;

	// security checks
	if (uSize > m_uMaxSize) return false;

	// serialize data in order to send from one mem segment
	memcpy(m_SendDataBuffer, _pPkg, g_uMessageSize);
	memcpy(m_SendDataBuffer + g_uMessageSize, _pPkg->pData, _pPkg->uLength);

	// if pkg was sent by server then 
	// feed it directly into its own queue
	if (m_Mode == ENM_SERVER)
	{
		m_pSockObj->FeedByHand( (CNetworkMessage*)m_SendDataBuffer );
		return true;
	}

	// now go send it actually
	iBytes = m_pSockObj->Send(m_SendDataBuffer, uSize);

	if ( (iBytes == SOCKET_ERROR) || (iBytes < (int)uSize) )
	{
		LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: (CreateServer) SendToServer() failed!");
		return false;
	}

	return true;
}

//----------------------------------------
// -- Send
//----------------------------------------
bool CNetworkManager::Send (const CNetworkMessage* _pPkg)
{
	bool bSent = false;

	if (_pPkg)
	{
		if (m_Mode == ENM_SERVER)
		{
			bSent = SendToClients(_pPkg);
		}
		else if (m_Mode == ENM_CLIENT)
		{
			bSent = SendToServer(_pPkg);
		}
	}

	return bSent;
}

//----------------------------------------
// -- MsgProc
//----------------------------------------
bool CNetworkManager::MsgProc (WPARAM wp, LPARAM lp)
{
	WORD  wEvent, wError;

	// get data
	wError = HIWORD(lp);
	wEvent = LOWORD(lp);

	// which event
	switch (wEvent)
	{
		// confirmation after successful connection
	case FD_CONNECT: break;

		// client wants to be accepted
	case FD_ACCEPT:  { return OnAccept(); } break;

		// we are receiving something
	case FD_READ:    { return OnReceive(wp); } break;

		// a client wants to go offline
	case FD_CLOSE:   { return OnDisconnect(wp); } break;

		// we sent something?
	case FD_WRITE:   break;

	}
	return true;
}

//----------------------------------------
// -- OnAccept
//----------------------------------------
bool CNetworkManager::OnAccept ()
{
	//int iSize  = 0;
	//int iBytes = 0;
	int i      = m_ClCount;

	// maximum of 255 clients at all
	if (m_ClCount >= MAX_CLIENT_SLOTS) return false;

	// application defined maximum number
	if ( (m_ClMax > 0) && ((UINT)m_ClCount >= m_ClMax) ) 
		return false;

	if ( !m_pSockObj->Accept(&(m_Clients[i].skToClient)) )
		return false;

	// add counters
	m_Clients[i].uID = m_ClID;

	++m_ClCount;
	++m_ClID;

	tObserversVector::const_iterator It;
	const tObserversVector::const_iterator ItEnd(m_Observers.end());
	for (It = m_Observers.begin(); It != ItEnd; ++It)
	{
		(*It)->OnNetworkAccept(m_Clients[i].uID);
	}

	return true;
}

//----------------------------------------
// -- OnReceive
//----------------------------------------
bool CNetworkManager::OnReceive (SOCKET _skReceiving)
{
	if (IsOk())
	{
		return m_pSockObj->Receive(_skReceiving);
	}

	return false;
}

//----------------------------------------
// -- OnDisconnect
//----------------------------------------
bool CNetworkManager::OnDisconnect (SOCKET _skDisconnecting)
{
	UCHAR i = 0;

	if (_skDisconnecting == INVALID_SOCKET)
		return false;

	if (m_Mode == ENM_SERVER)
	{
		// delete from the list of active clients
		for (i = 0; i < m_ClCount; ++i)
		{
			if (m_Clients[i].skToClient == _skDisconnecting)
				break;
		}

		if (i >= m_ClCount)
		{
			LOGGER->AddNewLog(ELL_ERROR, "CNetworkManager:: (CreateServer) Not listed client wants to be disconnected!");
			return false;
		}

		// shutdown and close socket
		if ( shutdown(m_Clients[i].skToClient, SD_BOTH) == SOCKET_ERROR )
			m_pSockObj->ProcessWSAError("shutdown() in CNetworkManager::OnDisconnect");
		if ( closesocket(m_Clients[i].skToClient) == SOCKET_ERROR )
			m_pSockObj->ProcessWSAError("closesocket() in CNetworkManager::OnDisconnect");

		m_Clients[i].skToClient = INVALID_SOCKET;

		tObserversVector::const_iterator It;
		const tObserversVector::const_iterator ItEnd(m_Observers.end());
		for (It = m_Observers.begin(); It != ItEnd; ++It)
		{
			(*It)->OnNetworkDisconnect(m_Clients[i].uID);
		}

		// copy last struct to deleted field, adjust counter
		memcpy(&m_Clients[i], &m_Clients[m_ClCount-1], sizeof(SClientInfo));
		--m_ClCount;
	}
	else
	{
		if ( shutdown(m_pSockObj->GetSocket(),SD_BOTH) == SOCKET_ERROR )
			m_pSockObj->ProcessWSAError("shutdown() in CNetworkManager::OnDisconnect");
		if ( closesocket(m_pSockObj->GetSocket()) == SOCKET_ERROR )
			m_pSockObj->ProcessWSAError("closesocket() in CNetworkManager::OnDisconnect");
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CNetworkManager:: (CreateServer) Client disconnected.");
	return true;
}

//----------------------------------------
// -- Update
//----------------------------------------
void CNetworkManager::Update ()
{
	while (IsOk() && IsPkgWaiting())
	{
		if (!GetNextPkg(&m_Message))
		{
			printf("GetNextPkg() failed\n");
		}
		else
		{
			tObserversVector::const_iterator It;
			const tObserversVector::const_iterator ItEnd(m_Observers.end());
			for (It = m_Observers.begin(); It != ItEnd; ++It)
			{
				(*It)->OnNetworkReceive(&m_Message);
			}

			// We reset the old vales just in case
			m_Message.uLength = 0;
			m_Message.uSender = 0;
			m_Message.uType   = 0;
		}
	}
}




//----------------------------------------------------------------------------
// LUA BIND
//----------------------------------------------------------------------------
void CNetworkManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;

	// ahora registramos lo que querramos
	module(l_pLUAState)
		[
			// registramos la clase CNetworkManager
			class_<CNetworkManager>(CScriptRegister::SetClassName("CNetworkManager"))
			
			// registramos su constructor
			.def(constructor<>())

			//-----------GENERAL FUNCTIONS---------------------
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"),
						&CScriptRegister::Help)
		];
}