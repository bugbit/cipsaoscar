#include "__PCH_Graphics.h"
#include "Materials/effectmanager.h"
#include "Materials/effect.h"
#include "Core/Core.h"
#include "RenderManager.h"
#include <assert.h>
#include "VertexType/RenderableVertexs.h"
#include "VertexType/VertexType.h"
#include "Mesh/StaticMesh.h"
#include "Textures/Texture.h"
#include "Textures/TextureManager.h"

#define STATIC_MESH_HEADER 0x5555

CStaticMesh::CStaticMesh()
: m_FileName("")
, m_NumVertexs(0)
, m_NumFaces(0)
{
}

CStaticMesh::~CStaticMesh()
{
	for(size_t b=0;b<m_RVs.size();++b)
	{
		CHECKED_DELETE(m_RVs[b]);
	}
}

bool CStaticMesh::Load(const std::string &FileName)
{
	FILE *l_File=NULL;
	fopen_s(&l_File,FileName.c_str(),"rb");
	if(l_File)
	{
		uint16 l_Header;
		fread(&l_Header,sizeof(uint16),1,l_File);		//Header
		if(l_Header!=STATIC_MESH_HEADER)
		{
			fclose(l_File);
			return false;
		}
		fread(&m_NumVertexs,sizeof(uint32),1,l_File);
		fread(&m_NumFaces,sizeof(uint32),1,l_File);
		uint16 l_NumRenderableVertexs;
		fread(&l_NumRenderableVertexs,sizeof(uint16),1,l_File);
		m_Textures.resize(l_NumRenderableVertexs);
		for(size_t b=0;b<l_NumRenderableVertexs;++b)
		{
			uint16 l_NumTextures;
			fread(&l_NumTextures,sizeof(uint16),1,l_File);
			for(size_t t=0;t<l_NumTextures;++t)
			{
				std::string l_TextureName;
				uint16 l_TextureNameSize;
				fread(&l_TextureNameSize,sizeof(uint16),1,l_File);
				l_TextureName.resize(l_TextureNameSize);
				fread(&l_TextureName[0],sizeof(char),l_TextureNameSize,l_File);
				l_TextureName.push_back(0);
				CTexture *l_Texture=CCore::GetSingleton().GetTextureManager()->GetTexture(l_TextureName);
				m_Textures[b].push_back(l_Texture);
			}

			uint16 l_VertexType;
			void *l_Faces=NULL;
			void *l_Vtxs=NULL;

			fread(&l_VertexType,sizeof(uint16),1,l_File);

			uint32 l_NumVertexs=0;
			uint32 l_NumFaces=0;
			uint32 l_VertexSize=0, l_IndexSize=0;
			fread(&l_NumVertexs,sizeof(uint32),1,l_File);
			fread(&l_NumFaces,sizeof(uint32),1,l_File);
			if(l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType())
			{
				l_VertexSize=sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX);
				l_IndexSize=sizeof(uint16);
			}
			else
			{
				fclose(l_File);
				return false;
			}

			if(l_NumVertexs!=0)
			{
				l_Vtxs=malloc(l_NumVertexs*l_VertexSize);
				assert(l_Vtxs!=NULL);
				fread(l_Vtxs,l_NumVertexs*l_VertexSize,1,l_File);
			}

			if(l_NumFaces!=0)
			{
				l_Faces=malloc(l_NumFaces*l_IndexSize*3);
				assert(l_Faces!=NULL);
				fread(l_Faces,l_NumFaces*l_IndexSize*3,1,l_File);
			}
			CRenderableVertexs *l_RV=NULL;
			CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();
			if(l_VertexType==TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType())
			{
				CalcTangentsAndBinormals(l_Vtxs,(uint16 *)l_Faces,l_NumVertexs,l_NumFaces*3,l_VertexSize,0,12,28,44,60);
				l_RV=new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(l_RM,l_Vtxs,l_Faces,l_NumVertexs,l_NumFaces*3);
			}
			assert(l_RV!=NULL);
			m_RVs.push_back(l_RV);
			free(l_Vtxs);
			free(l_Faces);
		}

		fread(&l_Header,sizeof(uint16),1,l_File);		//Sub-Header
		if(l_Header!=STATIC_MESH_HEADER)
		{
			fclose(l_File);
			return false;
		}
		fclose(l_File);
		return true;
	}
	return false;
}

bool CStaticMesh::GenMesh(const std::string &FileName)
{
#define QUAD_SIZE 16.0f

	uint16 l_Idx[6]={0,1,2,1,2,3};
	TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX l_Vtxs[4]={
		{-QUAD_SIZE,QUAD_SIZE,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f},
		{-QUAD_SIZE,-QUAD_SIZE,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
		{QUAD_SIZE,QUAD_SIZE,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f},
		{QUAD_SIZE,-QUAD_SIZE,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f},
	};
	CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();
	CalcTangentsAndBinormals(l_Vtxs,l_Idx,4,6,sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX),0,12,28,44,60);
	m_RVs.push_back(new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(l_RM,l_Vtxs,l_Idx,4,6));
	m_FileName=FileName;

	std::vector<CTexture*> l_Vect;


	l_Vect.push_back(CCore::GetSingleton().GetTextureManager()->GetTexture("./Data/Textures/diffuse_piedras.jpg"));
	l_Vect.push_back(CCore::GetSingleton().GetTextureManager()->GetTexture("./Data/Textures/normal_piedras.jpg"));
	l_Vect.push_back(CCore::GetSingleton().GetTextureManager()->GetTexture("./Data/Textures/Default_reflection.dds"));

	m_Textures.push_back(l_Vect);

	m_NumVertexs=4;
	m_NumFaces=2;

	return Save(FileName);
}

bool CStaticMesh::Save(const std::string &FileName)
{
	uint16 l_Header=STATIC_MESH_HEADER;
	FILE *l_File=NULL;
	fopen_s(&l_File,FileName.c_str(),"wb+");
	if(l_File)
	{
		fwrite(&l_Header,sizeof(uint16),1,l_File);		//Header
		fwrite(&m_NumVertexs,sizeof(uint32),1,l_File);
		fwrite(&m_NumFaces,sizeof(uint32),1,l_File);
		uint16 l_NumRenderableVertexs=(uint16)m_RVs.size();
		fwrite(&l_NumRenderableVertexs,sizeof(uint16),1,l_File);

		for(size_t b=0;b<m_RVs.size();++b)
		{
			uint16 l_NumTextures=(uint16)m_Textures[b].size();
			fwrite(&l_NumTextures,sizeof(uint16),1,l_File);
			for(size_t t=0;t<m_Textures[b].size();++t)
			{
				const std::string &l_TextureName=m_Textures[b][t]->GetFileName();
				uint16 l_TextureNameSize=(uint16)l_TextureName.size();
				fwrite(&l_TextureNameSize,sizeof(uint16),1,l_File);
				fwrite(&l_TextureName.c_str()[0],sizeof(char),l_TextureNameSize,l_File);
			}

			uint16 l_VertexType=m_RVs[b]->GetVertexType();
			void *l_Faces=NULL;
			void *l_Vtxs=NULL;

			fwrite(&l_VertexType,sizeof(uint16),1,l_File);

			uint32 l_NumVertexs=(uint32)m_RVs[b]->GetVertexsCount();
			uint32 l_NumFaces=(uint32)m_RVs[b]->GetFacesCount();
			fwrite(&l_NumVertexs,sizeof(uint32),1,l_File);
			fwrite(&l_NumFaces,sizeof(uint32),1,l_File);
			if(l_NumVertexs!=0)
			{
				m_RVs[b]->GetVertexs(&l_Vtxs);
				assert(l_Vtxs!=NULL);
				fwrite(l_Vtxs,m_RVs[b]->GetVertexSize()*m_RVs[b]->GetVertexsCount(),1,l_File);
				free(l_Vtxs);
			}

			if(l_NumFaces!=0)
			{
				m_RVs[b]->GetFaces(&l_Faces);
				assert(l_Faces!=NULL);
				fwrite(l_Faces,m_RVs[b]->GetIndexSize()*m_RVs[b]->GetFacesCount()*3,1,l_File);
				free(l_Faces);
			}
		}

		fwrite(&l_Header,sizeof(uint16),1,l_File);		//Sub-Header
		fclose(l_File);
		return true;
	}
	return false;
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	CEffect *l_Effect=CCore::GetSingleton().GetEffectManager()->GetEffect("normalmapeffect");
	if(l_Effect==NULL)
		return;

	CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();

	LPDIRECT3DDEVICE9 l_Device=l_RM->GetDevice();

	D3DXMATRIX l_MatWorld, l_MatView, l_MatProjection;
	l_Device->GetTransform(D3DTS_PROJECTION,&l_MatProjection);
	l_Device->GetTransform(D3DTS_VIEW,&l_MatView);
	l_Device->GetTransform(D3DTS_WORLD,&l_MatWorld);

	l_Effect->BeginRender(l_MatView,l_MatProjection);
	l_Effect->SetWorldMatrix(l_MatWorld);

	for(size_t b=0;b<m_RVs.size();++b)
	{
		for(size_t t=0;t<m_Textures[b].size();++t)
		{
			if(m_Textures[b][t])
				m_Textures[b][t]->Activate(t);
		}
		m_RVs[b]->Render(RM,l_Effect,"Main");
	}
}
