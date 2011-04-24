#include "__PCH_Base.h"

#include "SocketObject.h"
#include "Network/NetworkDefs.h"
#include "Network/NetworkMessage.h"
//#include "gui/GUIManager.h"


//-------------------------------
// Initialize data
//-------------------------------
bool CSocketObject::Init ()
{
	m_bIsOk = true;

	if (m_bIsOk)
	{
		// ...
	}

	if (!m_bIsOk)
	{
		Release();
	}

	return m_bIsOk;
}

//-------------------------------
// Finalize data
//-------------------------------
void CSocketObject::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//-------------------------------
// Free memory
//-------------------------------
void CSocketObject::Release ()
{
	if (IsRunning()) 
	{
		Disconnect();
		m_bRunning = false;
	}

	CHECKED_DELETE(m_pInbox);

	if (m_Buffer)
	{
		delete [] m_Buffer;
		m_Buffer = NULL;
	}

	m_skSocket = INVALID_SOCKET;
}

//-------------------------------
// CreateSocket
//-------------------------------
bool CSocketObject::CreateSocket ()
{
	// If socket is in use then close it
	if (m_skSocket != INVALID_SOCKET)
		Disconnect();

	m_skSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_skSocket == INVALID_SOCKET)
	{
		printf("error: socket() failed\n");
		return false;
	}

	m_pInbox = new CQueue();
	m_Buffer = new char[BUFFER_SIZE];
	memset(m_Buffer, 0, BUFFER_SIZE);

	return true;
}

//-------------------------------
// Bind
//-------------------------------
bool CSocketObject::Bind (int nPort)
{
	sockaddr_in saServerAddress;

	memset(&saServerAddress, 0, sizeof(sockaddr_in));
	saServerAddress.sin_family      = AF_INET;
	saServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	saServerAddress.sin_port        = htons(nPort);

	if (bind(m_skSocket, (sockaddr*)&saServerAddress, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		ProcessWSAError("Bind");
		Disconnect();
		return false;
	}

	return true;
}

//-------------------------------
// Listen
//-------------------------------
bool CSocketObject::Listen ()
{
	if (listen(m_skSocket, MAX_SIMULT_CONNECTS) != 0)
	{
		printf("error: listen() failed\n");
		return false;
	}
	m_bRunning = true;

	return true;
}

//-------------------------------
// Accept
//-------------------------------
bool CSocketObject::Accept (SOCKET* _skToNewClient)
{
	sockaddr_in saClientAddress;
	int iClientSize = sizeof(sockaddr_in);

	(*_skToNewClient) = accept(m_skSocket, (sockaddr*)&saClientAddress, &iClientSize);

	if ((*_skToNewClient) == INVALID_SOCKET)
	{
		printf("error: accept() failed\n");
		return false;
	}

	printf("Client from %d.%d.%d.%d now succesfully connected to us\n",
		saClientAddress.sin_addr.S_un.S_un_b.s_b1,
		saClientAddress.sin_addr.S_un.S_un_b.s_b2,
		saClientAddress.sin_addr.S_un.S_un_b.s_b3,
		saClientAddress.sin_addr.S_un.S_un_b.s_b4);

	return true;
}

//-------------------------------
// Connect
//-------------------------------
bool CSocketObject::Connect (char* _pszServer, int _iPort)
{
	sockaddr_in saServerAddress;
	LPHOSTENT pHost = NULL;

	// Try to find the server by address or name
	memset(&saServerAddress, 0, sizeof(sockaddr_in));
	saServerAddress.sin_port        = htons(_iPort);
	saServerAddress.sin_family      = AF_INET;
	saServerAddress.sin_addr.s_addr = inet_addr(_pszServer);

	if (saServerAddress.sin_addr.s_addr==INADDR_NONE)
	{
		pHost = gethostbyname(_pszServer);
		if (pHost != NULL)
		{
			saServerAddress.sin_addr.s_addr = ((LPIN_ADDR)pHost->h_addr)->s_addr;
		}
		else
		{
			printf("error: server \"%s\" not found\n", _pszServer);
			return false;
		}
	}

	// Connect the socket to the server address
	//if (connect(m_skSocket, (sockaddr*)&saServerAddress, sizeof(sockaddr)) == SOCKET_ERROR)
	if (WSAConnect(m_skSocket, (sockaddr*)&saServerAddress, sizeof(sockaddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		ProcessWSAError("Connect");
		Disconnect();
		return false;
	}

	// Now the SocketObject is active
	m_bRunning = true;

	return true;
}

//-------------------------------
// Disconnect
//-------------------------------
void CSocketObject::Disconnect ()
{
	if (m_skSocket != INVALID_SOCKET)
	{
		shutdown(m_skSocket, SD_BOTH); // read and write
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
		printf("socket closed\n");
	}
}

//-------------------------------
// Send
//-------------------------------
int CSocketObject::Send (const char* _pszPkg, UINT _uSize, SOCKET _skReceiver /*= INVALID_SOCKET*/)
{
	UINT uSent = 0;
	UINT n     = 0;

	SOCKET skRecv = _skReceiver != INVALID_SOCKET ? _skReceiver : m_skSocket;

	while (uSent < _uSize)
	{
		n = send(skRecv, _pszPkg + uSent, _uSize - uSent, 0);

		if (n == SOCKET_ERROR)
		{
			ProcessWSAError("Send");
			return n;
		}
		else uSent += n;
	}

	return uSent;
} 

//-------------------------------
// Receive
//-------------------------------
bool CSocketObject::Receive (SOCKET _sk)
{
	//HRESULT hr         = ZFX_OK;
	bool    bRcvOk     = true;
	UINT    uSize      = BUFFER_SIZE;   // max size to read in one call
	UINT    uBytesRead = 0;             // actual bytes read in one call
	UINT    uReadHead  = 0;             // reading position in m_Buffer
	UINT    n          = 0;             // data in buffer remaining
	bool    bDone      = false;         // ready

	CNetworkMessage* pPkg     = NULL;
	UINT        uPkgSizeTotal = 0;

	// read up to 65536 bytes each call and loop until
	// no more data is waiting at socket to be received
	while (!bDone)
	{
		uBytesRead = recv(_sk, &m_Buffer[n], uSize - n, 0);

		if (uBytesRead == SOCKET_ERROR)
		{
			int WSAError = WSAGetLastError();

			// ignore non-critical errors
			if ( (WSAError != WSAEMSGSIZE) && (WSAError != WSAEWOULDBLOCK) )
			{
				ProcessWSAError("Receive");
				bRcvOk = false;
				bDone  = true;
				break;
			}
		}

		// new got nBytesRead bytes in m_Buffer so enqueue it
		if (uBytesRead <= 0)
		{
			bDone = true;
		}
		else
		{
			// take care of old data in the buffer
			uBytesRead += n;

			// loop as long as we find another complete header
			// in our buffer, note that a package could have
			// been split so take care of that
			while ( (uBytesRead - uReadHead) >= g_uMessageSize ) 
			{
				// mask next chunk of data as ZFXPACKAGE
				pPkg = (CNetworkMessage*)&m_Buffer[uReadHead];
				pPkg->SetData(&m_Buffer[uReadHead] + g_uMessageSize);

				// how big is this package?
				uPkgSizeTotal = g_uMessageSize + pPkg->uLength;

				// do we have the whole package received
				if ( (uBytesRead - uReadHead) >= uPkgSizeTotal )
				{
					m_pInbox->Enqueue(pPkg, uPkgSizeTotal);
					uReadHead += uPkgSizeTotal;
				}
				// no just part of the package so go back to recv
				else
				{
					// copy the split package to start of the buffer
					memcpy(m_Buffer, &m_Buffer[uReadHead], uBytesRead - uReadHead);
					n = uBytesRead - uReadHead;
					break;
				}
			} // while

			// if we already got all data that was waiting
			if (uBytesRead < uSize) bDone = true;
		}
	} // while

	return bRcvOk;
} 

//-------------------------------
// GetNextPkg
//-------------------------------
bool CSocketObject::GetNextPkg(CNetworkMessage* pPkg_)
{
	// Is there a package at all?
	if (m_pInbox->GetCount() > 0)
	{
		// Stream serialized data into our buffer
		m_pInbox->Front(m_Buffer, true);

		// Fill it into the structure
		memcpy(pPkg_, m_Buffer, g_uMessageSize - sizeof(PVOID));
		memcpy(pPkg_->pData, m_Buffer + g_uMessageSize, pPkg_->uLength);

		return true;
	}
	return false;
}

//-------------------------------
// FeedByHand
//-------------------------------
void CSocketObject::FeedByHand(CNetworkMessage* _pPkg)
{
	int nPkgSizeTotal = g_uMessageSize + _pPkg->uLength;
	m_pInbox->Enqueue(_pPkg, nPkgSizeTotal);
} 


//-------------------------------
// ProcessWSAError
//-------------------------------
void CSocketObject::ProcessWSAError(const char* _pszInfo)
{
	int n = WSAGetLastError();

	char szError[1024];

	if (n == WSANOTINITIALISED)     sprintf_s(szError, "Error: %s | WSANOTINITIALISED\n", _pszInfo);
	else if (n == WSAENETDOWN)      sprintf_s(szError, "Error: %s | WSAENETDOWN\n", _pszInfo);
	else if (n == WSAEADDRINUSE)    sprintf_s(szError, "Error: %s | WSAEADDRINUSE\n", _pszInfo);
	else if (n == WSAEINTR)         sprintf_s(szError, "Error: %s | WSAEINTR\n", _pszInfo);
	else if (n == WSAEINPROGRESS)   sprintf_s(szError, "Error: %s | WSAEINPROGRESS\n", _pszInfo);
	else if (n == WSAEALREADY)      sprintf_s(szError, "Error: %s | WSAEALREADY\n", _pszInfo);
	else if (n == WSAEADDRNOTAVAIL) sprintf_s(szError, "Error: %s | WSAEADDRNOTAVAIL\n", _pszInfo);
	else if (n == WSAEAFNOSUPPORT)  sprintf_s(szError, "Error: %s | WSAEAFNOSUPPORT\n", _pszInfo);
	else if (n == WSAECONNREFUSED)  sprintf_s(szError, "Error: %s | WSAECONNREFUSED\n", _pszInfo);
	else if (n == WSAEFAULT)        sprintf_s(szError, "Error: %s | WSAEFAULT\n", _pszInfo);
	else if (n == WSAEINVAL)        sprintf_s(szError, "Error: %s | WSAEINVAL\n", _pszInfo);
	else if (n == WSAEISCONN)       sprintf_s(szError, "Error: %s | WSAEISCONN\n", _pszInfo);
	else if (n == WSAENETUNREACH)   sprintf_s(szError, "Error: %s | WSAENETUNREACH\n", _pszInfo);
	else if (n == WSAENOBUFS)       sprintf_s(szError, "Error: %s | WSAENOBUFS\n", _pszInfo);
	else if (n == WSAENOTSOCK)      sprintf_s(szError, "Error: %s | WSAENOTSOCK\n", _pszInfo);
	else if (n == WSAENOTCONN)      sprintf_s(szError, "Error: %s | WSAENOTCONN\n", _pszInfo);
	else if (n == WSAETIMEDOUT)     sprintf_s(szError, "Error: %s | WSAETIMEDOUT\n", _pszInfo);
	else if (n == WSAEWOULDBLOCK)   sprintf_s(szError, "Error: %s | WSAEWOULDBLOCK\n", _pszInfo);
	else if (n == WSAEACCES)        sprintf_s(szError, "Error: %s | WSAEACCES\n", _pszInfo);
	else if (n == WSAEMFILE)        sprintf_s(szError, "Error: %s | WSAEMFILE\n", _pszInfo);
	else if (n == WSAEOPNOTSUPP)    sprintf_s(szError, "Error: %s | WSAEOPNOTSUPP\n", _pszInfo);
	else                            sprintf_s(szError, "Error: %s | unknown error code\n", _pszInfo);

	//GUIManager::GetInstance()->SetNewLineInLogger(szError);
}