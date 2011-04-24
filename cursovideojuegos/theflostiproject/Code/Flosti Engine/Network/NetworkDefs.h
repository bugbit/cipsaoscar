//----------------------------------------------------------------------------------
// CNetwork defines
// Author: Gabriel Cirera
//
// Description:
// ...
//----------------------------------------------------------------------------------
#ifndef __NETWORK_DEFS__
#define __NETWORK_DEFS__

// ----------------------------------------
// -- Network properties
// ----------------------------------------
#define BUFFER_SIZE         65536 // 2^16
#define MAX_SIMULT_CONNECTS 32
#define MAX_CLIENT_SLOTS    256

// ----------------------------------------
// -- Windows Message Handlers
// ----------------------------------------
#define WM_SERVER (WM_USER + 1)
#define WM_CLIENT (WM_USER + 2)

// --------------------------------------------
// -- Network modes
// --------------------------------------------
enum ENetworkMode
{
	ENM_SERVER = 0,
	ENM_CLIENT = 1,
	ENM_INVALID = 2,
};

// --------------------------------------------
// -- Server connected clients list
// --------------------------------------------
struct SClientInfo
{
	SOCKET skToClient;
	UINT   uID;

	SClientInfo () : skToClient(INVALID_SOCKET), uID(0) {}
};

#endif