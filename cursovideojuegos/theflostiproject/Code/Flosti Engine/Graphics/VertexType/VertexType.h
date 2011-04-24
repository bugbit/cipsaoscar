//----------------------------------------------------------------------------------
// VertexType 
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef VERTEX_TYPE_H
#define VERTEX_TYPE_H

#include "Base.h"

void CalcTangentsAndBinormals(	void *VtxsData, unsigned short *IdxsData, size_t VtxCount, size_t IdxCount,
															size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t TangentStride,
															size_t BiNormalStride, size_t TextureCoordsStride);


#define VERTEX_TYPE_GEOMETRY		0x0001
#define VERTEX_TYPE_NORMAL			0x0002
#define VERTEX_TYPE_TANGENT			0x0004
#define VERTEX_TYPE_BINORMAL		0x0008
#define VERTEX_TYPE_TEXTURE1		0x0010
#define VERTEX_TYPE_TEXTURE2		0x0020
#define VERTEX_TYPE_DIFFUSE			0x0040

struct TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX
{
	float		x, y, z;
	float		nx, ny, nz, nw;
	float		tangentx, tangenty, tangentz, tangentw;
	float		binormalx, binormaly, binormalz, binormalw;
	float		tu, tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE1;
	}
	static inline unsigned int GetFVF()
	{
		return 0;
	}
	static LPDIRECT3DVERTEXDECLARATION9						s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration(CRenderManager *RM);
	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}

};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX2 (D3DFVF_XYZ|D3DFVF_TEX1)
#define D3DFVF_SCREEN_COLOR_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define D3DFVF_SCREEN_TEXTURE_VERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)

struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};

struct CUSTOMVERTEX2
{
	D3DXVECTOR3 pos;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};

struct CUSTOMVERTEX3
{
	float x,y,z;
	float u,v;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX2;
	}
};

struct SCREEN_COLOR_VERTEX
{
	float x,y,z,w;
	DWORD color;

	static unsigned int getFlags()
	{
		return D3DFVF_SCREEN_COLOR_VERTEX;
	}
};

struct SCREEN_TEXTURE_VERTEX
{
	float x,y,z,w;
	float u,v;
	static unsigned int getFlags()
	{
		return D3DFVF_SCREEN_TEXTURE_VERTEX;
	}
};

#endif
