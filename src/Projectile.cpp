#include "header.h"

#include "Projectile.h"

Projectile::Projectile(BaseEngine* pEngine, int oX, int oY, int mX, int mY, int speed)
    : DisplayableObject(oX, oY, pEngine, 6, 6, true)
{
    m_dX = mX - oX;
    m_dY = mY - oY;
    m_oX = oX;
    m_oY = oY;

    m_gEngine = (GameEngine*)this->getEngine();

    // Calculate the length of the direction vector
    float length = sqrt(m_dX * m_dX + m_dY * m_dY);

    // Normalize the direction vector to a unit vector
    float normDX = m_dX / length;
    float normDY = m_dY / length;

    m_velocityX = normDX * 1;
    m_velocityY = normDY * 1;
    m_lastUpdateTime = m_pEngine->getModifiedTime();
}

void Projectile::kill() {
    m_gEngine->drawableObjectsChanged();
    m_gEngine->removeDisplayableObject(this);
    delete this;
}

void Projectile::virtDraw()
{
    if (!m_bVisible)
        return;

    m_pEngine->drawForegroundRectangle(
        m_iCurrentScreenX, m_iCurrentScreenY,
        m_iCurrentScreenX + m_iDrawWidth - 1,
        m_iCurrentScreenY + m_iDrawHeight - 1,
        0xFF0000);
}

void Projectile::virtDoUpdate(int iCurrentTime)
{
    if (m_gEngine->getState()->getStateID() == RUNNINGSTATE) {
        // Get the elapsed time since the last update
        int deltaTime = m_pEngine->getModifiedTime() - m_lastUpdateTime; // Assume that getTime() returns the current time in seconds
        m_lastUpdateTime = m_pEngine->getModifiedTime(); // Update the last update time to the current time

        // Update the position of the projectile based on its velocity
        m_iCurrentScreenX += m_velocityX * deltaTime;
        m_iCurrentScreenY += m_velocityY * deltaTime;

        // Remove object if out of bounds

        if (m_iCurrentScreenX < 0)
        {
            kill();
            return;
        }
        if (m_iCurrentScreenX >= m_pEngine->getWindowWidth() -
            m_iDrawWidth)
        {
            kill();
            return;
        }
        if (m_iCurrentScreenY < 0)
        {
            kill();
            return;
        }
        if (m_iCurrentScreenY >= m_pEngine->getWindowHeight() -
            m_iDrawHeight)
        {
            kill();
            return;
        }

        // Remove object if hit a cover tile
        if (isOnCoverTile())
        {
            kill();
            return;
        }

        
        // Check if colliding with object
        EnemyObject* pObject;

        
        for(int i = 1; i <= m_gEngine->getContentCount(); i ++)
        {

            pObject = (EnemyObject*)m_pEngine->getDisplayableObject(i);

            if (pObject == NULL || pObject->getDead()) {
                continue;
            }


            if (CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
                pObject->getDrawingRegionLeft(), pObject->getDrawingRegionRight(), pObject->getDrawingRegionTop(), pObject->getDrawingRegionBottom()))
            {
                int iHealth = pObject->getHealth();
                iHealth--;
                if (iHealth == 0) {
                    pObject->kill();
                    return;
                }
                else {
                    pObject->setHealth(iHealth);
                }
                kill();
                return;
            }
        }
        
        
        this->redrawDisplay();
    }
}

bool Projectile::isOnCoverTile()
{
    GameTileManager* tm = m_gEngine->getTileManager();
    int mX = tm->getMapXForScreenX(m_iCurrentScreenX);
    int mY = tm->getMapYForScreenY(m_iCurrentScreenY);
    if (tm->getMapValue(mX, mY) == 1) {
        return true;
    }
    return false;
}



BossProjectile::BossProjectile(BaseEngine* pEngine, int oX, int oY, int mX, int mY, bool active, ShooterEnemyObject* origin)
    : Projectile(pEngine, oX, oY, mX, mY, 2)
{
    m_bIsActive = active;
    m_eOrigin = origin;
    if (!active)
        m_bVisible = false;
}

void BossProjectile::virtDraw()
{
    if (!m_bVisible)
        return;

    m_pEngine->drawForegroundRectangle(
        m_iCurrentScreenX, m_iCurrentScreenY,
        m_iCurrentScreenX + m_iDrawWidth - 1,
        m_iCurrentScreenY + m_iDrawHeight - 1,
        0x39FF14);
}

void BossProjectile::virtDoUpdate(int iCurrentTime)
{
    MovingPlayerObject* player = (MovingPlayerObject*)m_pEngine->getDisplayableObject(0);
    if (m_gEngine->getState()->getStateID() == RUNNINGSTATE) {
        // Get the elapsed time since the last update
        int deltaTime = m_pEngine->getModifiedTime() - m_lastUpdateTime; // Assume that getTime() returns the current time in seconds
        m_lastUpdateTime = m_pEngine->getModifiedTime(); // Update the last update time to the current time

        // Update the position of the projectile based on its velocity
        m_iCurrentScreenX += m_velocityX * deltaTime;
        m_iCurrentScreenY += m_velocityY * deltaTime;

        // Remove object if out of bounds

        if (m_iCurrentScreenX < 0)
        {
            kill();
            return;
        }
        if (m_iCurrentScreenX >= m_pEngine->getWindowWidth() -
            m_iDrawWidth)
        {
            kill();
            return;
        }
        if (m_iCurrentScreenY < 0)
        {
            kill();
            return;
        }
        if (m_iCurrentScreenY >= m_pEngine->getWindowHeight() -
            m_iDrawHeight)
        {
            kill();
            return;
        }


        // Remove object if hit a cover tile
        if (isOnCoverTile())
        {
            m_eOrigin->setPlayerInView(false);
            kill();
            return;
        }


        // Check if colliding with player

        if (CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
            player->getDrawingRegionLeft(), player->getDrawingRegionRight(), player->getDrawingRegionTop(), player->getDrawingRegionBottom()))
        {
            if (!m_bIsActive)
            {
                m_eOrigin->setPlayerInView(true);
                kill();
                return;
            }
            else
            {
                player->kill();
            }
        }


        this->redrawDisplay();
    }
}

void BossProjectile::activate()
{
}

void BossProjectile::deActivate()
{
}
