#include "__PCH_Quake.h"

#include "GUIPlayerObjectOfCounterRender.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Camera.h"

CGUIPlayerObjectOfCounterRender::CGUIPlayerObjectOfCounterRender():CGUIPlayerObjectASERender()
{
}

CGUIPlayerObjectOfCounterRender::~CGUIPlayerObjectOfCounterRender(void)
{
}

void CGUIPlayerObjectOfCounterRender::RenderScene(CRenderManager *renderManager, CFontManager *fm, CCamera *camera, CGUIPlayer &guiplayer)
{
	D3DXMATRIX translation;
	D3DXMATRIX rotation0;
	D3DXMATRIX rotation;
	D3DXMATRIX rotation2;
	Vect3f position = camera->GetObject3D()->GetPosition();
	float yaw =  camera->GetObject3D()->GetYaw();
	float pitch =  camera->GetObject3D()->GetPitch();

	D3DXMatrixRotationZ ( &rotation2,  pitch);

	//--------------Paint the Item------------------------------
	D3DXMatrixTranslation( &translation, position.x,position.y ,position.z);
	
	D3DXMatrixRotationY ( &rotation,  -yaw);
	D3DXMatrixRotationY ( &rotation0,  ePIf+sin( -guiplayer.GetYawGun() )*0.1f);
	
	m_Matrix = rotation0 * mTranslation * rotation2 * rotation * translation; 

	CGUIPlayerObjectASERender::RenderScene(renderManager,fm,camera,guiplayer);
}
