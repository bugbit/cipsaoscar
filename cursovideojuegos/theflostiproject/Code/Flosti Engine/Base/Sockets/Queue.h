//----------------------------------------------------------------------------------
// CQueue class
// Author: Gabriel Cirera
//
// Description:
// This class will manage all the messages that a client/server receives to be processed by the Application
//----------------------------------------------------------------------------------
#ifndef __QUEUE_H__
#define __QUEUE_H__

//--------------------------------------------------------------
// -- Queue element Class
//--------------------------------------------------------------
class CQueueElem
{
public:

	CQueueElem(const char* _pszData, unsigned int _uSize);
	~CQueueElem();

	CQueueElem*   m_pNext;
	char*         m_pData;
	unsigned int  m_uSize;
};

//--------------------------------------------------------------
// -- Queue Class
//--------------------------------------------------------------
class CQueue
{
public:

	CQueue() : m_pTail(NULL), m_pHead(NULL), m_uCount(0) {}
	~CQueue();

	void           Dequeue        ();
	void           Enqueue        (const void* _pData, unsigned int _uSize);
	void           Front          (void* pData_, bool _bDequeue);

	unsigned int   GetCount       () { return m_uCount; }
	unsigned int   GetFrontSize   () 
	{ 
		if (m_pHead) return m_pHead->m_uSize;
		else return 0;
	}

private:

	CQueueElem*   m_pHead;
	CQueueElem*   m_pTail;
	unsigned int  m_uCount;
};

#endif