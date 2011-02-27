#include "__PCH_Quake.h"

#include "GUIPlayerFaceRender.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Camera.h"

CGUIPlayerFaceRender::CGUIPlayerFaceRender(void)
{
}

CGUIPlayerFaceRender::~CGUIPlayerFaceRender(void)
{
}

void CGUIPlayerFaceRender::RenderScene(CRenderManager *renderManager, CFontManager *fm, CCamera *camera, CGUIPlayer &guiplayer)
{
	D3DXMATRIX translation;
	D3DXMATRIX translation2;
	D3DXMATRIX rotation0;
	D3DXMATRIX rotation;
	D3DXMATRIX rotation2;
	Vect3f position = camera->GetObject3D()->GetPosition();
	float yaw =  camera->GetObject3D()->GetYaw();
	float pitch =  camera->GetObject3D()->GetPitch();

	D3DXMatrixRotationZ ( &rotation2,  pitch);

	//--------------Paint the FACE------------------------------
	D3DXMatrixTranslation( &translation, position.x,position.y ,position.z);
	D3DXMatrixTranslation( &translation2, 0.6f, -0.28f , 0.25f);
	
	D3DXMatrixRotationY ( &rotation,  -yaw);
	D3DXMatrixRotationY ( &rotation0,  ePIf+sin( -guiplayer.GetYawGun() )*0.1f);
	
	m_Matrix = rotation0 * translation2 * rotation2 * rotation * translation; 

	CGUIPlayerObjectASERender::RenderScene(renderManager,fm,camera,guiplayer);
}
