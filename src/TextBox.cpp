#include "header.h"

#include "TextBox.h"

TextBox::TextBox(int iX, int iY, BaseEngine* bEngine, int w, int h)
    : DisplayableObject(iX, iY, bEngine, w, h, true)
{
    m_cText = "";
    m_gEngine = (GameEngine*)this->getEngine();
    m_bClicked = false;
    m_xPos = iX;
    m_yPos = iY;
    m_width = w;
    m_height = h;
    m_iColour = 0xD3D3D3;
    m_iPrevTime = m_gEngine->getModifiedTime();
}

void TextBox::virtDraw()
{
    m_gEngine->drawForegroundString(m_iCurrentScreenX, m_iCurrentScreenY - 50, "Enter Name:", 0xFFFFFF, m_gEngine->getFont("m5x7.ttf", 48));
    m_pEngine->drawForegroundRectangle(
        m_iCurrentScreenX, m_iCurrentScreenY,
        m_iCurrentScreenX + m_iDrawWidth - 1,
        m_iCurrentScreenY + m_iDrawHeight - 1,
        m_iColour);
    if (m_cText.size() > 0)
        m_pEngine->drawForegroundString(m_iCurrentScreenX + 5, m_iCurrentScreenY, m_cText.c_str(), 0x00000, m_gEngine->getFont("m5x7.ttf", 48));
}

void TextBox::virtDoUpdate(int iCurrentTime)
{
    
    
    // Do not update if text box hasn't been clicked
    if (!m_bClicked)
       return;

    // Check characters

    

    const char* c = "";
    if (m_gEngine->getModifiedTime() < m_iPrevTime + 150)
        return;

    if (m_gEngine->isKeyPressed(SDLK_a))
    {
        c = "a";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_b))
    {
        c = "b";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_c))
    {
        c = "c";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_d))
    {
        c = "d";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_e))
    {
        c = "e";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_f))
    {
        c = "f";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_g))
    {
        c = "g";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_h))
    {
        c = "h";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_i))
    {
        c = "i";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_j))
    {
        c = "j";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_k))
    {
        c = "k";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_l))
    {
        c = "l";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_m))
    {
        c = "m";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_n))
    {
        c = "n";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_o))
    {
        c = "o";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_p))
    {
        c = "p";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_q))
    {
        c = "q";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_r))
    {
        c = "r";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_s))
    {
        c = "s";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_t))
    {
        c = "t";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_u))
    {
        c = "u";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_v))
    {
        c = "v";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_w))
    {
        c = "w";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_x))
    {
        c = "x";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_y))
    {
        c = "y";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_z))
    {
        c = "z";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_SPACE))
    {
        c = " ";
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    if (m_gEngine->isKeyPressed(SDLK_BACKSPACE))
    {
        if (m_cText.size() > 0)
            m_cText.pop_back();
        m_iPrevTime = m_gEngine->getModifiedTime();
    }
    m_cText.append(c);

    this->redrawDisplay();
}
