#include "__PCH_Base.h"

#include <stddef.h>        // #def NULL
#include <memory.h>        // memcpy()
#include "Queue.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
// -- Queue class implementation
//--------------------------------------------------------------
//--------------------------------------------------------------

//----------------------------------------
// -- Free memory
//----------------------------------------
CQueue::~CQueue()
{
	while (m_pHead) Dequeue();
	m_pTail = NULL;
	m_pHead = NULL;
	m_uCount = 0;
}

//----------------------------------------
// -- Delete head element
//----------------------------------------
void CQueue::Dequeue()
{
	CQueueElem *pTemp;

	// already empty
	if (m_uCount == 0) 
		return;

	// exactly one element
	else if (m_uCount == 1)
	{
		delete m_pHead;
		m_pHead = NULL;
		m_pTail = NULL;
	}
	else
	{
		pTemp = m_pHead;
		m_pHead = m_pHead->m_pNext;
		delete pTemp;
	}

	--m_uCount;
}

//-----------------------------------------------
// -- Add a new element at the end of the queue
//-----------------------------------------------
void CQueue::Enqueue(const void* _pszData, unsigned int _uSize)
{
	CQueueElem* pNew = new CQueueElem((const char*)_pszData, _uSize);

	// this is the first package
	if (m_uCount == 0)
	{
		m_pHead = pNew;
		m_pTail = pNew;
	}
	// this is the second package
	else if(m_uCount == 1)
	{
		m_pHead->m_pNext = pNew;
		m_pTail = pNew;
	}
	else
	{
		m_pTail->m_pNext = pNew;
		m_pTail = pNew;
	}

	++m_uCount;
}

//-----------------------------------------------
// -- Get data from the front of the queue
//-----------------------------------------------
void CQueue::Front(void* pData_, bool _bDequeue)
{
	if (pData_ && m_pHead)
	{
		memcpy(pData_, m_pHead->m_pData, m_pHead->m_uSize);
	}
	if (_bDequeue) 
		Dequeue();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// -- Queue element class implementation
//--------------------------------------------------------------
//--------------------------------------------------------------

//----------------------------------------
// -- Constructor
//----------------------------------------
CQueueElem::CQueueElem(const char* _pszData, unsigned int _uSize)
{
	m_pData = NULL;
	m_pNext = NULL;
	m_pData = new char[_uSize];
	m_uSize = _uSize;
	memcpy(m_pData, _pszData, _uSize);
} 

//----------------------------------------
// -- Free memory
//----------------------------------------
CQueueElem::~CQueueElem(void)
{
	if (m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	m_pNext = NULL;
} 