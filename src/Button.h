#ifndef BUTTON_H
#define BUTTON_H

#include "GameEngine.h"

class SimpleImage;


class Button :
    public DisplayableObject
{
public:
    Button(int iX, int iY, BaseEngine* bEngine, int w, int h, const char* text);

    void virtDraw();

    bool isValidPosition(int iX, int iY);
private:
    BaseEngine* m_bEngine;
    int m_xPos;
    int m_yPos;
    int m_width;
    int m_height;
    const char* m_text;
    SimpleImage m_image;
};

#endif // !BUTTON_H


