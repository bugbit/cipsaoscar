#include "__PCH_Graphics.h"

#include <d3dx9.h>
#include <vector>
#include <assert.h>

#include "ASEObject.h"
#include "ASEDefines.h"
#include "text.h"
#include "ASETextureManager.h"

#include "RenderManager.h"


#pragma warning( disable : 4996 ) // disable deprecated warning 

CASEObject::CASEObject()
{
	numVertices=0;
	numCaras=0;
	numTVertices=0;
	numTCaras=0;	
}

bool CASEObject::Load(const char *nombreFichero, CRenderManager* manager,  char *nombreObjeto)
{
	LPDIRECT3DDEVICE9 g_pd3dDevice = manager->GetDevice();
	struct CoordsText
	{
		float u,v;
	};
	CoordsText *g_VerticesTextura=NULL;
	VOID *pMesh, *pMeshIndices;
	CUSTOMVERTEX *g_VerticesMesh;
	CUSTOMVERTEXNORMALTEXTURA *Geom;
	unsigned short *g_IndicesMesh, *g_TIndicesMesh;
	unsigned long numBytes;

	// Nuevo codigo para la lectura de multitextura
	// Vamos a crear una mesh por cada textura existente

	std::vector<int>				IndexMaterial;

	std::vector<D3DXVECTOR3>				VertexBuffer;
	std::vector<D3DXVECTOR3>				CVertexBuffer;

	std::vector< std::vector<D3DXVECTOR3> >	VertexBufferByMat;
	std::vector< std::vector<D3DXVECTOR3> >	IndexBufferByMat;

	std::vector< std::vector<D3DXVECTOR3> >	CVertexBufferByMat;
	std::vector< std::vector<D3DXVECTOR3> >	CIndexBufferByMat;

	std::map< int, int >					VertexIndexOldNew;
	std::map< int, int >					CVertexIndexOldNew;

	char *texto, direc[512];

	text fich(nombreFichero);
	unsigned short material;
	unsigned int iniObjeto, finObjeto;
	std::string auxTextura;
	bool l_BeginLoad=true;

	//CAPTURAMOS TEXTURAS COMUNES PARA TODOS LOS OBJETOS
	int num_texture=0;
	while(fich.seek("*BITMAP"))
	{	
		strcpy(direc,"");
		bool l_Continue=true;
		do
		{
			int pos;
			texto=fich.getword();
			if(texto!=NULL)
			{
				if(texto[0]=='"') pos=1;
				else pos=0;
				strcat(direc,(char *)&texto[pos]);
				strcat(direc," ");

			}
			else break;
			l_Continue=texto[strlen(texto)-1]!='"';
			delete texto;
		}while(l_Continue);
		LPDIRECT3DBASETEXTURE9 texture=NULL;
		if(texto!=NULL)
		{
			direc[strlen(direc)-2]='\0';
			auxTextura=direc;
			listaTexturas.push_back(auxTextura);
			numTexturas++;
			texture = CASETextureManager::GetInstance()->LoadTexture(direc,g_pd3dDevice);
			vec_textures.push_back(texture);
		}
		++num_texture;

		std::vector<D3DXVECTOR3> new_vertexBuffer;
		VertexBufferByMat.push_back( new_vertexBuffer );
		std::vector<D3DXVECTOR3> new_CvertexBuffer;
		CVertexBufferByMat.push_back( new_CvertexBuffer );

		std::vector<D3DXVECTOR3> new_indexBuffer;
		IndexBufferByMat.push_back(  new_indexBuffer );	
		std::vector<D3DXVECTOR3> new_CindexBuffer;
		CIndexBufferByMat.push_back( new_CindexBuffer );
	}
	fich.reset();
	//FIN CAPTURAMOS TEXTURAS COMUNES PARA TODOS LOS OBJETOS

	//BUSCAMOS PRINCIPIO OBJETO
	do
	{
		if(!fich.seek("*GEOMOBJECT")) return FALSE;
		if(!fich.seek("*NODE_NAME")) return FALSE;
		texto=fich.getword();
	}while(nombreObjeto!=NULL && strcmp(texto,nombreObjeto)!=0);
	delete texto;

	iniObjeto=fich.getPos();
	//FIN BUSCAMOS PRINCIPIO OBJETO
	//BUSCAMOS FIN OBJETO
	fich.seek("*GEOMOBJECT");
	finObjeto=fich.getPos();
	fich.setPos(iniObjeto);
	//FIN BUSCAMOS FIN OBJETO
	if(fich.seek("*MESH_NUMVERTEX"))
	{
		numVertices=fich.getint();
	}
	else fich.setPos(iniObjeto);

	if(fich.seek("*MESH_NUMFACES"))
	{
		numCaras=fich.getint();
	}
	else fich.setPos(iniObjeto);

	for(unsigned int iv=0;iv<numVertices;iv++)
	{
		fich.seek("*MESH_VERTEX");
		int bas=fich.getint();
		float x=(float)fich.getfloat();
		float y=(float)fich.getfloat();
		float z=(float)fich.getfloat();

		D3DXVECTOR3 new_vertex;
		new_vertex.x = x;
		new_vertex.y = z;
		new_vertex.z = y;

		VertexBuffer.push_back(new_vertex);

		//calcular caja contenedora
		if(l_BeginLoad)
		{
			CCmin=D3DXVECTOR3(new_vertex.x,new_vertex.y,new_vertex.z);
			CCmax=D3DXVECTOR3(new_vertex.x,new_vertex.y,new_vertex.z);
			l_BeginLoad=false;
		}
		if(CCmin.x>new_vertex.x) 	CCmin.x=new_vertex.x;
		if(CCmin.y>new_vertex.y) 	CCmin.y=new_vertex.y;
		if(CCmin.z>new_vertex.z) 	CCmin.z=new_vertex.z;

		if(CCmax.x<new_vertex.x)	CCmax.x=new_vertex.x;
		if(CCmax.y<new_vertex.y) 	CCmax.y=new_vertex.y;
		if(CCmax.z<new_vertex.z) 	CCmax.z=new_vertex.z;
		//fin caja contenedora
	}
	pos=(CCmax+CCmin)/2.0f;
	m_Radius=0.0f;
	for(unsigned int iv=0;iv<numVertices;iv++)
	{
		float l_Radius=LENGTH(pos,VertexBuffer[iv]);
		if(l_Radius>m_Radius)
			m_Radius=l_Radius;
	}
	std::map<size_t, size_t> l_MapFaceId;
	for(unsigned int ic=0;ic<numCaras;ic++)
	{
		D3DXVECTOR3 index_face;
		fich.seek("*MESH_FACE");
		fich.seek("A:");
		index_face.x  = (float)fich.getint();
		fich.seek("B:");
		index_face.y = (float)fich.getint();
		fich.seek("C:");
		index_face.z = (float)fich.getint();
		fich.seek("*MESH_MTLID"); 
		material = fich.getint();
		if(IndexBufferByMat.size()==1)
			material=0;
		if(ic == 3467)
			index_face.x=2312;
		l_MapFaceId[material*numCaras+IndexBufferByMat[material].size()]=ic;

		IndexBufferByMat[material].push_back( index_face );
		IndexMaterial.push_back( material );
	}

	if(fich.seek("*MESH_NUMTVERTEX"))
	{
		numTVertices=fich.getint();
	}
	else fich.setPos(iniObjeto);

	for(unsigned int iv=0;iv<numTVertices;iv++)
	{
		D3DXVECTOR3 new_tVvertex;
		fich.seek("*MESH_TVERT");
		unsigned int bas=fich.getint();
		new_tVvertex.x=(float)fich.getfloat();
		new_tVvertex.y=(float)fich.getfloat();
		new_tVvertex.z = 0;
		//lo tenemos que cargar asi, pq z es arriba para el max y x esta invertido por el max
		new_tVvertex.y = 1 - new_tVvertex.y;

		CVertexBuffer.push_back(new_tVvertex);
	}

	if(fich.seek("*MESH_NUMTVFACES"))
	{
		numTCaras=fich.getint();
	}
	else fich.setPos(iniObjeto);
	for(unsigned int ic=0;ic<numTCaras;ic++)
	{
		D3DXVECTOR3 index_tFace;
		fich.seek("*MESH_TFACE");
		unsigned int bas=fich.getint();
		index_tFace.x=(float)fich.getint();
		index_tFace.y=(float)fich.getint();
		index_tFace.z=(float)fich.getint();

		int mat = IndexMaterial[ ic ];
		CIndexBufferByMat[mat].push_back( index_tFace );
	}

	float *l_Normals=new float[9*numCaras];
	memset(l_Normals,0,sizeof(float)*3*numCaras);

	if(fich.seek("*MESH_NORMALS"))
	{
		for(unsigned int ic=0;ic<numCaras;ic++)
		{
			/*D3DXVECTOR3 index_face;		
			fich.seek("*MESH_FACENORMAL");
			fich.getint();
			float nx=fich.getfloat();
			float nz=fich.getfloat();
			float ny=fich.getfloat();
			D3DXVECTOR3 l_Normal(-nx,ny,-nz);
			D3DXVec3Normalize(&l_Normal,&l_Normal);
			for(size_t t=0;t<3;++t)
			{
			l_Normals[ic*9+t*3]=l_Normal.x;
			l_Normals[ic*9+t*3+1]=l_Normal.y;
			l_Normals[ic*9+t*3+2]=l_Normal.z;
			}*/
			for(size_t t=0;t<3;++t)
			{
				fich.seek("*MESH_VERTEXNORMAL");
				fich.getint();
				float nx= (float)fich.getfloat();
				float nz= (float)fich.getfloat();
				float ny= (float)fich.getfloat();
				D3DXVECTOR3 l_Normal(nx,ny,nz);
				//D3DXVec3Normalize(&l_Normal,&l_Normal);
				l_Normals[ic*9+t*3]=l_Normal.x;
				l_Normals[ic*9+t*3+1]=l_Normal.y;
				l_Normals[ic*9+t*3+2]=l_Normal.z;
			}
		}
		/*index_face.x  = (float)fich.getint();
		fich.seek("B:");
		index_face.y = (float)fich.getint();
		fich.seek("C:");
		index_face.z = (float)fich.getint();
		fich.seek("*MESH_MTLID"); 
		material = fich.getint();
		if(IndexBufferByMat.size()==1)
		material=0;

		IndexBufferByMat[material].push_back( index_face );
		IndexMaterial.push_back( material );*/
	}

	std::map<int,int>::iterator it_VertexIndexOldNew;
	typedef std::pair<int,int> Pair_IndexOldNew;
	//Ahora vamos a reasignar los vertices geometricos y de color por material
	for(int cont_texture = 0; cont_texture < num_texture; cont_texture++)
	{
		int contador_vertices = 0;
		int contador_tvertices = 0;
		VertexIndexOldNew.clear();
		CVertexIndexOldNew.clear();
		//Recorremos todas las caras geometricas y de textura
		for(size_t cont_face = 0; cont_face < IndexBufferByMat[cont_texture].size(); cont_face++)
		{
			D3DXVECTOR3 face_geom = IndexBufferByMat[cont_texture][cont_face];
			for(int j=0;j<3;j++)
			{
				it_VertexIndexOldNew = VertexIndexOldNew.find((int)face_geom[j]);
				if(it_VertexIndexOldNew != VertexIndexOldNew.end())
				{
					face_geom[j] = (float)it_VertexIndexOldNew->second;
				}
				else
				{
					VertexIndexOldNew.insert( Pair_IndexOldNew((int)face_geom[j], contador_vertices ) );
					VertexBufferByMat[cont_texture].push_back( VertexBuffer[(int)face_geom[j]] );
					face_geom[j] = (float)contador_vertices;
					contador_vertices++;
				}
			}

			D3DXVECTOR3 face_texture = CIndexBufferByMat[cont_texture][cont_face];
			for(int j=0;j<3;j++)
			{

				it_VertexIndexOldNew = CVertexIndexOldNew.find((int)face_texture[j]);
				if(it_VertexIndexOldNew != CVertexIndexOldNew.end())
				{
					face_texture[j]= (float)it_VertexIndexOldNew->second;
				}
				else
				{
					CVertexIndexOldNew.insert( Pair_IndexOldNew((int)face_texture[j], contador_tvertices ) );
					CVertexBufferByMat[cont_texture].push_back( CVertexBuffer[(int)face_texture[j]] );
					face_texture[j] = (float)contador_tvertices;
					contador_tvertices++;
				}
			}
			IndexBufferByMat[cont_texture][cont_face] = face_geom;
			CIndexBufferByMat[cont_texture][cont_face] = face_texture;

		}
	}
	CVertexBuffer.clear();
	VertexBuffer.clear();



	//Creamos una mesh para cada textura
	for(int cont_texture = 0; cont_texture < num_texture; cont_texture++)
	{
		unsigned int numVertices = (unsigned int) VertexBufferByMat[cont_texture].size();
		unsigned int numCaras = (unsigned int) IndexBufferByMat[cont_texture].size();
		unsigned int numTVertices = (unsigned int) CVertexBufferByMat[cont_texture].size();
		unsigned int numTCaras = (unsigned int) CIndexBufferByMat[cont_texture].size();

		vec_numCaresByMat.push_back((int)numCaras);

		//inicilizamos en memoria la matriz de vertices e indices
		g_VerticesMesh=new CUSTOMVERTEX[numVertices];
		g_IndicesMesh=new unsigned short[numCaras*3];
		for(size_t iv=0;iv<numVertices;iv++)
		{
			D3DXVECTOR3 vertex = VertexBufferByMat[cont_texture][iv];
			switch(iv%3)
			{
			case 0:
				g_VerticesMesh[iv].color=0xffffffff;
				break;
			case 1:
				g_VerticesMesh[iv].color=0xff0000ff;
				break;
			case 2:
				g_VerticesMesh[iv].color=0xff00ffff;
				break;
			}
			g_VerticesMesh[iv].x=vertex.x;
			g_VerticesMesh[iv].y=vertex.y;
			g_VerticesMesh[iv].z=vertex.z;
		}
		for(size_t ic=0;ic<numCaras;ic++)
		{
			D3DXVECTOR3 face = IndexBufferByMat[cont_texture][ic];
			g_IndicesMesh[ic*3]=(int)face.x;
			g_IndicesMesh[1+ic*3]=(int)face.y;
			g_IndicesMesh[2+ic*3]=(int)face.z;
		}
		LPDIRECT3DVERTEXBUFFER9 pVBMesh=NULL;
		if(numCaras != 0)
		{
			if( FAILED( g_pd3dDevice->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*(unsigned int)numVertices,0, D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &pVBMesh, NULL ) ) )
			{
				return false;
			}

			if( FAILED( pVBMesh->Lock( 0, sizeof(CUSTOMVERTEX)*(unsigned int)numVertices, (void**)&pMesh, 0 ) ) )
			{
				return false; 
			}
			memcpy( pMesh, g_VerticesMesh, sizeof(CUSTOMVERTEX)*(unsigned int)numVertices );
			pVBMesh->Unlock();
		}
		vec_pVBMeshByMat.push_back(pVBMesh);

		numBytes=numCaras*3*sizeof(unsigned short);
		LPDIRECT3DINDEXBUFFER9 pIBMesh=NULL;
		if(numCaras != 0)
		{
			if( FAILED( g_pd3dDevice->CreateIndexBuffer( numBytes,0, D3DFMT_INDEX16,D3DPOOL_DEFAULT, &pIBMesh, NULL ) ) )
			{
				return false;
			}
			if( FAILED( pIBMesh->Lock( 0, numBytes, (void**)&pMeshIndices, 0 ) ) )
			{
				return false; 
			}
			memcpy( pMeshIndices, g_IndicesMesh, numBytes);
			pIBMesh->Unlock();
		}
		vec_pIBMeshByMat.push_back(pIBMesh);

		g_VerticesTextura = new CoordsText[numTVertices];
		for(unsigned int iv=0;iv<numTVertices;iv++)
		{
			D3DXVECTOR3 texture_vertex = CVertexBufferByMat[cont_texture][iv];
			//lo tenemos que cargar asi, pq z es arriba para el max y x esta invertido por el max
			g_VerticesTextura[iv].u = texture_vertex.x;
			g_VerticesTextura[iv].v = texture_vertex.y;
		}

		g_TIndicesMesh = new unsigned short[numTCaras*3];
		for(unsigned int ic=0;ic<numTCaras;ic++)
		{
			D3DXVECTOR3 texture_face = CIndexBufferByMat[cont_texture][ic];
			g_TIndicesMesh[ic*3]= (int)texture_face.x;
			g_TIndicesMesh[1+ic*3]=(int)texture_face.y;
			g_TIndicesMesh[2+ic*3]=(int)texture_face.z;
		}



		Geom=new CUSTOMVERTEXNORMALTEXTURA [numCaras*3];
		for(unsigned int iv=0;iv<numCaras;iv++)
		{
			int vid1, vid2, vid3,tvid1, tvid2, tvid3;
			D3DXVECTOR3 pos1,pos2,pos3;

			vid3=g_IndicesMesh[3*iv];
			vid2=g_IndicesMesh[3*iv+1];
			vid1=g_IndicesMesh[3*iv+2];
			tvid3=g_TIndicesMesh[3*iv];
			tvid2=g_TIndicesMesh[3*iv+1];
			tvid1=g_TIndicesMesh[3*iv+2];
			//COPIA COORDENADAS DE VERTICES 
			pos1.x = Geom[iv*3].x = g_VerticesMesh[vid1].x;
			pos1.y = Geom[iv*3].y = g_VerticesMesh[vid1].y;
			pos1.z = Geom[iv*3].z = g_VerticesMesh[vid1].z;

			pos2.x = Geom[iv*3+1].x = g_VerticesMesh[vid2].x;
			pos2.y = Geom[iv*3+1].y = g_VerticesMesh[vid2].y;
			pos2.z = Geom[iv*3+1].z = g_VerticesMesh[vid2].z;

			pos3.x = Geom[iv*3+2].x = g_VerticesMesh[vid3].x;
			pos3.y = Geom[iv*3+2].y = g_VerticesMesh[vid3].y;
			pos3.z = Geom[iv*3+2].z = g_VerticesMesh[vid3].z;

			//COPIA COORDENADAS DE TEXTURAS
			Geom[iv*3].u=g_VerticesTextura[tvid1].u;
			Geom[iv*3].v=g_VerticesTextura[tvid1].v;
			D3DXVECTOR3 face = IndexBufferByMat[cont_texture][iv];
			size_t l_Face=l_MapFaceId[cont_texture*this->numCaras+iv];


			Geom[iv*3+2].nx=l_Normals[l_Face*9+0];
			Geom[iv*3+2].ny=l_Normals[l_Face*9+1];
			Geom[iv*3+2].nz=l_Normals[l_Face*9+2];
			Geom[iv*3+1].nx=l_Normals[l_Face*9+3];
			Geom[iv*3+1].ny=l_Normals[l_Face*9+4];
			Geom[iv*3+1].nz=l_Normals[l_Face*9+5];
			Geom[iv*3+0].nx=l_Normals[l_Face*9+6];
			Geom[iv*3+0].ny=l_Normals[l_Face*9+7];
			Geom[iv*3+0].nz=l_Normals[l_Face*9+8];

			/*Geom[iv*3+2].nx=l_Normals[vid3*3];
			Geom[iv*3+2].ny=l_Normals[vid3*3+1];
			Geom[iv*3+2].nz=l_Normals[vid3*3+2];
			Geom[iv*3+1].nx=l_Normals[vid2*3];
			Geom[iv*3+1].ny=l_Normals[vid2*3+1];
			Geom[iv*3+1].nz=l_Normals[vid2*3+2];
			Geom[iv*3+0].nx=l_Normals[vid1*3];
			Geom[iv*3+0].ny=l_Normals[vid1*3+1];
			Geom[iv*3+0].nz=l_Normals[vid1*3+2];*/

			Geom[iv*3+1].u=g_VerticesTextura[tvid2].u;
			Geom[iv*3+1].v=g_VerticesTextura[tvid2].v;



			Geom[iv*3+2].u=g_VerticesTextura[tvid3].u;
			Geom[iv*3+2].v=g_VerticesTextura[tvid3].v;


			/*Geom[iv*3].color=0xffffffff;
			Geom[iv*3+1].color=0xffffffff;
			Geom[iv*3+2].color=0xffffffff;*/
		}
		LPDIRECT3DVERTEXBUFFER9 pVBGeomTextura=NULL;
		if(numCaras != 0)
		{
			if( FAILED( g_pd3dDevice->CreateVertexBuffer( sizeof(CUSTOMVERTEXNORMALTEXTURA)*numCaras*3,
				0, D3DFVF_CUSTOMVERTEXNORMALTEXTURA,
				D3DPOOL_DEFAULT, &pVBGeomTextura, NULL ) ) )
			{
				return false;
			}

			if( FAILED( pVBGeomTextura->Lock( 0, sizeof(CUSTOMVERTEXNORMALTEXTURA)*numCaras*3, (void**)&pMesh, 0 ) ) )
				return false; 
			memcpy( pMesh, Geom, sizeof(CUSTOMVERTEXNORMALTEXTURA)*numCaras*3 );
			pVBGeomTextura->Unlock();
		}
		vec_pVBGeomTexturaByMat.push_back(pVBGeomTextura);

		delete g_IndicesMesh;
		delete g_VerticesMesh;
		delete g_TIndicesMesh;
		delete g_VerticesTextura;
		delete Geom;
	}
	delete l_Normals;
		

	return true;
}

LPDIRECT3DVERTEXDECLARATION9 CASEObject::m_pASEVertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9 CASEObject::GetASEVertexDeclaration(LPDIRECT3DDEVICE9 Device)
{
	if(m_pASEVertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 decl[] = 
		{
			{ 0, 0 ,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};
		Device->CreateVertexDeclaration( decl, &m_pASEVertexDeclaration );
	}
	return m_pASEVertexDeclaration;
}

void CASEObject::ReleaseVertexDeclaration()
{
	CHECKED_RELEASE(m_pASEVertexDeclaration);
}


void CASEObject::Render(CRenderManager* manager)
{
	LPDIRECT3DDEVICE9 g_pd3dDevice = manager->GetDevice();
	for(unsigned int cont = 0; cont < vec_textures.size(); cont++)
	{
		if(vec_numCaresByMat[cont] != 0)
		{
			
			g_pd3dDevice->SetStreamSource( 0, vec_pVBGeomTexturaByMat[cont], 0, sizeof(CUSTOMVERTEXNORMALTEXTURA) );
			g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEXNORMALTEXTURA);
			g_pd3dDevice->SetTexture (0, vec_textures[cont]);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,vec_numCaresByMat[cont]);

		}
	}
	g_pd3dDevice->SetTexture (0, NULL);
}


void CASEObject::CleanUp()
{
	vec_textures.clear();
	vec_numCaresByMat.clear();
	listaTexturas.clear();
	for(size_t cont = 0; cont < vec_textures.size(); cont++)
	{
		if(vec_numCaresByMat[cont] != 0)
		{
			CHECKED_RELEASE(vec_pVBGeomTexturaByMat[cont]);
			CHECKED_RELEASE(vec_pIBMeshByMat[cont]);
			CHECKED_RELEASE(vec_pVBMeshByMat[cont]);
		}		
	}
}

bool CASEObject::IsInBoundingBox(const Vect3f &Pos)
{
	return Pos.x>=CCmin.x && Pos.x<=CCmax.x
		&& Pos.y>=CCmin.y && Pos.y<=CCmax.y
		&& Pos.z>=CCmin.z && Pos.z<=CCmax.z;
}
