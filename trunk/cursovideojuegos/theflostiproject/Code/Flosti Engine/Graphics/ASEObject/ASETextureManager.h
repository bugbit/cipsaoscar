//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// 
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_ASE_TEXTURE_MANAGER_H_
#define INC_ASE_TEXTURE_MANAGER_H_

#include <d3dx9.h>
#include <map>
#include <string>

class CASETextureManager
{

public:
	CASETextureManager();
	~CASETextureManager() {/*Nothing todo*/;}

	static CASETextureManager *	GetInstance			();
	void												CleanUp					();
	LPDIRECT3DTEXTURE9					LoadTexture			(const std::string& filename, LPDIRECT3DDEVICE9 Device);
	void												SetTexturePath	(const std::string& path) {m_TEXTURE_PATH = path;}

private:
	void												GetFilename			(const char *path, std::string *filename);

private:
	typedef std::map<std::string,LPDIRECT3DTEXTURE9>::iterator TTextureIterator;

	static CASETextureManager*									m_TextureManager;
	std::map<std::string,LPDIRECT3DTEXTURE9>		m_TextureMap;
	std::string																	m_TEXTURE_PATH;
};

#endif //INC_ASE_TEXTURE_MANAGER_H_