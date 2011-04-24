#ifndef __DEFINES_H
#define __DEFINES_H

#include <string>
#include "d3dx9.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEXTEXTURA (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_VERTEX_TEXTURED (D3DFVF_XYZ|D3DFVF_TEX1)
#define D3DFVF_CUSTOMVERTEXNORMALTEXTURA (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


struct SCREEN_COLOR_VERTEX
{
	float x, y, z, w;
	DWORD color;
    float u, v; 
	static unsigned int getFlags()
	{
		return(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	}
};


struct CUSTOMVERTEX
{
    float x, y, z;
    DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};
struct CUSTOMVERTEXTEXTURA
{
    float x, y, z;
	DWORD color;
    float u, v;         // COORDENADAS DE TEXTURAS
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEXTEXTURA;
	}
};

struct VERTEX_TEXTURED
{
	float x, y, z;
    float u, v;         // COORDENADAS DE TEXTURAS
	static unsigned int getFlags()
	{
		return D3DFVF_VERTEX_TEXTURED;
	}
};
struct CUSTOMVERTEXNORMALTEXTURA
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;         // COORDENADAS DE TEXTURAS
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEXNORMALTEXTURA;
	}
};

#define MESH_GEOMETRIA							0x0001
#define MESH_GEOMETRIA_INDEXADA			0x0002
#define MESH_TEXTURAS								0x0004
#define	MESH_TEXTURAS_INDEXADAS			0x0008
#define MESH_COLOR									0x0010
#define MESH_CAJA_CONTENEDORA				0x0020
#define MESH_ESFERA_CONTENEDORA			0x0040

#define DEG2RAD(x) x*D3DX_PI/180.0f
#define RAD2DEG(x) x*180.0f/D3DX_PI
#define LENGTH(a,b) sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z))
#define LENGTHSQR(a,b) ((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z))


#endif