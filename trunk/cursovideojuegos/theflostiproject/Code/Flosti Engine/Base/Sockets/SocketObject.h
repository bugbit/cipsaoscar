//----------------------------------------------------------------------------------
// CSocketObject class
// Author: Gabriel Cirera
//
// Description:
// This class encapsulates the Windows Sockets lib
//----------------------------------------------------------------------------------
#ifndef __SOCKET_OBJECT_H__
#define __SOCKET_OBJECT_H__

// Includes
#include <WinSock2.h>
#include "Queue.h"

// Forward declarations
class CNetworkMessage;

//--------------------------------------------------------------
// -- Socket Object Class
//--------------------------------------------------------------
class CSocketObject 
{
public:
	CSocketObject() : m_skSocket(INVALID_SOCKET), m_bRunning(false), m_pInbox(NULL), m_Buffer(NULL) {}
	~CSocketObject() { Done(); }

	bool        Init                  ();
	void        Done                  ();
	bool        IsOk                  () const { return m_bIsOk; }

	bool        IsRunning             () { return m_bRunning; }

	// Socket misc operations
	bool        CreateSocket          ();
	bool        Bind                  (int _iPort);
	bool        Listen                ();
	bool        Accept                (SOCKET* _skToNewClient);
	bool        Connect               (char* _pszServer, int _iPort);
	void        Disconnect            ();

	// Send and receive data
	int         Send                  (const char* _pszPkg, UINT _uSize, SOCKET _sk = INVALID_SOCKET);
	bool        Receive               (SOCKET _sk);

	void        FeedByHand            (CNetworkMessage* _pPkg);

	// For informational purposes
	SOCKET      GetSocket             () { return m_skSocket; }
	void        ProcessWSAError       (const char* _pszInfo);

	// Information about inbox
	bool        IsPkgWaiting          () { return m_pInbox->GetCount() > 0; }
	UINT        GetNextPkgSize        () { return m_pInbox->GetFrontSize(); }
	bool        GetNextPkg            (CNetworkMessage* pPkg_);

private:
	void        Release               ();

	// Variables
	bool        m_bIsOk;
	CQueue*     m_pInbox;
	SOCKET      m_skSocket;
	char*       m_Buffer;
	bool        m_bRunning;
};

#endif