#include "__PCH_Quake.h"

#include "ItemManager.h"
#include "itemmodel.h"
#include "Item.h"
#include "ItemModelASE.h"

//---Engine Includes--------
#include "Graphics/RenderManager.h"
#include "Math/Matrix44.h"
#include "PhysX/PhysicActor.h"
#include "Logger/Logger.h"
#include "xml/XMLTreeNode.h"

CItemManager::CItemManager(void)
:m_bIsOk(false)
,m_fRotationY(0.f)
{
}

CItemManager::~CItemManager(void)
{
	Done();
}

void CItemManager::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

bool CItemManager::Init()
{
	m_fRotationY=0.f;
	m_bIsOk=true;

	return m_bIsOk;
}

void CItemManager::Release()
{
	ReleaseItems();
	ReleaseModels();
}

void CItemManager::ReleaseModels()
{
	std::map<CItem::ETYTE,CItemModel *>::iterator it=m_Models.begin(),itend=m_Models.end();
	
	for(;it!=itend;it++)
	{
		CItemModel *itemmanager=(*it).second;
		CHECKED_DELETE(itemmanager);
	}
	m_Models.clear();
}

void CItemManager::ReleaseItems()
{
	std::vector<CItem *>::iterator it=m_Items.begin(),itend=m_Items.end();
	
	for(;it!=itend;it++)
	{
		CItem *item=*it;
		CHECKED_DELETE(item);
	}
	m_Items.clear();
}

bool CItemManager::LoadModelsXml(std::string filexml)
{
	m_sModelXML=filexml;
	LOGGER->AddNewLog(ELL_INFORMATION, "CItemManager::LoadModelsXml: Iniciando el parseo del fichero %s", filexml.c_str());
	CXMLTreeNode newFile;
	m_bIsOk = false;
	if (!newFile.LoadFile((filexml).c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CItemManager::LoadModelsXml: No se ha podido leer correctamente el fichero ->%s", filexml.c_str());
		m_bIsOk = false;
	}
	else
	{
		CXMLTreeNode itemsmodels = newFile["itemsmodels"];
		int count=itemsmodels.GetNumChildren();
		for (int i=0;i<count;i++)
		{
			CXMLTreeNode model_node=itemsmodels(i);	
			std::string name=model_node.GetName();
			CItem::ETYTE type=CItemTypeManager::GetInstance().GetTypeForName(name);
			if (type!=CItem::NONE)
			{
				std::map<CItem::ETYTE,CItemModel *>::iterator itm=m_Models.find(type);				
				bool crear_model=itm==m_Models.end();
				CItemModel *itemmodel;
				if (crear_model)
					itemmodel=CreateItemModel(model_node);
				else
				{
					itemmodel=(*itm).second;
					itemmodel->Done();
					itemmodel->Init();
				}
				LoadNodeItemModel(itemmodel,model_node);
				if (crear_model)
					AddModel(type,itemmodel);
				LOGGER->AddNewLog(ELL_INFORMATION, "CItemManager::LoadModelsXml: Cargado item %s",name.c_str());
			}
			else
				LOGGER->AddNewLog(ELL_INFORMATION, "CItemManager::LoadModelsXml: item %s no reconocido",name.c_str());
		}
	}
	return true;
}

CItemModel * CItemManager::CreateItemModel(CXMLTreeNode &node)
{
	return new CItemModelASE();
}

void CItemManager::LoadNodeItemModel(CItemModel *itemmodel,CXMLTreeNode &node)
{
	CItemModelASE *itemmodelase=dynamic_cast<CItemModelASE *>(itemmodel);
	if (itemmodelase!=NULL)
	{
		std::string model=node.GetPszProperty("model");
		std::string texture=node.GetPszProperty("texture");
		itemmodelase->LoadModelASE(model,texture);
	}
	itemmodel->SetFlagDespY(node.GetBoolProperty("despy",true));
	itemmodel->SetDebug(node.GetBoolProperty("debug",false));
	Vect3f *size=LoadNodeItemModel(node,"size");
	if (size!=NULL)
		itemmodel->SetSize(*size);
	itemmodel->SetScale(LoadNodeItemModel(node,"scale"));
	itemmodel->SetDesp(LoadNodeItemModel(node,"desp"));
}

Vect3f *CItemManager::LoadNodeItemModel(CXMLTreeNode &node,std::string vname)
{
	CXMLTreeNode vnode=node[vname.c_str()];

	if (!vnode.Exists())
		return NULL;
	float x=vnode.GetFloatProperty("x");
	float y=vnode.GetFloatProperty("y");
	float z=vnode.GetFloatProperty("z");
	Vect3f *v=new Vect3f(x,y,z);

	return v;
}

bool CItemManager::ReloadModelsXml()
{
	if (m_sModelXML!="")
	{
		if (!LoadModelsXml(m_sModelXML))
			return false;
		ReCreateItemsActor();
	}

	return true;
}

void CItemManager::ReCreateItemsActor()
{
	std::vector<CItem *>::iterator it=m_Items.begin(),itend=m_Items.end();
	
	for(;it!=itend;it++)
	{
		CItem *item=*it;
		item->ReCreateActor();
	}
}

void CItemManager::Update(float elapsedTime)
{
		m_fRotationY += elapsedTime*0.25f;
		std::vector<CItem *>::iterator it=m_Items.begin(),itend=m_Items.end();
	
	for(;it!=itend;it++)
	{
		CItem *item=*it;
		item->Update(elapsedTime);
	}
}

void CItemManager::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	std::vector<CItem *>::iterator it=m_Items.begin(),itend=m_Items.end();
	
	for(;it!=itend;it++)
	{
		CItem *item=*it;
		if (!item->GetSelected())
			RenderItem(*it,renderManager,fontManager);
	}
}

void CItemManager::RenderItem(CItem *item,CRenderManager* renderManager, CFontManager* fontManager)
{

	//		- Girar los items sobre si mismos.
	//		//		- Hacerlos subir y bajar en el eje Y
	//		//	1. Primer tenemos que modificar la posición del item para que suba y baje...
	//		//		1.1 Incrementar m_angle 0.25 unidades cada segundo
	//		//		1.2 Utilizar el sinus y m_angle para obtener un valor que vaya oscilando entre -0.2 y 0.2
	//		//		1.3 Este valor lo tenemos que sumar a la position.y
	//	2. En segundo lugar tenemos que animar la rotación del item sobre si mismo.
	//		//		2.1 Utilizaremos D3DXMatrixRotationY con la matriz rotation y con el ángulo m_angle
	// 3. Finalmente aplicamos los cambios a la matrix final
	//		//		3.1 En Direct3D se multiplican.. en nuestro caso tendríamos que hacer:
	//		//		3.2 matrizScalado * matrizRotación * matrizTranslación

	CItemModel &model=item->GetModel();
	Vect3f pos(item->GetPosition());
	const Vect3f *mscale=model.GetScale();
	const Vect3f *desp=model.GetDesp();
	D3DXMATRIX matrix;
	D3DXMATRIX translation;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;

	if (desp!=NULL)
		pos += *desp;	
	if (model.IsDespY())
		pos.y += 0.3f*sin(m_fRotationY);
	D3DXMatrixTranslation(&translation,pos.x,pos.y,pos.z);
	D3DXMatrixRotationY (&rotation,m_fRotationY);
	if (mscale==NULL)
		matrix=rotation*translation;
	else
	{
		D3DXMatrixScaling(&scale,mscale->x,mscale->y,mscale->z);
		matrix=scale*rotation*translation;
	}
	renderManager->SetTransform(matrix);
	model.RenderModel(renderManager,fontManager);
}
