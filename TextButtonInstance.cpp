#include "TextButtonInstance.h"
Vector2 boxBegin;
Vector2 boxEnd;
Vector2 fontLocationRelativeTo;
Color4 textColor;
Color4 textOutlineColor;
Color4 boxColor;
Color4 boxOutlineColor;
Color4 textColorOvr;
Color4 textOutlineColorOvr;
Color4 boxColorOvr;
Color4 boxOutlineColorOvr;
Color4 textColorDn;
Color4 textOutlineColorDn;
Color4 boxColorDn;
Color4 boxOutlineColorDn;
bool centeredWithinBox;
std::string title;
G3D::GFontRef* font;
int textSize;
bool floatBottom;
bool floatRight;
bool floatCenter;
bool visible;
ButtonListener* buttonListener;

TextButtonInstance::TextButtonInstance(void)
{
	boxBegin = Vector2(0,0);
	boxEnd = Vector2(0,0);
	fontLocationRelativeTo = Vector2(0,0);
	centeredWithinBox = false;
	title = "TextBox";
	textColor = Color4(1, 1, 1, 1);
	textOutlineColor = Color4(0, 0, 0, 0);
	boxColor = Color4(0.6F,0.6F,0.6F,0.4F);
	boxOutlineColor = Color4(0, 0, 0, 0);
	textSize = 12;
	floatBottom = false;
	floatRight = false;
	floatCenter = false;
	visible = true;
	className = "TextButton";
}

TextButtonInstance::~TextButtonInstance(void)
{
	delete buttonListener;
}

void TextButtonInstance::setButtonListener(ButtonListener* listener)
{
	buttonListener = listener;
}

void TextButtonInstance::onClick()
{
	if(buttonListener != NULL)
	{
		buttonListener->onButton1MouseClick(this);
	}
}

void TextButtonInstance::drawObj(RenderDevice* rd)
{
	Vector3 point1;
	Vector3 point2;
	if(floatBottom)
	{
		point1 = Vector3(boxBegin.x, rd->getHeight() + boxBegin.y,0);
		point2 = Vector3(boxEnd.x, rd->getHeight() + boxEnd.y,0);
		
	}
	else
	{
		point1 = Vector3(boxBegin.x, boxBegin.y,0);
		point2 = Vector3(boxEnd.x, boxEnd.y,0);
	}
	Draw::box(Box(point1, point2), rd, boxColor, boxOutlineColor);
	Vector2 RelativeTo = Vector2(point1.x + fontLocationRelativeTo.x, point1.y + fontLocationRelativeTo.y);
	font->draw2D(rd, title, RelativeTo, textSize, textColor, textOutlineColor);

}