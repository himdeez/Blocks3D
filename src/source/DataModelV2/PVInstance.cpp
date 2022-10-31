#include "DataModelV2/PVInstance.h"

PVInstance::PVInstance(void)
{
	Instance::Instance();
	nameShown = false;
	controllerFlagShown = true;
	className = "PVInstance";
	listicon = 0;
	controller = (Enum::Controller::Value)0;
}

PVInstance::PVInstance(const PVInstance &oinst)
{
	Instance::Instance(oinst);
}

PVInstance::~PVInstance(void)
{
}

void PVInstance::postRender(RenderDevice* rd)
{
}

static TCHAR* enumStr(int controller)
{
	switch(controller)
	{
		case Enum::Controller::None:
			return "None";
		case Enum::Controller::KeyboardLeft:
			return "KeyboardLeft";
		case Enum::Controller::KeyboardRight:
			return "KeyboardRight";
		case Enum::Controller::Joypad1:
			return "Joypad1";
		case Enum::Controller::Joypad2:
			return "Joypad2";
		case Enum::Controller::Chase:
			return "Joypad1";
		case Enum::Controller::Flee:
			return "Joypad2";
	}
	return "None";
}

static Enum::Controller::Value strEnum(TCHAR * tval)
{
	if(strcmp(tval, "KeyboardLeft") == 0)
		return Enum::Controller::KeyboardLeft;
	if(strcmp(tval, "KeyboardRight") == 0)
		return Enum::Controller::KeyboardRight;
	if(strcmp(tval, "Joypad1") == 0)
		return Enum::Controller::Joypad1;
	if(strcmp(tval, "Joypad2") == 0)
		return Enum::Controller::Joypad2;
	if(strcmp(tval, "Chase") == 0)
		return Enum::Controller::Chase;
	if(strcmp(tval, "Flee") == 0)
		return Enum::Controller::Flee;
	return Enum::Controller::None;
}



PROPERTIES_START(PVInstance, Instance)
	
	DECLARE_BOOL_PROPERTY(
		"Item",
		"NameShown",
		"This chooses whether the item name is shown",
		nameShown
	);
	
	DECLARE_BOOL_PROPERTY(
		"Item",
		"ControllerFlagShown",
		"This chooses whether the item's ControllerFlag is shown",
		controllerFlagShown
	);
	
	DECLARE_ENUM_PROPERTY(
		"Behaviour",
		"Controller",
		"This chooses what type of controller is used",
		enumStr(controller),
		"None\0KeyboardRight\0KeyboardLeft\0Joypad1\0Joypad2\0Chase\0Flee"
	)

PROPERTIES_END()

PROP_UPDATE_START(PVInstance)

	DECLARE_PROP_UPDATE("NameShown")
		nameShown = GET_VALUE() == TRUE;
	DECLARE_PROP_UPDATE_END() // NameShown
	
	DECLARE_PROP_UPDATE("ControllerFlagShown")
		controllerFlagShown = GET_VALUE() == TRUE;
	DECLARE_PROP_UPDATE_END() // ControllerFlagShown
	
	DECLARE_PROP_UPDATE("Controller")
		controller = strEnum(GET_ENUM());
	DECLARE_PROP_UPDATE_END() // Controller
	
PROP_UPDATE_END(Instance)