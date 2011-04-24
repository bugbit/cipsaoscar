#ifndef RENDERABLE_VERTEXS_H
#define RENDERABLE_VERTEXS_H

class CRenderableVertexs
{
protected:
	LPDIRECT3DVERTEXBUFFER9				m_VB;
	LPDIRECT3DINDEXBUFFER9				m_IB;
	size_t								m_IndexCount, m_VertexCount;
public:
	CRenderableVertexs() : m_VB(NULL), m_IB(NULL), m_IndexCount(0), m_VertexCount(0) {}
	virtual ~CRenderableVertexs() {}
	virtual bool Render(CRenderManager *RM) const = 0;
	virtual bool Render(CRenderManager *RM, CEffect *Effect, const std::string &TechniqueName) const = 0;
	virtual inline size_t GetFacesCount() const {return m_IndexCount/3;}
	virtual inline size_t GetVertexsCount() const {return m_VertexCount;}
	virtual inline unsigned short GetVertexType() const = 0;
	virtual void GetFaces(void **Faces)
	{
		if(m_IB)
		{
			void *l_DataAddress;
			*Faces=malloc(GetIndexSize()*m_IndexCount);
			m_IB->Lock(0,(UINT)GetIndexSize()*m_IndexCount,&l_DataAddress,0);
			memcpy(*Faces,l_DataAddress,GetIndexSize()*m_IndexCount);
			m_IB->Unlock();
		}
	}
	virtual void GetVertexs(void **Vtxs)
	{	
		if(m_VB)
		{
			void *l_DataAddress;
			*Vtxs=malloc(GetVertexSize()*m_VertexCount);
			m_VB->Lock(0,(UINT)GetVertexSize()*m_VertexCount,&l_DataAddress,0);
			memcpy(*Vtxs,l_DataAddress,GetVertexSize()*m_VertexCount);
			m_VB->Unlock();
		}
	}
	virtual inline size_t GetVertexSize() = 0;
	virtual inline size_t GetIndexSize() = 0;
};

template<class T>
class CIndexedVertexs : public CRenderableVertexs
{
protected:
	inline size_t GetVertexSize() {return sizeof(T);}
	inline size_t GetIndexSize() {return sizeof(unsigned short);}
public:
	CIndexedVertexs(CRenderManager *RM, void *VertexAddress, void *IndexAddres, size_t VertexCount, size_t IndexCount)
	{
		m_IndexCount=IndexCount;
		m_VertexCount=VertexCount;
		assert(RM);
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		if(l_Device->CreateVertexBuffer((UINT)VertexCount*sizeof(T),0,T::GetFVF(),D3DPOOL_DEFAULT,&m_VB,0)==D3D_OK)
		{
			void *l_VBAddress=NULL;
			m_VB->Lock(0,(UINT)VertexCount*sizeof(T),&l_VBAddress,0);
			memcpy(l_VBAddress,VertexAddress,VertexCount*sizeof(T));
			m_VB->Unlock();
		}
		else
			return;

		if(l_Device->CreateIndexBuffer((UINT)IndexCount*sizeof(unsigned short),0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_IB,NULL)==D3D_OK)
		{
			void *l_IBAddress=NULL;
			m_IB->Lock(0,(UINT)IndexCount*sizeof(unsigned short),&l_IBAddress,0);
			memcpy(l_IBAddress,IndexAddres,IndexCount*sizeof(unsigned short));
			m_IB->Unlock();
		}
		else
			return;
	}
	~CIndexedVertexs()
	{
		CHECKED_RELEASE(m_IB);
		CHECKED_RELEASE(m_VB);
		m_IndexCount=0;
		m_VertexCount=0;
	}

	bool Render(CRenderManager *RM) const
	{
		assert(RM);
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		l_Device->SetFVF(T::GetFVF());
		l_Device->SetStreamSource(0,m_VB,0,sizeof(T));
		l_Device->SetIndices(m_IB);
		l_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,(UINT)m_VertexCount,0,(UINT)m_IndexCount/3);
		return true;
	}
	bool Render(CRenderManager *RM, CEffect *Effect, const std::string &TechniqueName) const
	{
		assert(RM);
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		UINT l_NumPasses;
		Effect->SetTechnique("Main");
		if(SUCCEEDED(Effect->GetD3DEffect()->Begin(&l_NumPasses,0)))
		{
			l_Device->SetVertexDeclaration(T::GetVertexDeclaration(RM));
			l_Device->SetStreamSource(0,m_VB,0,sizeof(T));
			l_Device->SetIndices(m_IB);
			for (UINT b=0;b<l_NumPasses;++b)
			{
				Effect->GetD3DEffect()->BeginPass(b);
				l_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,(UINT)m_VertexCount,0,(UINT)m_IndexCount/3);
				Effect->GetD3DEffect()->EndPass();
			}
			Effect->GetD3DEffect()->End();
		}
		return true;
	}
	virtual inline unsigned short GetVertexType() const {return T::GetVertexType();}
};


#endif