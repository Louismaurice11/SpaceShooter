#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "GameEngine.h"

#define MOVINGSTATE 0
#define SHOOTINGSTATE 1

class MovingObject :
	public DisplayableObject
{
public:
	MovingObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, int health);

	virtual ~MovingObject(){
		delete m_iManager;
	}

	void setHealth(int iHealth) { m_health = iHealth; }

	SimpleImage getSprite() {
		return m_sprite;
	}

	int getHealth() { return m_health; }

	virtual void virtDraw() override = 0;

	virtual void virtDoUpdate(int iCurrentTime) override = 0;

	void kill() {
		m_bDead = true;
	}

protected:
	GameEngine* m_gEngine;

	int m_health;

	int m_iDirection;

	int m_bStill;

	int m_iPrevTime, m_iShootTimer, m_iRenderOffset;

	bool m_bDead, m_bDeathComplete;

	ImageManager* m_iManager;

	SimpleImage m_sprite;
};

class EnemySpawner;

class MovingPlayerObject :
	public MovingObject
{
public:
	MovingPlayerObject(BaseEngine* pEngine, EnemySpawner* enemySpawner, int colour);

	~MovingPlayerObject() {
		printf("deleted player\n");
	}

	void setColour(int iColour) {
		m_iColour = iColour;
	}

	void win() {
		m_bWin = true;
	}
	
	//void kill() {}

	bool checkCoverTop(int iSpeed);

	bool checkCoverBottom(int iSpeed);

	bool checkCoverLeft(int iSpeed);

	bool checkCoverRight(int iSpeed);

	void setTrueSides()
	{
		int iTPixel = m_sprite.getPixelColour(0, 0);
		m_trueTop = 100;
		m_trueBottom = 0;
		m_trueLeft = 100;
		m_trueRight = 0;

		// loop through all pixels in the sprite
		for (int y = 0; y < 64; y++) {
			for (int x = 0; x < 64; x++) {
				int pixel = m_sprite.getPixelColour(x, y);
				if (pixel != iTPixel) {
					// pixel is not transparent, update the true sides
					if (x < m_trueLeft) {
						m_trueLeft = x;
					}
					if (x > m_trueRight) {
						m_trueRight = x;
					}
					if (y < m_trueTop) {
						m_trueTop = y;
					}
					if (y > m_trueBottom) {
						m_trueBottom = y;
					}
				}
			}
		}

		m_trueRight = 64 - m_trueRight;
		m_trueBottom = 64 - m_trueBottom;
	}


	virtual void virtDraw() override;

	virtual void virtDoUpdate(int iCurrentTime) override;

private:
	EnemySpawner* m_enemySpawner;

	bool m_bWin;

	bool m_bSprint;

	int m_iSkin, m_iColour;

	int m_trueTop, m_trueBottom, m_trueLeft, m_trueRight;
};

class EnemyObject :
	public MovingObject
{
public:
	EnemyObject(BaseEngine* pEngine, int width, int height, int health, int ID, int type)
		: MovingObject( 0, 0, pEngine, width, height, health)
	{
		m_playerX = 0; // initialize to 0
		m_playerY = 0; // initialize to 0
		m_objectID = ID;
		m_enemyType = type;
		m_bIsDead = false;

		// randomly choose one of the four edges of the screen, excluding the corners
		int edge = rand() % 2; // 0: top/bottom, 1: left/right

		// set initial position based on chosen edge
		switch (edge)
		{
		case 0: // top/bottom
			if (rand() % 2 == 0) { // top
				m_iCurrentScreenX = rand() % (m_pEngine->getWindowWidth() - m_iDrawWidth);
				m_iCurrentScreenY = 0;
			}
			else { // bottom
				m_iCurrentScreenX = rand() % (m_pEngine->getWindowWidth() - m_iDrawWidth);
				m_iCurrentScreenY = m_pEngine->getWindowHeight() - m_iDrawHeight;
			}
			break;
		case 1: // left/right
			if (rand() % 2 == 0) { // left
				m_iCurrentScreenX = 0;
				m_iCurrentScreenY = rand() % (m_pEngine->getWindowHeight() - m_iDrawHeight);
			}
			else { // right
				m_iCurrentScreenX = m_pEngine->getWindowWidth() - m_iDrawWidth;
				m_iCurrentScreenY = rand() % (m_pEngine->getWindowHeight() - m_iDrawHeight);
			}
			break;
		}
	}

	virtual ~EnemyObject() {
	}

	int getEnemyType() { return m_enemyType; }

	bool getDead() { return m_bDead; }

	void setPlayerPosition();

	void checkKill();

	void killEnemy();

	virtual void virtDraw() override = 0;

	virtual void virtDoUpdate(int iCurrentTime) override = 0;

protected:

	int m_playerX;

	int m_playerY;

	int m_objectID;

	int m_enemyType;

	bool m_bIsDead;
};

class FlyingEnemyObject :
	public EnemyObject
{
public:
	FlyingEnemyObject(BaseEngine* pEngine, int ID);

    ~FlyingEnemyObject() { std::cout << "deleted enemy" << std::endl; }

	virtual void virtDraw() override;

	virtual void virtDoUpdate(int iCurrentTime) override;

private:
    int m_speedChangeTimer;

    int m_time;

	int m_speed;
};

class StandardEnemyObject :
	public EnemyObject
{
public:
	StandardEnemyObject(BaseEngine* pEngine, int ID);

	~StandardEnemyObject() { std::cout << "deleted enemy" << std::endl; };

	virtual void virtDraw() override;

	virtual void virtDoUpdate(int iCurrentTime) override;

private:
};

class ShooterEnemyObject :
	public EnemyObject
{
public:
	ShooterEnemyObject(BaseEngine* pEngine, int ID);

	~ShooterEnemyObject() { std::cout << "deleted enemy" << std::endl; };

	virtual void virtDraw() override;

	virtual void virtDoUpdate(int iCurrentTime) override;

	void checkPlayerInView();

	void setPlayerInView(bool b)
	{
		m_bPlayerInView = b;
	}
private:
	bool m_bPlayerInView;  
	int m_iPrevTime;
	int m_iState;
};


#endif // !MOVINGOBJECT_H