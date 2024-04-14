#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "GameEngine.h"

class TextBox :
    public DisplayableObject
{
public:

    TextBox(int iX, int iY, BaseEngine* bEngine, int w, int h);

    std::string getName() {
        return m_cText;
    }

    void setIsClicked(bool b)
    {
        m_bClicked = b;
        if (m_bClicked == true)
        {
            m_iColour = 0xFFFFFF;
            this->redrawDisplay();
        }
        if (m_bClicked == false)
        {
            m_iColour = 0xD3D3D3;
            this->redrawDisplay();
        }
    }

    void virtDraw();
    virtual void virtDoUpdate(int iCurrentTime) override;

    int countChars(const char* ptr)
    {
        int i = 0;
        while (ptr[i] != '\0')
            i++;
        return 0;
    }

    bool isValidPosition(int iX, int iY)
    {
        if (iX < m_xPos)
            return false;
        if (iY < m_yPos)
            return false;
        if (iX >= (m_xPos + m_width))
            return false;
        if (iY >= (m_yPos + m_height))
            return false;

        return true;
    }
private:
    std::string m_cText;
    GameEngine* m_gEngine;
    int m_xPos;
    int m_yPos;
    int m_width;
    int m_height;
    int m_iColour;
    bool m_bClicked;
    int m_iPrevTime;
};


#endif // !TEXTBOX_H

