#include "XMLTreeNode.h"

#include "Assert.h"


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CXMLTreeNode::Done ()
{
  if (IsOk())
  {
     Release();
     m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CXMLTreeNode::Release ()
{
  if (m_pDoc)
  {
    xmlFreeDoc(m_pDoc);
    m_pDoc = NULL;
  }

  m_pNode = NULL;
  
  if (m_pWriter)
  {
    xmlFreeTextWriter(m_pWriter);
    m_pWriter = NULL;
  }

  xmlCleanupParser();
}

//----------------------------------------------------------------------------
// Load File
//----------------------------------------------------------------------------
bool CXMLTreeNode::LoadFile (const char* _pszFileName)
{
  m_bIsOk = false;

  assert(_pszFileName);

  if (_pszFileName)
  {
    m_pDoc = xmlParseFile(_pszFileName);
    assert(m_pDoc);

    if (m_pDoc)
    {
      m_pNode = xmlDocGetRootElement(m_pDoc);
      assert(m_pNode);

      if (m_pNode)
      {
        m_bIsOk = true;
        return true;
      }
    }
  }

  Release();

  return false;
}

//----------------------------------------------------------------------------
// Returns a subtree node from a given key
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::GetSubTree (const char* _pszKey) const
{
  assert(m_pNode && _pszKey);
  CXMLTreeNode NewTree;

  if (_pszKey && m_pNode)
  {
    _FindSubTree(m_pNode, _pszKey, NewTree);
  }

  return NewTree;
}

//----------------------------------------------------------------------------
// Recursive to find a key on a tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::_FindSubTree(xmlNodePtr _pNode, const char* _pszKey, CXMLTreeNode& _TreeFound) const
{
  while (_pNode != NULL)
  {
    if (xmlStrcmp(_pNode->name, (const xmlChar*)_pszKey))
    {
      if (_FindSubTree(_pNode->xmlChildrenNode, _pszKey, _TreeFound))
      {
        return true;
      }
    }
    else
    {
      _TreeFound.m_pNode = _pNode;
      _TreeFound.m_pDoc = m_pDoc;
      return true;
    }

    _pNode = _pNode->next;
  }

  return false;
}

//----------------------------------------------------------------------------
// Operator that returns a tree node
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator[] (const char* _pszKey) const
{
  assert(_pszKey && m_pNode);

  CXMLTreeNode TreeFound;

  if (_pszKey && m_pNode)
  {
    TreeFound = GetSubTree(_pszKey);
  }

  return TreeFound;
}

//----------------------------------------------------------------------------
// Operator to get children nodes
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator() (int _iIndex) const
{
  assert(_iIndex >= 0 && m_pNode);

  CXMLTreeNode TreeFound;

  if (_iIndex >= 0 && m_pNode)
  {
    int iCount = 0;
    xmlNodePtr pChildren = m_pNode->children;
    while (pChildren != NULL)
    {
      if (_iIndex == iCount)
      {
        TreeFound.m_pNode = pChildren;
        TreeFound.m_pDoc = m_pDoc;
        break;
      }

      pChildren = pChildren->next;
      ++iCount;
    }
  }

  return TreeFound;
}

//----------------------------------------------------------------------------
// Returns the number of children a tree has
//----------------------------------------------------------------------------
int CXMLTreeNode::GetNumChildren ()
{
  assert(m_pNode);

  int iCount = 0;

  if (m_pNode)
  {
    xmlNodePtr pChildren = m_pNode->children;
    while (pChildren != NULL)
    {
      ++iCount;
      pChildren = pChildren->next;
    }
  }

  return iCount;
}

//----------------------------------------------------------------------------
// Returns a param of the tree from a given key
//----------------------------------------------------------------------------
xmlChar* CXMLTreeNode::GetProperty (const char* _pszKey) const
{
  assert(_pszKey && m_pNode);

  xmlChar* value = NULL;

  if (_pszKey && m_pNode)
  {
    value = xmlGetProp(m_pNode, (const xmlChar*)_pszKey);
  }

  return value;
}

//----------------------------------------------------------------------------
// Returns an integer param if found. Else a default value
//----------------------------------------------------------------------------
int CXMLTreeNode::GetIntProperty (const char* _pszKey, int _iDefault) const
{
  int iRet = _iDefault;

  xmlChar* value = GetProperty(_pszKey);

  if (value)
  {
    iRet = atoi((const char*)value);
  }

  xmlFree(value);

  return iRet;
}

//----------------------------------------------------------------------------
// Returns a float param if found. Else a default value
//----------------------------------------------------------------------------
float CXMLTreeNode::GetFloatProperty (const char* _pszKey, float _fDefault) const
{
  float fRet = _fDefault;

  xmlChar* value = GetProperty(_pszKey);

  if (value)
  {
    fRet = static_cast<float>(atof((const char*)value));
  }

  xmlFree(value);

  return fRet;
}
//----------------------------------------------------------------------------
// Returns a boolean param if found. Else a default value
//----------------------------------------------------------------------------

bool CXMLTreeNode::GetBoolProperty (const char* _pszKey, bool _bDefault) const
{
  bool bRet = _bDefault;

  xmlChar* value = GetProperty(_pszKey);

  if (value)
  {
    const char* pszValue = (const char*)value;
    if (strcmp("TRUE", pszValue) == 0 || strcmp("true", pszValue) == 0 || strcmp("True", pszValue) == 0)
    {
      bRet = true;
    }
    else
      bRet = false;
  }

  xmlFree(value);

  return bRet;
}

//----------------------------------------------------------------------------
// Returns an string param if found. Else a default value
//----------------------------------------------------------------------------
const char* CXMLTreeNode::GetPszProperty (const char* _pszKey, const char* _pszDefault) const
{
  const char* pszRet = _pszDefault;

  xmlChar* value = GetProperty(_pszKey);

  if (value)
  {
    pszRet = (const char*)value;
  }

  return pszRet;
}

//----------------------------------------------------------------------------
// Returns an keyword from the tree from a given key
//----------------------------------------------------------------------------
xmlChar* CXMLTreeNode::GetKeyword (const char* _pszKey) const
{
  assert(_pszKey && m_pNode && m_pDoc);

  xmlChar* value = NULL;

  if (_pszKey && m_pNode && m_pDoc)
  {
    CXMLTreeNode FoundTree;
    if (_FindSubTree(m_pNode, _pszKey, FoundTree))
      value = xmlNodeListGetString(FoundTree.m_pDoc, FoundTree.m_pNode->xmlChildrenNode, 1);
  }

  return value;
}

//----------------------------------------------------------------------------
// Returns an integer keyword if found. Else a default value
//----------------------------------------------------------------------------
int CXMLTreeNode::GetIntKeyword (const char* _pszKey, int _iDefault/*=0*/) const
{
  int iRet = _iDefault;

  xmlChar* value = GetKeyword(_pszKey);

  if (value)
  {
    iRet = atoi((const char*)value);
  }

  return iRet;
}

//----------------------------------------------------------------------------
// Returns a float keyword if found. Else a default value
//----------------------------------------------------------------------------
float CXMLTreeNode::GetFloatKeyword (const char* _pszKey, float _fDefault/*=0.0*/) const
{
  float fRet = _fDefault;

  xmlChar* value = GetKeyword(_pszKey);

  if (value)
  {
    fRet = static_cast<float>(atof((const char*)value));
  }

  return fRet;
}

//----------------------------------------------------------------------------
// Returns a boolean keyword if found. Else a default value
//----------------------------------------------------------------------------
bool CXMLTreeNode::GetBoolKeyword (const char* _pszKey, bool _bDefault/*=false*/) const
{
  bool bRet = _bDefault;

  xmlChar* value = GetKeyword(_pszKey);

  if (value)
  {
    const char* pszValue = (const char*)value;
    if (strcmp("TRUE", pszValue) == 0 || strcmp("true", pszValue) == 0 || strcmp("True", pszValue) == 0)
    {
      bRet = true;
    }
    else
      bRet = false;
  }

  return bRet;
}

//----------------------------------------------------------------------------
// Returns a string keyword if found. Else a default value
//----------------------------------------------------------------------------
const char* CXMLTreeNode::GetPszKeyword (const char* _pszKey, const char* _pszDefault/*=NULL*/) const
{
  const char* pszRet = _pszDefault;

  xmlChar* value = GetKeyword(_pszKey);

  if (value)
  {
    pszRet = (const char*)value;
  }

  return pszRet;
}

//----------------------------------------------------------------------------
// Checks if a key is on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::ExistsKey (const char* _pszKey)
{
  assert(_pszKey);

  CXMLTreeNode TreeFound = GetSubTree(_pszKey);
  return TreeFound.Exists();
}

//----------------------------------------------------------------------------
// Starts a new file and prepares it to be written
//----------------------------------------------------------------------------
bool CXMLTreeNode::StartNewFile(const char* _pszFileName)
{
  assert(_pszFileName);
  
  m_bIsOk = false;
  
  if (_pszFileName)
  {
    m_pszFileName = _pszFileName;

    // Create a new XmlWriter for DOM, with no compression.
    m_pWriter = xmlNewTextWriterDoc(&m_pDoc, 0);
    assert(m_pWriter);

    if (m_pWriter)
    {
      // Start the document with the xml default for the version, encoding ISO 8858-1 and the default for the standalone declaration.
      int rc = xmlTextWriterStartDocument(m_pWriter, NULL, MY_ENCODING, NULL);
      assert(rc >= 0);
      if (rc >= 0) 
      {
        m_bIsOk = true;
        return true;
      }
    }
  }

  Release();

  return false;
}


//----------------------------------------------------------------------------
// Finished a file and saves it
//----------------------------------------------------------------------------
void CXMLTreeNode::EndNewFile ()
{
  assert(m_pWriter && m_pDoc && m_pszFileName);

  if (m_pWriter && m_pDoc && m_pszFileName)
  {
    xmlFreeTextWriter(m_pWriter);
    m_pWriter = NULL;

    xmlSaveFileEnc(m_pszFileName, m_pDoc, MY_ENCODING);
  }
}

//----------------------------------------------------------------------------
// Write a comment in the xml file in the current node
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteComment(const char* _pszComment)
{
  assert(_pszComment && m_pWriter);

  if (_pszComment && m_pWriter)
  {
    int rc = xmlTextWriterWriteComment(m_pWriter, BAD_CAST _pszComment);
    assert(rc >= 0);

    if (rc < 0)
      return false;
  }
  else
    return false;

  return true;
}

//----------------------------------------------------------------------------
// Starts a new node on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::StartElement(const char* _pszKey)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    // Start an element named "EXAMPLE". Since thist is the first element, this will be the root element of the document.
    int rc = xmlTextWriterStartElement(m_pWriter, BAD_CAST _pszKey);
    assert(rc >= 0);

    if (rc < 0)
      return false;
  }
  else
    return false;

  return true;
}

//----------------------------------------------------------------------------
// Starts a new node on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::EndElement()
{
  assert(m_pWriter);

  if (m_pWriter)
  {
    /* Close the element named HEADER. */
    int rc = xmlTextWriterEndElement(m_pWriter);
    assert(rc >= 0);

    if (rc < 0)
      return false;
  }
  else
    return false;

  return true;
}

//----------------------------------------------------------------------------
// Writes a string keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WritePszKeyword(const char* _pszKey, const char* _pszValue)
{
  assert(_pszKey && _pszValue && m_pWriter);

  if (_pszKey && _pszValue && m_pWriter)
  {
    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST _pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes an integer keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteIntKeyword(const char* _pszKey, int _iValue)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _iValue);

    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes a float keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteFloatKeyword(const char* _pszKey, float _fValue)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _fValue);

    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes a boolean keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteBoolKeyword(const char* _pszKey, bool _bValue)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _bValue ? "true" : "false");

    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes a string property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WritePszProperty(const char* _pszKey, const char* _pszValue)
{
  assert(_pszKey && _pszValue && m_pWriter);

  if (_pszKey && _pszValue && m_pWriter)
  {
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST _pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes an integer property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteIntProperty(const char* _pszKey, int _iValue)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _iValue);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes a float property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteFloatProperty(const char* _pszKey, float _fValue)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    char pszValue[32];
    sprintf_s(pszValue, "%f", _fValue);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}

//----------------------------------------------------------------------------
// Writes a boolean property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteBoolProperty(const char* _pszKey, bool _bValue)
{
  assert(_pszKey && m_pWriter);

  if (_pszKey && m_pWriter)
  {
    char pszValue[32];
    sprintf_s(pszValue, _bValue ? "true" : "false");

    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }

  return false;
}