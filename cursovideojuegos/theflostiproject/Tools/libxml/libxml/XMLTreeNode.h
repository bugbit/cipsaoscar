//----------------------------------------------------------------------------------
// CXMLTreeNode class
// This class manages xml files both for read and write.
// It internally uses the libxml tool.
//----------------------------------------------------------------------------------
#pragma once

#ifndef _XML_TREE_NODE_H_
#define _XML_TREE_NODE_H_

// Defines
#define MY_ENCODING "ISO-8859-1"

// Includes
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include <string.h>

//Class
class CXMLTreeNode
{
public:
  // Init and End protocols
  CXMLTreeNode () : m_bIsOk(false), m_pDoc(NULL), m_pNode(NULL), m_pWriter(NULL), m_pszFileName(NULL){}
  virtual ~CXMLTreeNode () { Done(); }

  void               Done               ();
  bool               IsOk               () const { return m_bIsOk; }

  // -----------------------
  // Read functions
  // -----------------------
  bool               LoadFile           (const char* _pszFileName);

  bool               Exists             () { return m_pNode != NULL; }
  bool               ExistsKey          (const char* _pszKey);

  // To get properties from xml file
  int                GetIntProperty     (const char* _pszKey, int _iDefault=0) const;
  float              GetFloatProperty   (const char* _pszKey, float _fDefault=0.0) const;
  bool               GetBoolProperty    (const char* _pszKey, bool _bDefault=false) const;
  const char*        GetPszProperty     (const char* _pszKey, const char* _pszDefault=NULL) const;

  // To get keywords from xml file
  int                GetIntKeyword      (const char* _pszKey, int _iDefault=0) const;
  float              GetFloatKeyword    (const char* _pszKey, float _fDefault=0.0) const;
  bool               GetBoolKeyword     (const char* _pszKey, bool _bDefault=false) const;
  const char*        GetPszKeyword      (const char* _pszKey, const char* _pszDefault=NULL) const;

  int                GetNumChildren     ();

  CXMLTreeNode       operator[]         (const char* _pszKey) const;
  CXMLTreeNode       operator()         (int _iIndex) const;

  // -----------------------
  // Write functions
  // -----------------------
  bool               StartNewFile       (const char* _pszFileName);
  void               EndNewFile         ();
  bool               WriteComment       (const char* _pszComment);
  bool               StartElement       (const char* _pszKey);
  bool               EndElement         ();
  
  // To write keywords to xml file
  bool               WritePszKeyword    (const char* _pszKey, const char* _pszValue);
  bool               WriteIntKeyword    (const char* _pszKey, int _iValue);
  bool               WriteFloatKeyword  (const char* _pszKey, float _fValue);
  bool               WriteBoolKeyword   (const char* _pszKey, bool _bValue);

  // To write properties to xml file
  bool               WritePszProperty   (const char* _pszKey, const char* _pszValue);
  bool               WriteIntProperty   (const char* _pszKey, int _iValue);
  bool               WriteFloatProperty (const char* _pszKey, float _fValue);
  bool               WriteBoolProperty  (const char* _pszKey, bool _bValue);

private:
  void               Release            ();
  CXMLTreeNode       GetSubTree         (const char* _pszKey) const;
  bool              _FindSubTree        (xmlNodePtr _pNode, const char* _pszKey, CXMLTreeNode& _TreeFound) const;
  
  xmlChar*           GetProperty        (const char* _pszKey) const;
  xmlChar*           GetKeyword         (const char* _pszKey) const;

  xmlChar*           ConvertInput       (const char *_pszIn, const char *_pszEncoding);

  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  xmlDocPtr           m_pDoc;           // Pointer to the doc xmllib structure
  xmlNodePtr          m_pNode;          // Pointer to the root node xmllib structure
  xmlTextWriterPtr    m_pWriter;        // Pointer to the writer xmllib structure
  const char*         m_pszFileName;    // Name of the new file to be created
};


#endif