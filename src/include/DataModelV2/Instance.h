#pragma once
#include <G3DAll.h>
#include "propertyGrid.h"
#include "map"

/* PROP_UPDATE */
// functions macros
#define PROP_UPDATE_START(Class) void Class::PropUpdate(LPPROPGRIDITEM &item) {
#define PROP_UPDATE_END(BaseClass) \
	BaseClass::PropUpdate(item); \
}
#define PROP_UPDATE_END_NO_BASECLASS() }
#define DECLARE_PROP_UPDATE(PropName) if(strcmp(item->lpszPropName, PropName) == 0) {
#define DECLARE_PROP_UPDATE_END() return; \
	}

// values macros
#define GET_VALUE() item->lpCurValue // gets just a normal value
#define GET_COLOR3() Color3( \
			GetRValue(GET_VALUE())/255.0F, \
			GetGValue(GET_VALUE())/255.0F, \
			GetBValue(GET_VALUE())/255.0F \
		) // gets color3
#define GET_STRING_CONST() (LPSTR)GET_VALUE() // gets lpstr string
#define GET_ENUM() (TCHAR*)GET_VALUE() // gets tchar
#define GET_STRING() (LPTSTR)GET_VALUE() // gets non-const tchar string

// verbose macros
#define GET_BOOL GET_VALUE
#define GET_TCHAR GET_ENUM
#define GET_LPTSTR GET_STRING
#define GET_LPSTR GET_STRING_CONST

/* PROPERTIES */
// functions macros
#define PROPERTIES_START_NO_BASECLASS(Class) std::vector<PROPGRIDITEM> Class::getProperties() { \
	std::vector<PROPGRIDITEM> properties;

#define PROPERTIES_START(Class, BaseClass) std::vector<PROPGRIDITEM> Class::getProperties() { \
	std::vector<PROPGRIDITEM> properties = BaseClass::getProperties();
#define PROPERTIES_END() \
	return properties; \
}

#define DECLARE_STRING_PROPERTY(Section, Name, Description, Variable) properties.push_back(createPGI( \
		Section, \
		Name, \
		Description, \
		(LPARAM)Variable, \
		PIT_EDIT \
	));

#define DECLARE_COLOR3_PROPERTY(Section, Name, Description, Variable) properties.push_back(createPGI( \
		Section, \
		Name, \
		Description, \
		RGB((Variable.r*255),(Variable.g*255),(Variable.b*255)), \
		PIT_COLOR \
	));

#define DECLARE_BOOL_PROPERTY(Section, Name, Description, Variable) properties.push_back(createPGI( \
		Section, \
		Name, \
		Description, \
		(LPARAM)Variable, \
		PIT_CHECK \
	));

#define DECLARE_VECTOR3_PROPERTY(Section, Name, Description, Variable) properties.push_back(createPGI( \
		Section, \
		Name, \
		Description, \
		(LPARAM)Variable, \
		PIT_EDIT \
	));

#define DECLARE_ENUM_PROPERTY(Section, Name, Description, Variable, Enums) properties.push_back(createPGI( \
		Section, \
		Name, \
		Description, \
		(LPARAM)Variable, \
		PIT_COMBO, \
		TEXT(Enums) \
	));

class Instance
{
public:
	bool canDelete;
	Instance(void);
	Instance(const Instance&);
	virtual ~Instance(void);
	std::string name;
	virtual void render(RenderDevice*);
	virtual void renderName(RenderDevice*);
	virtual void update();
	std::vector<Instance*> children; // All children.
	std::string getClassName();
	Instance* findFirstChild(std::string);
	std::vector<Instance* > getChildren();
	std::vector<Instance* > getAllChildren();
	virtual void setParent(Instance*);
	void setName(std::string newName);
	void addChild(Instance*);
	void removeChild(Instance*);
	void clearChildren();
	Instance* getParent();
	virtual Instance* clone() const { return new Instance(*this); }
	virtual std::vector<PROPGRIDITEM> getProperties();
	virtual void PropUpdate(LPPROPGRIDITEM &pItem);
	int listicon;
protected:
	std::string className;
	Instance* parent;  // Another pointer.
	PROPGRIDITEM createPGI(LPSTR catalog, LPSTR propName, LPSTR propDesc, LPARAM curVal, INT type, TCHAR choices[] = NULL);
private:
	static const std::map<std::string, Instance> g_logLevelsDescriptions;
};
