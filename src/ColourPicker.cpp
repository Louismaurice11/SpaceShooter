#include "header.h"

#include "ColourPicker.h"

void ColourPicker::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	if (iMapX == m_iColour) {
		m_select.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY + 10, 64, 64, m_select.getPixelColour(32,32));
	}
	switch (iMapX) {
	case 0:
		m_spritesBlue.renderImageWithMask(pSurface, 128, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
		break;
	case 1:
		m_spritesGreen.renderImageWithMask(pSurface, 128, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
		break;
	case 2:
		m_spritesGrey.renderImageWithMask(pSurface, 128, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
		break;
	default:
		m_spritesRed.renderImageWithMask(pSurface, 128, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
		break;
	}
}
