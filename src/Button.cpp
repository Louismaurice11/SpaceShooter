#include "header.h"

#include "Button.h"

Button::Button(int iX, int iY, BaseEngine* bEngine, int w, int h, const char* text)
	: DisplayableObject(iX, iY, bEngine, w, h, true)
{
	m_bEngine = bEngine;
	m_xPos = iX;
	m_yPos = iY;
	m_width = w;
	m_height = h;
	m_text = text;
	m_image = ImageManager::loadImage("Button01.png", true);
}

void Button::virtDraw()
{
	m_image.renderImageWithMask(m_pEngine->getForegroundSurface(), 0, 0, getDrawingRegionLeft(), getDrawingRegionTop()-20, m_image.getWidth(), m_image.getHeight());
	/*
	m_pEngine->drawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x0000FF);
	*/
	int Size = 0;
	while (m_text[Size] != '\0') Size++;
	m_pEngine->drawForegroundString(getXCentre() - Size * 8, m_iCurrentScreenY, m_text, 0xffffff, m_pEngine->getFont("m5x7.ttf", 48));
}

bool Button::isValidPosition(int iX, int iY)
{
	try {
		if (iX < m_xPos)
			return false;
		if (iY < m_yPos)
			return false;
		if (iX >= (m_xPos + m_width))
			return false;
		if (iY >= (m_yPos + m_height))
			return false;
	}
	catch (int num) {
		return false;
	}

	return true;
}
