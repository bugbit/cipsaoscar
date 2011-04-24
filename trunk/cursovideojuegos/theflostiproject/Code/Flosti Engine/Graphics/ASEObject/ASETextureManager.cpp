#include "__PCH_Graphics.h"

#include "ASETextureManager.h"
#include <assert.h>




CASETextureManager * CASETextureManager::m_TextureManager=NULL;

CASETextureManager * CASETextureManager::GetInstance()
{
	if(m_TextureManager==NULL)
		m_TextureManager=new CASETextureManager();
	assert(m_TextureManager!=NULL);
	return m_TextureManager;
}

CASETextureManager::CASETextureManager()
: m_TEXTURE_PATH("./")
{}

void CASETextureManager::CleanUp()
{
	for(TTextureIterator it=m_TextureMap.begin();it!=m_TextureMap.end();++it)
	{
		if (it->second)
		{
			it->second->Release();
			it->second = NULL;
		}
		
	}
	m_TextureMap.clear();
	
	if (m_TextureManager)
	{
		delete m_TextureManager;
		m_TextureManager = NULL;
	}
}

LPDIRECT3DTEXTURE9 CASETextureManager::LoadTexture(const std::string &filename, LPDIRECT3DDEVICE9 Device)
{
	LPDIRECT3DTEXTURE9 l_Texture=NULL;
	if(m_TextureMap.find(filename)!=m_TextureMap.end())
		l_Texture=m_TextureMap[filename];
	else
	{
		std::string l_FilenameWithoutPath, l_FilenameComplete;
		GetFilename(filename.c_str(),&l_FilenameWithoutPath);

		l_FilenameComplete=m_TEXTURE_PATH;
		l_FilenameComplete+=l_FilenameWithoutPath;
		D3DXCreateTextureFromFile(Device,l_FilenameComplete.c_str(),&l_Texture);
		m_TextureMap[filename]=l_Texture;
	}
	assert(l_Texture!=NULL);
	return l_Texture;
}


void CASETextureManager::GetFilename(const char *path, std::string *filename)
{
	char *pnt=NULL;
	for(int b=(int)strlen(path);b>0;--b)
	{
		if(path[b]=='\\')
		{
			pnt=(char*)&(path[b+1]);
			b=-1;
		}
	}
	if(pnt==NULL)
		pnt=(char*)&(path[0]);
	*filename=pnt;
}