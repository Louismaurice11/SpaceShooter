#include "header.h"
#include "MovingObject.h"

/***MovingObject Functions***/

MovingObject::MovingObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, int health)
    : DisplayableObject(xStart, yStart, pEngine, width, height, true)
{
    m_gEngine = (GameEngine*)this->getEngine();
    m_health = health;
    m_iDirection = 1;
    m_bStill = true;
    m_iRenderOffset = 0;
    m_iManager = new ImageManager();
    m_iPrevTime = m_gEngine->getModifiedTime();
    m_bDead = false;
    m_bDeathComplete = false;
    m_iShootTimer = m_gEngine->getModifiedTime();
    m_iColour = 3;
}


/***MovingPlayerObject Functions***/

MovingPlayerObject::MovingPlayerObject(BaseEngine* pEngine, EnemySpawner* enemySpawner, int colour)
    : MovingObject(640, 448, pEngine, 64, 64, 10)
{
    m_enemySpawner = enemySpawner;
    m_sprite = m_iManager->loadImage("playersRight.png");
    setTrueSides();
    m_bWin = false;
    m_iSkin = 1;
    m_iColour = colour;
}

bool MovingPlayerObject::checkCoverTop(int iSpeed)
{
    bool bIsCover = false;
    GameTileManager* tm = m_gEngine->getTileManager();
    int iMapY = tm->getMapYForScreenY(getDrawingRegionTop() + m_trueTop - iSpeed);
    int iMapX = tm->getMapXForScreenX(getDrawingRegionLeft() + m_trueLeft);

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    iMapX = tm->getMapXForScreenX(getDrawingRegionRight() - m_trueRight);

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    return bIsCover;
}

bool MovingPlayerObject::checkCoverBottom(int iSpeed)
{
    bool bIsCover = false;
    GameTileManager* tm = m_gEngine->getTileManager();
    int iMapY = tm->getMapYForScreenY(getDrawingRegionBottom() + iSpeed);
    int iMapX = tm->getMapXForScreenX(getDrawingRegionLeft() + m_trueLeft);

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    iMapX = tm->getMapXForScreenX(getDrawingRegionRight() - m_trueRight);

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    return bIsCover;
}

bool MovingPlayerObject::checkCoverLeft(int iSpeed)
{
    bool bIsCover = false;
    GameTileManager* tm = m_gEngine->getTileManager();
    int iMapY = tm->getMapYForScreenY(getDrawingRegionTop() + m_trueTop);
    int iMapX = tm->getMapXForScreenX(getDrawingRegionLeft() + m_trueLeft - iSpeed);

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    iMapY = tm->getMapYForScreenY(getDrawingRegionBottom());

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    return bIsCover;
}

bool MovingPlayerObject::checkCoverRight(int iSpeed)
{
    bool bIsCover = false;
    GameTileManager* tm = m_gEngine->getTileManager();
    int iMapY = tm->getMapYForScreenY(getDrawingRegionTop() + m_trueTop);
    int iMapX = tm->getMapXForScreenX(getDrawingRegionRight() - m_trueRight + iSpeed);

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    iMapY = tm->getMapYForScreenY(getDrawingRegionBottom());

    if (tm->getMapValue(iMapX, iMapY) == 1) bIsCover = true;

    return bIsCover;
}

void MovingPlayerObject::virtDraw()
{
    int iDeathOffset = 0;
    int iIdleOffset = 0;
    int iMovingOffset = 0;
    int mRO = 0, iRO = 0;
    std::string colour;
    std::string direction;

    int spriteCenterX = m_iCurrentScreenX + 64 / 2;

    int textLeftX = spriteCenterX - (m_gEngine->getName().size() * 12) / 2; 

    m_gEngine->drawForegroundString(textLeftX, getDrawingRegionBottom() - 10,
        m_gEngine->getName().c_str(), 0x000000, m_gEngine->getFont("m5x7.ttf", 30));

    switch (m_iSkin) {
    case 0:
        iDeathOffset = 320;
        iIdleOffset = 0;
        iMovingOffset = 192;
        break;
    case 1:
        iDeathOffset = 768;
        iIdleOffset = 448;
        iMovingOffset = 640;
        break;
    default:
        break;
    }

    switch (m_iColour) {
    case 0:
        colour = "Blue";
        break;
    case 1:
        colour = "Green";
        break;
    case 2:
        colour = "Grey";
        break;
    case 3:
        colour = "Red";
        break;
    default:
        std::cout << "Error: Invalid colour" << std::endl;
        exit(1);
    }


    if (m_bDead)
    {
        if (m_gEngine->getModifiedTime() >= m_iPrevTime + 100)
        {
            m_iPrevTime = m_gEngine->getModifiedTime();
            m_iRenderOffset += 64;
            if (m_iRenderOffset > 320)
                m_bDeathComplete = true;
        }
        m_sprite = m_iManager->loadImage("playersRight.png");
        m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, iDeathOffset, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
    }
    else {
        int t;
        switch (m_bSprint) {
        case true:
            t = 160;
            break;
        default:
            t = 250;
            break;
        }
        if (m_gEngine->getModifiedTime() >= m_iPrevTime + t)
        {
            m_iPrevTime = m_gEngine->getModifiedTime();
            m_iRenderOffset += 64;
            if (m_iRenderOffset > 192)
                m_iRenderOffset = 0;
        }
        switch (m_iDirection)
        {
        case 0:
            mRO = 576 - m_iRenderOffset;
            iRO = 576;
            direction = "Left";
            break;
        default:
            mRO = 0 + m_iRenderOffset;
            iRO = 0;
            direction = "Right";
            break;
        }
        m_sprite = m_iManager->loadImage("players" + direction + colour + ".png");
        if (!m_bStill)
            m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), mRO, iMovingOffset, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
        else
            m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), iRO, iIdleOffset, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
    }
}

void MovingPlayerObject::virtDoUpdate(int iCurrentTime)
{
    if (m_bWin)
    {
        m_gEngine->removeDisplayableObject(this);
        m_gEngine->setState(new WinState(m_gEngine));
    }
    if (m_bDead) {
        if (m_bDeathComplete)
        {
            m_gEngine->removeDisplayableObject(this);
            m_gEngine->setState(new GameOverState(m_gEngine));
        }
        return;
    }
    m_bStill = true;
    int s = 1;
    if (m_gEngine->getState()->getStateID() == RUNNINGSTATE) {
        if (m_gEngine->isKeyPressed(SDLK_LSHIFT)) {
            m_bSprint = true;
            s = 2;
        }
        else {
            m_bSprint = false;
        }
        // Change position if player presses a key
        if (m_gEngine->isKeyPressed(SDLK_w) && !checkCoverTop(s))
        {
            m_iCurrentScreenY -= s;
            m_bStill = false;
        }
        if (m_gEngine->isKeyPressed(SDLK_s) && !checkCoverBottom(s))
        {
            m_iCurrentScreenY += s;
            m_bStill = false;
        }
        if (m_gEngine->isKeyPressed(SDLK_a) && !checkCoverLeft(s))
        {
            m_iCurrentScreenX -= s;
            m_bStill = false;
        }
        if (m_gEngine->isKeyPressed(SDLK_d) && !checkCoverRight(s))
        {
            m_iCurrentScreenX += s;
            m_bStill = false;
        }

        // Stop object from going out of bounds
        if (m_iCurrentScreenX < 0)
            m_iCurrentScreenX = 0;
        if (m_iCurrentScreenX >= m_gEngine->getWindowWidth() -
            m_iDrawWidth)
            m_iCurrentScreenX = m_gEngine->getWindowWidth() -
            m_iDrawWidth;
        if (m_iCurrentScreenY < 0)
            m_iCurrentScreenY = 0;
        if (m_iCurrentScreenY >= m_gEngine->getWindowHeight() -
            m_iDrawHeight)
            m_iCurrentScreenY = m_gEngine->getWindowHeight() -
            m_iDrawHeight;
        

        if (m_gEngine->getCurrentMouseX() <= m_iCurrentScreenX)
            m_iDirection = 0;
        if (m_gEngine->getCurrentMouseX() > m_iCurrentScreenX)
            m_iDirection = 1;

        m_enemySpawner->checkLevelChange();
        this->redrawDisplay();
    }
}


/***EnemyObject Functions***/

void EnemyObject::setPlayerPosition()
{
    DisplayableObject* obj = m_gEngine->getDisplayableObject(0);
    m_playerX = obj->getXCentre();
    m_playerY = obj->getYCentre();
}

void EnemyObject::checkKill()
{
    MovingPlayerObject* pObject = (MovingPlayerObject*) m_gEngine->getDisplayableObject(0);
    // If colliding with player

    if (CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
        pObject->getDrawingRegionLeft(), pObject->getDrawingRegionRight(), pObject->getDrawingRegionTop(), pObject->getDrawingRegionBottom()))
    {
        // Game Over
        pObject->kill();
    }

}

void EnemyObject::killEnemy()
{
    m_bVisible = false;
    m_bIsDead = true;
    m_gEngine->getEnemySpawner()->killEnemy();
    m_gEngine->drawableObjectsChanged();
    m_gEngine->removeDisplayableObject(this);
    delete this;
}


/***FlyingEnemyObject Functions***/

FlyingEnemyObject::FlyingEnemyObject(BaseEngine* pEngine, int ID)
    : EnemyObject(pEngine, 64, 64, 3, ID, FLYINGENEMY)
{
    m_speed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (4 - 2) + 2;
    m_speedChangeTimer = (rand() % 5 + 3) * 1000;
    m_time = getEngine()->getRawTime();
}

void FlyingEnemyObject::virtDraw()
{
    if (m_bVisible) {
        if (m_bDead)
        {
            if (m_gEngine->getModifiedTime() >= m_iPrevTime + 100)
            {
                m_iPrevTime = m_gEngine->getModifiedTime();
                m_iRenderOffset += 64;
                if (m_iRenderOffset > 384)
                {
                    m_bDeathComplete = true;
                    m_bVisible = false;
                    m_gEngine->getEnemySpawner()->killEnemy();
                    m_gEngine->drawableObjectsChanged();
                    m_gEngine->removeDisplayableObject(this);
                    delete this;
                }

            }
            switch (m_iDirection)
            {
            case 0:
                m_sprite = m_iManager->loadImage("enemiesLeft.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 192, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            case 1:
                m_sprite = m_iManager->loadImage("enemiesRight.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 192, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
            }
        }
        else
        {
            if (m_gEngine->getModifiedTime() >= m_iPrevTime + 250)
            {
                m_iPrevTime = m_gEngine->getModifiedTime();
                m_iRenderOffset += 64;
                if (m_iRenderOffset > 192)
                    m_iRenderOffset = 0;
            }
            switch (m_iDirection)
            {
            case 0:
                m_sprite = m_iManager->loadImage("enemiesLeft.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 64, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            case 1:
                m_sprite = m_iManager->loadImage("enemiesRight.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 64, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            default:
                break;
            }
        }
    }
}

void FlyingEnemyObject::virtDoUpdate(int iCurrentTime)
{

    if (m_gEngine->getState()->getStateID() != RUNNINGSTATE || m_bDead == true) {
        return;
    }

    setPlayerPosition();

    EnemyObject* eObject;
    for (int iObjectId = 1; iObjectId < m_gEngine->getContentCount(); iObjectId++) {
        eObject = (EnemyObject*)m_gEngine->getDisplayableObject(iObjectId);
        // Only check collision with other enemies, skip self
        if (eObject == nullptr || eObject == this || eObject->getEnemyType() != FLYINGENEMY || eObject->getDead()) {
            continue;
        }

        // Check for collision
        if (CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
            eObject->getDrawingRegionLeft(), eObject->getDrawingRegionRight(), eObject->getDrawingRegionTop(), eObject->getDrawingRegionBottom()))
        {
            // Adjust positions to move away from the colliding object
            int dx = m_iCurrentScreenX - eObject->getXCentre();
            int dy = m_iCurrentScreenY - eObject->getYCentre();
            float angle = atan2(dy, dx);

            m_iCurrentScreenX += m_speed * cos(angle);
            m_iCurrentScreenY += m_speed * sin(angle);
        }
    }

    DisplayableObject* pObject = m_gEngine->getDisplayableObject(0);
    int dx = m_playerX - m_iCurrentScreenX;
    int dy = m_playerY - m_iCurrentScreenY;
    float angle = atan2(dy, dx);
    m_iCurrentScreenX += m_speed * cos(angle);
    m_iCurrentScreenY += m_speed * sin(angle);

    if (m_gEngine->getModifiedTime() >= m_time + m_speedChangeTimer) {
        // generate a new random speed
        m_speed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (5 - 2) + 2;

        // reset the timer for the next speed change
        m_speedChangeTimer = (rand() % 5 + 4) * 1000; // change speed every 4-8 seconds
        m_time = m_gEngine->getModifiedTime();
    }

    if (m_iCurrentScreenX <= m_playerX)
        m_iDirection = 1;
    else
        m_iDirection = 0;

    this->redrawDisplay();
    checkKill();
}


/***StandardEnemyObject Functions***/

StandardEnemyObject::StandardEnemyObject(BaseEngine* pEngine, int ID)
    : EnemyObject(pEngine, 35, 35, 6, ID, STANDARDENEMY)
{
    m_bStill = false;
}

void StandardEnemyObject::virtDraw()
{
    if (m_bVisible) {
        if (m_bDead)
        {
            if (m_gEngine->getModifiedTime() >= m_iPrevTime + 100)
            {
                m_iPrevTime = m_gEngine->getModifiedTime();
                m_iRenderOffset += 64;
                if (m_iRenderOffset > 384)
                {
                    m_bDeathComplete = true;
                    m_bVisible = false;
                    m_gEngine->getEnemySpawner()->killEnemy();
                    m_gEngine->drawableObjectsChanged();
                    m_gEngine->removeDisplayableObject(this);
                    delete this;
                }
                
            }
            switch (m_iDirection)
            {
            case 0:
                m_sprite = m_iManager->loadImage("enemiesLeft.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 640, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            case 1:
                m_sprite = m_iManager->loadImage("enemiesRight.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 640, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
            }
        }
        else
        {
            if (m_gEngine->getModifiedTime() >= m_iPrevTime + 250)
            {
                m_iPrevTime = m_gEngine->getModifiedTime();
                m_iRenderOffset += 64;
                if (m_iRenderOffset > 192)
                    m_iRenderOffset = 0;
            }
            switch (m_iDirection)
            {
            case 0:
                m_sprite = m_iManager->loadImage("enemiesLeft.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 512, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            case 1:
                m_sprite = m_iManager->loadImage("enemiesRight.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 512, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            default:
                break;
            }
        }
    }
}

void StandardEnemyObject::virtDoUpdate(int iCurrentTime)
{
    int newX = m_iCurrentScreenX;
    int newY = m_iCurrentScreenY;

    if (m_gEngine->getState()->getStateID() != RUNNINGSTATE || m_bDead == true) {
        return;
    }
    int px, py, ex, ey;

    setPlayerPosition();
    GameTileManager* tm = m_gEngine->getTileManager();
    MovingPlayerObject* player = (MovingPlayerObject*)m_gEngine->getDisplayableObject(0);
    
    px = tm->getMapXForScreenX(player->getXCentre());
    py = tm->getMapYForScreenY(player->getYCentre());
    ex = tm->getMapXForScreenX(getXCentre());
    ey = tm->getMapYForScreenY(getYCentre());
    
    if (px == ex && py == ey) {
        return;
    }
    tm->setStartAndEndNodes(ex, ey, px, py);

    GameTileManager::sNode* nextNode = tm->solveAStar();
    GameTileManager::sNode* nodeEnd = tm->getEndNode();
    GameTileManager::sNode* nodes = tm->nodeList();


    for (int x = 0; x < tm->getMapWidth(); x++)
    {
        for (int y = 0; y < tm->getMapHeight(); y++)
        {
            nodes[y * tm->getMapWidth() + x].bInPath = false;
        }
    }
    
    if (nodeEnd != nullptr) {
        GameTileManager::sNode* p = nodeEnd;
        while (p->parent != nullptr) {
            p->bInPath = true;
            p = p->parent;
        }
    }

    if (nextNode == nullptr)
    {
        return;
    }
    int nextNodeScreenX = nextNode->x * tm->getTileWidth();
    int nextNodeScreenY = nextNode->y * tm->getTileHeight();    
    int iMaxSpeed = 2;
    double dx = nextNodeScreenX - m_iCurrentScreenX;
    double dy = nextNodeScreenY - m_iCurrentScreenY;
    double distance = sqrt(dx * dx + dy * dy);

    // If the distance is less than the movement speed, move to the next node
    if (distance <= iMaxSpeed) {
        newX = nextNodeScreenX;
        newY = nextNodeScreenY;
    }
    else {
        // Otherwise, move towards the next node
        double moveX = dx / distance * iMaxSpeed;
        double moveY = dy / distance * iMaxSpeed;

        newX += moveX;
        newY += moveY;
    }

    EnemyObject* eObject;

    for (int iObjectId = 1; iObjectId < m_gEngine->getContentCount(); iObjectId++) {
        eObject = (EnemyObject*)m_gEngine->getDisplayableObject(iObjectId);
        // Only check collision with other enemies, skip self
        if (eObject == nullptr || eObject == this || eObject->getEnemyType() == FLYINGENEMY || eObject->getDead()) {
            continue;
        }

        // Check for collision
        if (CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
            eObject->getDrawingRegionLeft(), eObject->getDrawingRegionRight(), eObject->getDrawingRegionTop(), eObject->getDrawingRegionBottom()))
        {
            // Adjust positions to move away from the colliding object
            int dx = newX - eObject->getXCentre();
            int dy = newY - eObject->getYCentre();
            float angle = atan2(dy, dx);

            newX += iMaxSpeed * cos(angle);
            newY += iMaxSpeed * sin(angle);
        }
    }

    if (nextNode->x < ex)
        m_iDirection = 0;

    if (nextNode->x > ex)
        m_iDirection = 1;

    m_iCurrentScreenX = newX;
    m_iCurrentScreenY = newY;

    this->redrawDisplay();
    checkKill();
}


/***ShooterEnemyObject Functions***/

ShooterEnemyObject::ShooterEnemyObject(BaseEngine* pEngine, int ID)
    : EnemyObject(pEngine, 64, 64, 6, ID, BOSSENEMY)
{
    m_bPlayerInView = false;
    m_iPrevTime = m_gEngine->getModifiedTime();
    m_iState = MOVINGSTATE;
}

void ShooterEnemyObject::virtDraw()
{
    if (m_bVisible) {
        if (m_bDead)
        {
            if (m_gEngine->getModifiedTime() >= m_iPrevTime + 100)
            {
                m_iPrevTime = m_gEngine->getModifiedTime();
                m_iRenderOffset += 64;
                if (m_iRenderOffset > 384)
                {
                    m_bDeathComplete = true;
                    m_bVisible = false;
                    m_gEngine->getEnemySpawner()->killEnemy();
                    m_gEngine->drawableObjectsChanged();
                    m_gEngine->removeDisplayableObject(this);
                    delete this;
                }

            }
            switch (m_iDirection)
            {
            case 0:
                m_sprite = m_iManager->loadImage("enemiesLeft.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 960, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                break;
            case 1:
                m_sprite = m_iManager->loadImage("enemiesRight.png");
                m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 960, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
            }
        }
        else
        {
            switch (m_iState) {
            case MOVINGSTATE:
                if (m_gEngine->getModifiedTime() >= m_iPrevTime + 250)
                {
                    m_iPrevTime = m_gEngine->getModifiedTime();
                    m_iRenderOffset += 64;
                    if (m_iRenderOffset > 192)
                        m_iRenderOffset = 0;
                }
                switch (m_iDirection)
                {
                case 0:
                    m_sprite = m_iManager->loadImage("enemiesLeft.png");
                    m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 832, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                    break;
                case 1:
                    m_sprite = m_iManager->loadImage("enemiesRight.png");
                    m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 832, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                    break;
                default:
                    break;
                }
                break;
            case SHOOTINGSTATE:
                if (m_gEngine->getModifiedTime() >= m_iPrevTime + 250)
                {
                    m_iPrevTime = m_gEngine->getModifiedTime();
                    m_iRenderOffset += 64;
                    if (m_iRenderOffset > 192)
                        m_iRenderOffset = 0;
                }
                switch (m_iDirection)
                {
                case 0:
                    m_sprite = m_iManager->loadImage("enemiesLeft.png");
                    m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 576 - m_iRenderOffset, 896, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                    break;
                case 1:
                    m_sprite = m_iManager->loadImage("enemiesRight.png");
                    m_sprite.renderImageWithMask(m_gEngine->getForegroundSurface(), 0 + m_iRenderOffset, 896, getDrawingRegionLeft(), getDrawingRegionTop(), 64, 64);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
}

void ShooterEnemyObject::virtDoUpdate(int iCurrentTime)
{
    if (m_gEngine->getState()->getStateID() != RUNNINGSTATE || m_bDead == true) {
        return;
    }

    setPlayerPosition();
    GameTileManager* tm = m_gEngine->getTileManager();
    MovingPlayerObject* player = (MovingPlayerObject*)m_gEngine->getDisplayableObject(0);

    if (m_bPlayerInView)
    {
        switch (m_iState) {
        case MOVINGSTATE:
            m_iState = SHOOTINGSTATE;
            m_iRenderOffset = 0;
            break;
        default:
            break;
        }
        if (m_gEngine->getModifiedTime() >= m_iShootTimer + 1000)
        {
            m_gEngine->appendObjectToArray(new BossProjectile(getEngine(), m_iCurrentScreenX, m_iCurrentScreenY, player->getXCentre(), player->getYCentre(), true, this));
            m_iShootTimer = m_gEngine->getModifiedTime();
        }
        return;
    }
    m_iState = MOVINGSTATE;
    m_gEngine->appendObjectToArray(new BossProjectile(getEngine(), m_iCurrentScreenX, m_iCurrentScreenY, player->getXCentre(), player->getYCentre(), false, this));
    int px, py, ex, ey;


    px = tm->getMapXForScreenX(player->getXCentre());
    py = tm->getMapYForScreenY(player->getYCentre());
    ex = tm->getMapXForScreenX(getXCentre());
    ey = tm->getMapYForScreenY(getYCentre());

    if (px == ex && py == ey) {
        return;
    }
    tm->setStartAndEndNodes(ex, ey, px, py);

    GameTileManager::sNode* nextNode = tm->solveAStar();
    GameTileManager::sNode* nodeEnd = tm->getEndNode();
    GameTileManager::sNode* nodes = tm->nodeList();


    for (int x = 0; x < tm->getMapWidth(); x++)
    {
        for (int y = 0; y < tm->getMapHeight(); y++)
        {
            nodes[y * tm->getMapWidth() + x].bInPath = false;
        }
    }

    if (nodeEnd != nullptr) {
        GameTileManager::sNode* p = nodeEnd;
        while (p->parent != nullptr) {
            p->bInPath = true;
            p = p->parent;
        }
    }

    if (nextNode == nullptr)
    {
        return;
    }

    int nextNodeScreenX = nextNode->x * tm->getTileWidth();
    int nextNodeScreenY = nextNode->y * tm->getTileHeight();
    int iMaxSpeed = 2;
    double dx = nextNodeScreenX - m_iCurrentScreenX;
    double dy = nextNodeScreenY - m_iCurrentScreenY;
    double distance = sqrt(dx * dx + dy * dy);

    // If the distance is less than the movement speed, move to the next node
    if (distance <= iMaxSpeed) {
        m_iCurrentScreenX = nextNodeScreenX;
        m_iCurrentScreenY = nextNodeScreenY;
    }
    else {
        // Otherwise, move towards the next node
        double moveX = dx / distance * iMaxSpeed;
        double moveY = dy / distance * iMaxSpeed;

        m_iCurrentScreenX += moveX;
        m_iCurrentScreenY += moveY;
    }

    EnemyObject* eObject;

    for (int iObjectId = 1; iObjectId < m_gEngine->getContentCount(); iObjectId++) {
        eObject = (EnemyObject*)m_gEngine->getDisplayableObject(iObjectId);
        // Only check collision with other enemies, skip self
        if (eObject == nullptr || eObject == this || eObject->getEnemyType() == FLYINGENEMY || eObject->getDead()) {
            continue;
        }


        // Check for collision
        if (CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
            eObject->getDrawingRegionLeft(), eObject->getDrawingRegionRight(), eObject->getDrawingRegionTop(), eObject->getDrawingRegionBottom()))
        {
            // Adjust positions to move away from the colliding object
            int dx = m_iCurrentScreenX - eObject->getXCentre();
            int dy = m_iCurrentScreenY - eObject->getYCentre();
            float angle = atan2(dy, dx);

            m_iCurrentScreenX += iMaxSpeed * cos(angle);
            m_iCurrentScreenY += iMaxSpeed * sin(angle);
        }
    }
    


    if (nextNode->x < ex)
        m_iDirection = 0;

    if (nextNode->x > ex)
        m_iDirection = 1;

    this->redrawDisplay();
    checkKill();
}

void ShooterEnemyObject::checkPlayerInView()
{
}