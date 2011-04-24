//----------------------------------------------------------------------------------
// CASEObject class
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_ASE_OBJECT_H
#define INC_ASE_OBJECT_H

#include <vector>
#include <d3dx9.h>
#include <string>

//---Forward Declarations--
class CRenderManager;
//-------------------------


class CASEObject
{
public:
	CASEObject();
	~CASEObject() {/*Nothing*/;}

	bool				Load							( const char *nombreFichero, CRenderManager* manager, char *nombreObjeto=NULL);
	void				CleanUp						( void );
	void				Render						(CRenderManager* manager);
	bool				IsInBoundingBox		( const Vect3f &Pos );

private:
	size_t									GetNumTextures( ) const { return listaTexturas.size();}
	size_t									GetNumFacesByMat(size_t IdMaterial) const {return vec_numCaresByMat[IdMaterial];}
	LPDIRECT3DINDEXBUFFER9	GetIndexBufferByMat(size_t IdMaterial) {return vec_pIBMeshByMat[IdMaterial];}
	LPDIRECT3DVERTEXBUFFER9	GetVertexBufferByMat(size_t IdMaterial) {return vec_pVBGeomTexturaByMat[IdMaterial];}
	LPDIRECT3DBASETEXTURE9	GetTextureByMat(size_t IdMaterial) const {return vec_textures[IdMaterial];}
	static void							ReleaseVertexDeclaration();

private:
	D3DXVECTOR3		pos;
	D3DXVECTOR3		CCmin;
	D3DXVECTOR3		CCmax;
	uint32				numVertices;
	uint32				numCaras;
	uint32				numTVertices;
	uint32				numTCaras;
	uint32				numTexturas;
	float					m_Radius;
	
	std::vector< LPDIRECT3DVERTEXBUFFER9 >	vec_pVBMeshByMat;
	std::vector< LPDIRECT3DINDEXBUFFER9 >		vec_pIBMeshByMat;
	std::vector< LPDIRECT3DVERTEXBUFFER9 >	vec_pVBGeomTexturaByMat;
	std::vector< LPDIRECT3DBASETEXTURE9 >		vec_textures;
	std::vector< int >											vec_numCaresByMat;
	std::vector <std::string>								listaTexturas;
	static LPDIRECT3DVERTEXDECLARATION9			m_pASEVertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9			GetASEVertexDeclaration(LPDIRECT3DDEVICE9 Device);
};

#endif //INC_ASE_OBJECT_H