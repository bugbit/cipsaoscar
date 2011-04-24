#include "__PCH_Graphics.h"
#include "AviPlayer.h"
#include "Logger/Logger.h"
#include "Exceptions/Exception.h"
#include "Graphics/RenderManager.h"


CAviPlayer::CAviPlayer(CRenderManager* rm)
: m_lTimeMS(0)
, m_bPlaying(false)
, m_bDone(false)
, m_iActiveStream(0)
, m_iLastFrame(-1)
, m_pAviFile(NULL)
, m_pTexture(NULL)
, m_fDecimalTime(0.f)
{
  AVIFileInit();
	m_pDevice = rm->GetDevice();
	assert(m_pDevice);
}

CAviPlayer::~CAviPlayer()
{
  AVIFileExit();
  Release();
}

bool CAviPlayer::Load(const std::string& fileName)
{
  try
  {  

    if(AVIFileOpen(&m_pAviFile, fileName.c_str(), OF_READ, NULL) != 0)
		{
			/*LOGGER->AddNewLog(ELL_ERROR, "CAviPlayer:: No se ha podido leer correctamente el fichero avi ->%s", fileName.c_str());
			std::string msg_error = "Error al leer el archivo avi a cargar: " + fileName;
			throw CException(__FILE__, __LINE__, msg_error);*/
			return false;
		}

    //Retreive Audio streams
    long streamNo = 0;
    do
    {
      AUDIO_STREAM *as = new AUDIO_STREAM();

      //Get new m_pStream
      if(AVIFileGetStream(m_pAviFile, &as->m_pStream, streamtypeAUDIO, streamNo++))
      {
        delete as;
        break;
      }

      //Add m_pStream
      m_audio.push_back(as);
    }
    while(true);

    streamNo = 0;
    do
    {
      VIDEO_STREAM *vs = new VIDEO_STREAM();

      //Get m_video m_pStream
      if(AVIFileGetStream(m_pAviFile, &vs->m_pStream, streamtypeVIDEO, streamNo++))
      {
        delete vs;
        break;
      }

      //GetFrame object
      vs->m_pGetFrame = AVIStreamGetFrameOpen(vs->m_pStream, NULL);

      //Get runtime
      vs->m_lRunTime = AVIStreamEndTime(vs->m_pStream);

      //Add m_pStream
      m_video.push_back(vs);
    }
    while(true);

    return true;
  }
  catch(...)
  {
		std::string msg_error = "CAviPlayer::Load-> Error in Load";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
  }
}

void CAviPlayer::Release()
{
  for(uint32 i=0; i < m_video.size(); i++)
  {
    CHECKED_DELETE(m_video[i]);
  }
  m_video.clear();

  for(uint32 i=0;i<m_audio.size();i++)
  {
    CHECKED_DELETE(m_audio[i]);
  }
  m_audio.clear();

	AVIFileRelease(m_pAviFile);
}

void CAviPlayer::Play()
{
  m_lTimeMS         = 0;
  m_bPlaying        = true;
  m_bDone           = false;
  m_iActiveStream   = 0;
  m_iLastFrame      = -1;
}

void CAviPlayer::Stop()
{
  m_lTimeMS         = 0;
  m_bPlaying        = false;
  m_bDone           = false;
  m_iActiveStream   = 0;
  m_iLastFrame      = -1;
}

void CAviPlayer::Pause()
{
  m_bPlaying = !m_bPlaying;
}

bool CAviPlayer::Done()
{
  return m_bDone;
}

void CAviPlayer::Update(float deltaTime)
{

  if(m_bPlaying)
  {
		float decimal = deltaTime * 1000;
		long time_aux = 0;
		if (decimal < 1.f )
		{
			m_fDecimalTime += decimal;
			time_aux = (long) m_fDecimalTime;
			m_fDecimalTime -= time_aux;
		}
    m_lTimeMS += (long)(deltaTime * 1000);
		m_lTimeMS += time_aux;
  }
  else 
  {
    return;
  }

  long frame;
  if(m_lTimeMS <= m_video[m_iActiveStream]->m_lRunTime)
  {
    frame = AVIStreamTimeToSample(m_video[m_iActiveStream]->m_pStream, m_lTimeMS);

    //Retrieve new frame
    if(frame != m_iLastFrame && frame != -1)
    {
      m_iLastFrame = frame;

      BITMAPINFOHEADER *bip = NULL;
      bip = (BITMAPINFOHEADER*)AVIStreamGetFrame(m_video[m_iActiveStream]->m_pGetFrame, frame);

      if(bip != NULL)
      {
				assert(m_pTexture);
				LPDIRECT3DTEXTURE9 texture = m_pTexture->GetD3DXTexture();
        if(texture != NULL)
					texture->Release();

        D3DXCreateTextureFromFileInMemoryEx(  m_pDevice, bip, bip->biSize + bip->biWidth * bip->biHeight * bip->biBitCount / 8,
                                              bip->biWidth, bip->biHeight,
                                              1, D3DUSAGE_DYNAMIC, D3DFMT_R8G8B8, D3DPOOL_DEFAULT,
                                              D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);

				m_pTexture->SetD3DXTexture(texture);
      }
    }
  }
  else
  {
    m_iActiveStream++;
    m_lTimeMS = 0;

    //No more streams to play
    if(m_iActiveStream >= m_video.size())
    {
      Stop();
      m_bDone = true;
      m_iActiveStream = 0;
    }
  }	
}