#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameEngine.h"
#include <corecrt_math.h>

class GameEngine;

class EnemyObject;

class Projectile :
    public DisplayableObject
{
public:
    Projectile(BaseEngine* pEngine, int oX, int oY, int mX, int mY, int speed);

    ~Projectile() {
        printf("deleted projectile\n");
    }

    void kill();

    virtual void virtDraw() override;

    virtual void virtDoUpdate(int iCurrentTime) override;

    bool isOnCoverTile();

    bool isCollided(EnemyObject* obj);

protected:
    int m_dX;
    int m_dY;
    int m_oX;
    int m_oY;
    double m_velocityX;
    double m_velocityY;
    double m_lastUpdateTime;
    GameEngine* m_gEngine;
};

class ShooterEnemyObject;

class BossProjectile :
    public Projectile
{
public:
    BossProjectile(BaseEngine* pEngine, int oX, int oY, int mX, int mY, bool active, ShooterEnemyObject* origin);

    virtual void virtDraw() override;

    virtual void virtDoUpdate(int iCurrentTime) override;

    void activate();

    void deActivate();

private:
    int m_bIsActive;
    ShooterEnemyObject* m_eOrigin;
};


#endif // !PROJECTILE_H



