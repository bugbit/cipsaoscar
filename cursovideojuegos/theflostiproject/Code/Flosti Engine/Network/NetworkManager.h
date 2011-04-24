//----------------------------------------------------------------------------------
// CNetworkManager class
// Author: Gabriel Cirera
//
// Description:
// This class will implement the server/client side
//----------------------------------------------------------------------------------
#ifndef __NETWORK_H__
#define __NETWORK_H__

// Includes
#include "Script/ScriptRegister.h"
#include "Sockets/SocketObject.h"
#include "NetworkDefs.h"
#include "NetworkMessage.h"
#include <vector>


//---Forward declarations---
class CScriptManager;
enum ENetworkMode;
class INetworkObserver;
//--------------------------

//--------------------------------------------------------------
// -- Network class
//--------------------------------------------------------------
class CNetworkManager:public CScriptRegister
{
public:
	CNetworkManager() :	m_bIsOk(false), m_pSockObj(NULL), m_SendDataBuffer(NULL), m_ReceiveDataBuffer(NULL), m_hWndMain(NULL),
											m_iPort(0), m_ClID(1), m_ClCount(0), m_ClMax(0), m_Mode(ENM_INVALID) {}
	~CNetworkManager() { Done(); }

	// initialize the engine stuff
	bool            Init                 (ENetworkMode _eMode, int _iPort, const char* _pszIP, UINT _uMaxPkgSize, UINT _uMaxClients);
	void            Done                 ();
	bool            IsOk                 () const { return m_bIsOk; }
	void            SetWindow            (HWND _hWnd) { m_hWndMain = _hWnd; }
	void            Update               ();

	// Interface functions
	bool            IsServer             () const { return m_Mode == ENM_SERVER; }
	bool            IsRunning            () { return m_pSockObj && m_pSockObj->IsRunning(); }
	bool            MsgProc              (WPARAM wp, LPARAM lp);
	bool            Send                 (const CNetworkMessage* _pPkg);
	bool            SendToServer         (const CNetworkMessage* _pPkg);
	bool            SendToClients        (const CNetworkMessage* _pPkg);
	bool            SendToClient         (const CNetworkMessage* _pPkg, UINT _uClientID);
	bool            IsPkgWaiting         () { return m_pSockObj->IsPkgWaiting();   }
	UINT            GetNextPkgSize       () { return m_pSockObj->GetNextPkgSize(); }
	bool            GetNextPkg           (CNetworkMessage* pPkg_) { return m_pSockObj->GetNextPkg(pPkg_); }

	void            RegisterObserver     (INetworkObserver* _pObs);
	void            UnregisterObserver   (INetworkObserver* _pObs);

	UINT            GetBuffersSize       () { return (m_uMaxSize + g_uMessageSize); }

	//----CScriptRegister interface-------------------
	virtual void	RegisterFunctions	(CScriptManager* scriptManager);

private:
	//Types
	typedef std::vector<INetworkObserver*> tObserversVector;

	// Members
	void            Release              ();

	// Initialize socketobjects
	bool            CreateServer         (CSocketObject **ppSkObject);
	bool            CreateClient         (CSocketObject **ppSkObject);

	// During message processing
	bool            OnAccept             (void);
	bool            OnReceive            (SOCKET skReceiving);
	bool            OnDisconnect         (SOCKET skDisconnecting);

	// Variables
	bool            m_bIsOk;
	CSocketObject*  m_pSockObj;
	ENetworkMode    m_Mode;
	SClientInfo     m_Clients[MAX_CLIENT_SLOTS];
	char            m_ClCount;  // 1 byte -> 255
	UINT            m_ClID;
	UINT            m_ClMax;

	char*           m_SendDataBuffer;
	char*           m_ReceiveDataBuffer;

	CNetworkMessage m_Message;

	tObserversVector m_Observers;

	// From interface
	HWND            m_hWndMain; // application main window
	int             m_iPort;    // port for communication
	char            m_pIP[256]; // IP address (valid for clients)
	UINT            m_uMaxSize; // max package size for send/receive
};


#endif