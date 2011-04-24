#include "__PCH_Network.h"  
#include "NetworkMessage.h"


//--------------------------------------
// We free the memory
//--------------------------------------
void CNetworkMessage::Release()
{
	uLength     = 0;
	uType       = 0;
	uSender     = 0;
	uReadBytes  = 0;

	if (pData)
	{
		if (bOwnMemory)
			delete [] pData;

		pData = NULL;
		pShiftData = NULL;
	}
}