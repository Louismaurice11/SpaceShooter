#pragma once
#include "TileManager.h"
class ColourPicker :
    public TileManager
{
public:
    ColourPicker()
        : TileManager(64, 64, 4, 1)
    {
        m_iColour = 0;
        m_spritesBlue = ImageManager::loadImage("playersRightBLue.png");
        m_spritesGreen = ImageManager::loadImage("playersRightGreen.png");
        m_spritesGrey = ImageManager::loadImage("playersRightGrey.png");
        m_spritesRed = ImageManager::loadImage("playersRightRed.png");
        m_select = ImageManager::loadImage("Select.png");
    }

    void setColour(int colour) {
        m_iColour = colour;
    }

    int getColour() {
        return m_iColour;
    }

    virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;
private:
    int m_iColour;

    SimpleImage m_spritesBlue, m_spritesGreen, m_spritesGrey, m_spritesRed, m_select;
};

