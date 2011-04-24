//----------------------------------------------------------------------------------
// INetworkObserver interface
// Author: Gabriel Cirera
//
// Description:
// A class registered as a Network Observer will be able to 
// receive callbacks from network
//----------------------------------------------------------------------------------
#ifndef __NETWORK_OBSERVER_H__
#define __NETWORK_OBSERVER_H__

class CNetworkMessage;

//--------------------------------------------------------------
// -- Network Observer class
//--------------------------------------------------------------
class INetworkObserver
{
public:
	virtual void   OnNetworkAccept         (uint32 uIDAccepted)         = 0;
	virtual void   OnNetworkReceive        (CNetworkMessage* _pMessage) = 0;
	virtual void   OnNetworkDisconnect     (uint32 uIDDisconnected)     = 0;
};


#endif //__NETWORK_OBSERVER_H__