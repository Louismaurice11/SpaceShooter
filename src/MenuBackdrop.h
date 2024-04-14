#pragma once
#include "GameEngine.h"

class SimpleImage;

class MenuBackdrop :
    public DisplayableObject
{
public:
    MenuBackdrop(int iX, int iY, BaseEngine* bEngine);

    void virtDraw();
private:
    SimpleImage m_image;
};

