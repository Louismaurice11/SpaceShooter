#include "header.h"
#include "MenuBackdrop.h"

MenuBackdrop::MenuBackdrop(int iX, int iY, BaseEngine* bEngine)
	: DisplayableObject(iX, iY, bEngine, 576, 576, true)
{
	m_image = ImageManager::loadImage("MenuBackdrop.png");
}

void MenuBackdrop::virtDraw()
{
	m_image.renderImageWithMaskAndTransparency(m_pEngine->getForegroundSurface(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_image.getWidth(), m_image.getHeight(), m_image.getPixelColour(0,0), 75);
}
