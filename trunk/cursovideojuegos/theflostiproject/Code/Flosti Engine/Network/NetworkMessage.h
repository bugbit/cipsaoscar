//----------------------------------------------------------------------------------
// CNetworkMessage class
// Author: Gabriel Cirera
//
// Description:
// The Network Message will contain the information that will travel
// from client to server and viceversa. The Types of messages should 
// be defined in some place on the GameLogic of the game.
//
// NOTE: The value 0 is used for an invalid message ID so be careful 
// not to overwrite this value with a valid one.
//----------------------------------------------------------------------------------
#ifndef __NETWORK_MESSAGE_H__
#define __NETWORK_MESSAGE_H__

#include <WinSock2.h>

//---Forward declarations---
class CNetworkManager;
class CSocketObject;
//--------------------------

// --------------------------------------------
// -- Basic package structure
// --------------------------------------------
class CNetworkMessage
{
	friend CNetworkManager;
	friend CSocketObject;

public:
	CNetworkMessage() : uType(0), uSender(0), uLength(0), uReadBytes(0), pData(NULL), pShiftData(pData), bOwnMemory(false) {}

	CNetworkMessage(UCHAR type, UINT sender, int buff_size) : 
	uType(type), uSender(sender), uLength(0), uReadBytes(0)
	{
		bOwnMemory = true;
		pData = new char[buff_size];
		pShiftData = pData;
	}

	CNetworkMessage(UCHAR type, UINT sender, char* buffer) : 
	uType(type), uSender(sender), uLength(0), uReadBytes(0), pData(buffer), pShiftData(buffer), bOwnMemory(false)
	{}

	~CNetworkMessage(){ Release(); }

	void StartReadingData() { pShiftData = pData; }

	const UCHAR     GetType            () const { return uType; }

	template <class T> int WriteData (T& data);
	template <class T> int ReadData  (T& data);

private:
	void            Release            ();
	const void*     GetData            () const { return pData; }
	void            SetData            (char* data) { bOwnMemory = false; pData = data; }

	// class members
	bool            bOwnMemory;        // This will be used to delete the memory or not at the end of the classs usage
	UINT            uReadBytes;        // Bytes parsed with the Read functions (every time we perform a Read from the pData we shift this value to read the next values)
	UINT            uLength;           // Length of data in bytes
	UCHAR           uType;             // Use for type (0 is ID notification)
	UINT            uSender;           // Sender ID (Server is 0)
	char*           pData;             // Message Data
	char*           pShiftData;        // Pointer to the current position in data
};


// --------------------------------------------
// -- Template functions
// --------------------------------------------

//------------------------------------
// Writes data to the buffer
// @in: data to be written
// @out: bytes written or -1 if wrong
//------------------------------------
template <class T>
int CNetworkMessage::WriteData (T& data)
{
	assert(pData);

	if ( pData && (uLength + sizeof(T)) < CORE->GetNetworkManager()->GetBuffersSize() )
	{
		memcpy(pShiftData, &data, sizeof(T));
		uLength += sizeof(T);
		pShiftData += uLength;

		return sizeof(T);
	}

	return -1;
}

//------------------------------------
// Reads data from the buffer
// @out: data to be read from buffer
// @out: bytes read or -1 if wrong
//------------------------------------
template <class T>
int CNetworkMessage::ReadData (T& uData) 
{ 
	assert(pData); 

	if (pData)
	{
		// Just in case someone does not call the StartReadingData function
		if (!pShiftData) pShiftData = pData;

		uData = *((T*)pShiftData);
		pShiftData += sizeof(T);

		return sizeof(T);
	}

	return -1;
}

// --------------------------------------------
// -- Basic packet message size
// --------------------------------------------
static UINT g_uMessageSize = sizeof(CNetworkMessage);


#endif