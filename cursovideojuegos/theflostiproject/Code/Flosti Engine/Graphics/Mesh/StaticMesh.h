//----------------------------------------------------------------------------------
// CStaticMesh class
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include <string>
#include <vector>

//---Forward Declarations---
class CRenderableVertexs;
class CRenderManager;
class CEffect;
class CTexture;
//--------------------------

class CStaticMesh
{

public:
	CStaticMesh();
	~CStaticMesh();
	bool	GenMesh	(const std::string &FileName);
	bool	Load		(const std::string &FileName);
	bool	Save		(const std::string &FileName);
	void	Render	(CRenderManager *RM) const;

protected:
	std::vector<CRenderableVertexs*>			m_RVs;
	std::vector<std::vector<CTexture *>>	m_Textures;
	std::string														m_FileName;
	unsigned int													m_NumVertexs, m_NumFaces;
};

#endif //CStaticMesh
