#include "DataModelV2/LightingInstance.h"
#include "Application.h"
#include "Globals.h"
#include "StringFunctions.h"
#include "Listener/ModeSelectionListener.h"

LightingInstance::LightingInstance(void)
{
	Instance::Instance();
	name = "Lighting";
	className = "Lighting";
	listicon = 10;
	canDelete = false;

	_hideSky = false;

	lighting.setTime(G3D::toSeconds(2, 00, 00, PM));

	// Maybe this is Top and Bottom Ambient?
	lighting.ambient = Color3(0.5F, 0.5F, 0.5F);
	lighting.lightColor = Color3(0.8F, 0.8F, 0.8F);

	sky = Sky::create(NULL, ExePath() + "/content/sky/");
	clearColor = Color4(0.0F, 0.0F, 0.0F, 0.0F);
}

LightingInstance::~LightingInstance(void)
{
	sky->~Sky();
}

// Getters
G3D::SkyRef LightingInstance::getSky()
{
	return sky;
}

G3D::LightingParameters LightingInstance::getLightingParameters()
{
	return lighting;
}

PROPERTIES_START(LightingInstance, Instance)

	DECLARE_COLOR3_PROPERTY(
		"Appearance",
		"TopAmbient",
		"The color of the TopAmbient for 3D Objects",
		topAmbient
	);

	DECLARE_COLOR3_PROPERTY(
		"Appearance",
		"BottomAmbient",
		"The color of the BottomAmbient for 3D Objects",
		bottomAmbient
	);

	DECLARE_COLOR3_PROPERTY(
		"Appearance",
		"SpotLight",
		"The color of the SpotLight",
		spotLight
	);

	DECLARE_COLOR3_PROPERTY(
		"Appearance",
		"ClearColor",
		"",
		clearColor
	);
	
PROPERTIES_END()


PROP_UPDATE_START(LightingInstance)
	
	DECLARE_PROP_UPDATE("TopAmbient")
		topAmbient = GET_COLOR3();
	DECLARE_PROP_UPDATE_END() // BottomAmbient
	
	DECLARE_PROP_UPDATE("BottomAmbient")
		bottomAmbient = GET_COLOR3();
	DECLARE_PROP_UPDATE_END() // BottomAmbient
	
	DECLARE_PROP_UPDATE("SpotLight")
		spotLight = GET_COLOR3();
	DECLARE_PROP_UPDATE_END() // SpotLight
	
	DECLARE_PROP_UPDATE("ClearColor")
		clearColor = GET_COLOR3();
	DECLARE_PROP_UPDATE_END() // ClearColor
	
PROP_UPDATE_END(Instance)


// Functions
void LightingInstance::suppressSky(bool doSuppress)
{
	_hideSky = doSuppress;
}

void LightingInstance::update()
{
	RenderDevice* rd = g_usableApp->getRenderDevice();

    
	if(!_hideSky) {
		rd->clear(sky.isNull(), true, true);
		if (sky.notNull()) sky->render(rd, lighting);
	} else {
		rd->setColorClearValue(clearColor);
		rd->clear(true, true, true);
		suppressSky(false);
	}
	
    // Setup lighting
    rd->enableLighting();

	rd->setShadeMode(RenderDevice::SHADE_SMOOTH);
	rd->setAmbientLightColor(Color3(1,1,1));

	rd->setLight(0, GLight::directional(lighting.lightDirection, lighting.lightColor, true, true));
	rd->setAmbientLightColor(lighting.ambient);

	rd->beforePrimitive();
	CoordinateFrame forDraw = rd->getObjectToWorldMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	g_dataModel->getWorkspace()->render(rd);
	g_dataModel->getWorkspace()->renderName(rd);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	rd->setObjectToWorldMatrix(forDraw);
	rd->afterPrimitive();

	g_dataModel->getSelectionService()->render(rd);
	g_usableApp->tool->render(rd, g_usableApp->mouse);
	

    rd->disableLighting();

	drawEffects();
}

void LightingInstance::drawEffects()
{
	RenderDevice* rd = g_usableApp->getRenderDevice();
    if (sky.notNull()) {
        sky->renderLensFlare(rd, lighting);
    }
}