#define WINVER	0x0400
#include <G3DAll.h>
#include "DataModelV2/Instance.h"



Instance::Instance(void)
{
	parent = NULL;
	name = "Default Game Instance";
	className = "BaseInstance";
	listicon = 1;
	canDelete = true;
}

Instance::Instance(const Instance &oinst)
{
	
	name = oinst.name;
	className = oinst.className;
	canDelete = oinst.canDelete;
	listicon = oinst.listicon;
}

void Instance::render(RenderDevice* rd)
{
	for(size_t i = 0; i < children.size(); i++)
	{
		children[i]->render(rd);
	}
}

void Instance::renderName(RenderDevice* rd)
{
	for(size_t i = 0; i < children.size(); i++)
	{
		children[i]->renderName(rd);
	}
}

void Instance::update()
{
}

PROPGRIDITEM Instance::createPGI(LPSTR catalog, LPSTR propName, LPSTR propDesc, LPARAM curVal, INT type, TCHAR choices[])
{
	PROPGRIDITEM pItem;
	PropGrid_ItemInit(pItem);
	pItem.lpszCatalog=catalog;
	pItem.lpszPropName=propName;
	pItem.lpszPropDesc=propDesc;
	pItem.lpCurValue=curVal;
	pItem.iItemType=type;
	if(choices != NULL)
		pItem.lpszzCmbItems = choices;
	return pItem;
}

PROP_UPDATE_START(Instance)
	DECLARE_PROP_UPDATE("Name")
		name = GET_STRING_CONST();
	DECLARE_PROP_UPDATE_END() // Name
PROP_UPDATE_END_NO_BASECLASS()

PROPERTIES_START_NO_BASECLASS(Instance)
	
	DECLARE_STRING_PROPERTY(
		"Properties",
		"Name",
		"The name of this instance",
		name.c_str()
	)
	
PROPERTIES_END()


Instance::~Instance(void)
{
	for(size_t i = 0; i < children.size(); i++)
	{
		delete children.at(i);
	}
}

void Instance::setName(std::string newName)
{
	name = newName;
}

std::string Instance::getClassName()
{
	return className;
}

std::vector<Instance* > Instance::getChildren()
{
	return children;
}

std::vector<Instance* > Instance::getAllChildren()
{
	if(!children.empty())
	{
		std::vector<Instance* > totalchildren = children;
		for(size_t i = 0; i < children.size(); i++)
		{
			std::vector<Instance* > subchildren = children.at(i)->getAllChildren();
			if(!subchildren.empty())
				totalchildren.insert(totalchildren.end(), subchildren.begin(), subchildren.end());
		}
		return totalchildren;
	}
	return children;
}

void Instance::setParent(Instance* newParent)
{
	if(parent != NULL)
	{
		parent->removeChild(this);
	}
	parent = newParent;
	if(newParent != NULL)
	{
		newParent->addChild(this);
	}
}

Instance* Instance::getParent()
{
	return parent;
}

void Instance::addChild(Instance* newChild)
{
	children.push_back(newChild);
}

void Instance::clearChildren()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		delete children.at(i);
	} 
	children.clear();
}
void Instance::removeChild(Instance* oldChild)
{
	for(size_t i = 0; i < children.size(); i++)
	{
		if(children.at(i) == oldChild)
		{
			children.erase(children.begin() + i);
		}
	}
}

Instance* Instance::findFirstChild(std::string name)
{
	for(size_t i = 0; i < children.size(); i++)
	{
		if(children.at(i)->name.compare(name) == 0)
		{
			return children.at(i);
		}
	}
	return NULL;
}



