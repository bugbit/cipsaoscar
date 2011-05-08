#include "__PCH_Graphics.h"

#include <d3dx9.h>
#include "BillBoard.h"
#include "VertexType/VertexType.h"
#include "Camera.h"
#include "Object3D.h"
#include "RenderManager.h"
#include "Textures/Texture.h"
#include "Base/Math/Matrix34.h"

CBillBoard::CBillBoard()
: m_Position(Vect3f(0,0,0))
, m_SizeX(2)
, m_SizeY(2)
, m_fDesfaseUActual(0.0f)
, m_fDesfaseVActual(0.0f)
, m_fPitch(0.0f)
, m_fYaw(0.0f)
, m_bFixedPitch(false)
, m_bFixedYaw(false)
{}

void CBillBoard::Update(CCamera *camera,float dt)
{

	/*0, 0.5             0.5, 0.5
	A	/------------------\	B
		|              /   |
		|       POS /      |
		|      / *         |
		| /                |
	C	\------------------/	D
	0, 0                 0.5, 0*/

  float yaw_angle;

  if(!m_bFixedYaw)
    yaw_angle = camera->GetObject3D()->GetYaw();
  else
    yaw_angle = m_fYaw;

	Vect3f l_VRight = Vect3f( cos(yaw_angle-D3DX_PI*0.5f), 0.0f, sin(yaw_angle-D3DX_PI*0.5f) );
  Vect3f l_VUp;

  if(!m_bFixedPitch)
    l_VUp = Vect3f(camera->GetVecUp().x,camera->GetVecUp().y,camera->GetVecUp().z);
  else
    l_VUp = Vect3f( -cos(yaw_angle) * sin(m_fPitch), cos(m_fPitch), -sin(yaw_angle) * sin(m_fPitch) );

	float normalize =	l_VRight.x*l_VRight.x + l_VRight.y*l_VRight.y + (l_VRight.z * l_VRight.z);

	normalize = sqrt( normalize );
	l_VRight = l_VRight/normalize;

	normalize =	l_VUp.x*l_VUp.x + l_VUp.y*l_VUp.y + l_VUp.z*l_VUp.z;
	normalize = sqrt( normalize );
	l_VUp = l_VUp/normalize;

	m_PointA = m_Position - (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointB = m_Position + (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointC = m_Position - (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
	m_PointD = m_Position + (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
}

void CBillBoard::Render(CRenderManager *rm,const CTexture& texture )
{
  rm->SetTransform((Mat44f) m44fIDENTITY);
  
  LPDIRECT3DDEVICE9 pd3dDevice = rm->GetDevice();

  TTEXTURE_VERTEX l_Points[4];
  unsigned short l_Indexes[6]={0,2,1,1,2,3};

  //0 0  1
  //0 1  3
  //1 0  2
  //1 1  4
  

  pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

  pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

  l_Points[0].x = m_PointA.x;
  l_Points[0].y = m_PointA.y;
  l_Points[0].z = m_PointA.z;
  l_Points[0].u = m_fDesfaseUActual;
  l_Points[0].v = m_fDesfaseVActual + m_fDesfaseV;

  l_Points[1].x = m_PointB.x;
  l_Points[1].y = m_PointB.y;
  l_Points[1].z = m_PointB.z;
  l_Points[1].u = m_fDesfaseUActual + m_fDesfaseU;
  l_Points[1].v = m_fDesfaseVActual + m_fDesfaseV;

  l_Points[2].x = m_PointC.x;
  l_Points[2].y = m_PointC.y;
  l_Points[2].z = m_PointC.z;
  l_Points[2].u = m_fDesfaseUActual;
  l_Points[2].v = m_fDesfaseVActual;

  l_Points[3].x = m_PointD.x;
  l_Points[3].y = m_PointD.y;
  l_Points[3].z = m_PointD.z;
  l_Points[3].u = m_fDesfaseUActual + m_fDesfaseU;
  l_Points[3].v = m_fDesfaseVActual;

	rm->GetDevice()->SetTexture(0,texture.GetD3DXTexture());
  //device->SetTexture(0, Texture);
  rm->GetDevice()->SetFVF(TTEXTURE_VERTEX::GetFVF());
  rm->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(TTEXTURE_VERTEX));
}

CBillBoardColor::CBillBoardColor()
:CBillBoard(),
m_Color(0)
{
}

void CBillBoardColor::Render(CRenderManager *rm, const CTexture &texture)
{
	rm->SetTransform((Mat44f) m44fIDENTITY);
  
  LPDIRECT3DDEVICE9 pd3dDevice = rm->GetDevice();

  COLOR_VERTEX l_Points[4];
  unsigned short l_Indexes[6]={0,2,1,1,2,3};

  //0 0  1
  //0 1  3
  //1 0  2
  //1 1  4
  
	//////--render el quad de difuminacion---//

	pd3dDevice->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE  );

	pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); 
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); 

  l_Points[0].x = m_PointA.x;
  l_Points[0].y = m_PointA.y;
  l_Points[0].z = m_PointA.z;
  l_Points[0].u = m_fDesfaseUActual;
  l_Points[0].v = m_fDesfaseVActual + m_fDesfaseV;
	l_Points[0].color = m_Color;

  l_Points[1].x = m_PointB.x;
  l_Points[1].y = m_PointB.y;
  l_Points[1].z = m_PointB.z;
  l_Points[1].u = m_fDesfaseUActual + m_fDesfaseU;
  l_Points[1].v = m_fDesfaseVActual + m_fDesfaseV;
	l_Points[1].color = m_Color;

  l_Points[2].x = m_PointC.x;
  l_Points[2].y = m_PointC.y;
  l_Points[2].z = m_PointC.z;
  l_Points[2].u = m_fDesfaseUActual;
  l_Points[2].v = m_fDesfaseVActual;
	l_Points[2].color = m_Color;

  l_Points[3].x = m_PointD.x;
  l_Points[3].y = m_PointD.y;
  l_Points[3].z = m_PointD.z;
  l_Points[3].u = m_fDesfaseUActual + m_fDesfaseU;
  l_Points[3].v = m_fDesfaseVActual;
	l_Points[3].color = m_Color;

	rm->GetDevice()->SetTexture(0,texture.GetD3DXTexture());
  //device->SetTexture(0, Texture);
  rm->GetDevice()->SetFVF(TTEXTURE_VERTEX::GetFVF());
  rm->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(TTEXTURE_VERTEX));
}