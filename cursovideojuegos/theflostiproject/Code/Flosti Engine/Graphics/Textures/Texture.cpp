#include "__PCH_Graphics.h"
#include "Texture.h"
#include "Core/Core.h"
#include "RenderManager.h"
#include "Logger/Logger.h"


CTexture::~CTexture()
{
  UnLoad();
}

void CTexture::UnLoad()
{
  CHECKED_RELEASE(m_Texture);
}

bool CTexture::LoadTexture()
{
	UnLoad();
	D3DXIMAGE_INFO info;
  CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();

	HRESULT result;
	result = D3DXGetImageInfoFromFile(m_FileName.c_str(), &info);
	if (result != D3D_OK) 	
	{
		LOGGER->AddNewLog(ELL_ERROR,"CTexture::LoadTexture->Error al obtenre la información del fichero:: %s",m_FileName.c_str());
		return false;
	}

	D3DCOLOR color = D3DCOLOR_COLORVALUE(0.f,0.f,0.f,0.f);
	HRESULT l_HR = D3DXCreateTextureFromFileEx(  
		l_RM->GetDevice(),			//Direct3D device object
		m_FileName.c_str(),			//bitmap filename
		info.Width,							//bitmap image width
		info.Height,						//bitmap image height
		1,											//mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,				//the type of surface (standard)
		D3DFMT_UNKNOWN,					//surface format (default)
		D3DPOOL_DEFAULT,				//memory class for the texture
		D3DX_DEFAULT,						//image filter
		D3DX_DEFAULT,						//mip filter
		color,									//color key for transparency
		&info,									//bitmap file info (from loaded file)
		NULL,										//color palette
		&m_Texture );						//destination texture

  if(l_HR!=D3D_OK)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::LoadTexture->Error al crear la textura del fichero: %s",m_FileName.c_str());
    return false;
  }

	m_uWidth	= info.Width;
	m_uHeight	= info.Height;
  return true;
}

bool CTexture::Save(const std::string &FileName)
{
	if (!m_Texture)
	{
		return false;
	}

	HRESULT result;
	result = D3DXSaveTextureToFile(  FileName.c_str(),
																	 D3DXIFF_PNG,
																	 m_Texture,
																	 NULL );

	if(result!=D3D_OK)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::Save->Error al guardar la textura en el fichero: %s",FileName.c_str());
    return false;
  }

	return true;
}

bool CTexture::Load(const std::string &FileName)
{
  m_FileName=FileName;
  return LoadTexture();
}

bool CTexture::Reload()
{
  UnLoad();
  return LoadTexture();
}

void CTexture::Activate(size_t Stage)
{
  CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();
  l_RM->GetDevice()->SetTexture((DWORD)Stage,m_Texture);
}

LPDIRECT3DSURFACE9 CTexture::GetD3DSurface () const
{
	if (m_Texture)
	{
		LPDIRECT3DSURFACE9 renderSurface;
		m_Texture->GetSurfaceLevel(0,&renderSurface);
		return renderSurface;
	}
	return NULL;
}


bool CTexture::CreateTexture (uint32 width, uint32 height, bool renderTarget)
{
	UnLoad();
  CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();
	if (renderTarget)
	{
		l_RM->GetDevice()->CreateTexture(	width, height, 1, D3DUSAGE_RENDERTARGET	, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &m_Texture, NULL);
	}
	else
	{
		l_RM->GetDevice()->CreateTexture(	width, height, 0, 0	, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_Texture, NULL);
	}

	if (!m_Texture)
	{
		l_RM->GetDevice()->CreateTexture(	width, height, 0, 0	, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_Texture, NULL);
	}

	if (!m_Texture)
  {

    LOGGER->AddNewLog(ELL_ERROR,"CTexture::CreateTextureFromBuffer-> Error al crear la textura de buffeer");
    return false;
  }
  else
  {
		m_uWidth	= width;
		m_uHeight	= height;
    return true;
  }
}

bool CTexture::FillTextureWithColor (uint32 width, uint32 height, const CColor& color)
{
  if (m_Texture==NULL)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor-> m_Texture no ha sido creado aun ");
    return false;
  }

  HRESULT hr;
  D3DLOCKED_RECT lock;
  hr=m_Texture->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
  if(hr==D3D_OK)
  {
    uint8 *pTxtBuffer; // Bitmap buffer, texture buffer
    pTxtBuffer = (uint8*)lock.pBits;

		
    uint32 j = 0;
    for( uint32 cont = 0; cont< width * height; cont++)
    {
      //BLUE
      pTxtBuffer[cont*4 + 0] = (uint8)(color.GetBlue()*255);
      //GREEN
      pTxtBuffer[cont*4 + 1] = (uint8)(color.GetGreen()*255);
      //RED
      pTxtBuffer[cont*4 + 2] = (uint8)(color.GetRed()*255);
      //ALPHA
      pTxtBuffer[cont*4 + 3] = (uint8)(color.GetAlpha()*255);
    }
    hr=m_Texture->UnlockRect(0);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor->Error en la llamada lockRect");
    return false;
  }

	m_uWidth	= width;
	m_uHeight	= height;

  return true;
}

bool CTexture::FillTextureFromBuffer (uint32 width, uint32 height, uint8* pImgBuffer)
{
  if (m_Texture==NULL)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureFromBuffer-> m_Texture no ha sido creado aun ");
    return false;
  }

  HRESULT hr;
  D3DLOCKED_RECT lock;
  hr=m_Texture->LockRect(0,&lock,0,D3DLOCK_DISCARD);
  if(hr==D3D_OK)
  {
    uint8 *pTxtBuffer; // Bitmap buffer, texture buffer
    pTxtBuffer = (uint8*)lock.pBits;

    uint32 j = 0;
    for( uint32 cont = 0; cont< width * height; cont++)
    {
      //BLUE
      pTxtBuffer[cont*4 + 0] = pImgBuffer[j]; j++;
      //GREEN
      pTxtBuffer[cont*4 + 1] = pImgBuffer[j]; j++; 			
      //RED
      pTxtBuffer[cont*4 + 2] = pImgBuffer[j]; j++;
      //ALPHA
      pTxtBuffer[cont*4 + 3] = 255;
    }
    hr=m_Texture->UnlockRect(0);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureFromBuffer->Error en la llamada lockRect");
    return false;
  }

	m_uWidth	= width;
	m_uHeight	= height;

  return true;
}