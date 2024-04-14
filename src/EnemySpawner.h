#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include "GameEngine.h"

/*
L1 - 1 standard enemy

L2 - 2 standard enemy

L3 - 2 standard 1 flying

L4 - 3 Standard 2 flying

L5 - 4 Standard 3 flying

L6 - 4 standard 4 flying

L7 - 1 boss

L8 - 4 standard 4 flying

L9 - 1 boss 2 standard

L10 - 1 boss 2 standard 2 flying
*/
class EnemySpawner
{
public:
	EnemySpawner(GameEngine* gEngine);

	int getLevel()
	{
		return m_iLevel;
	}

	void setLevel(int level)
	{
		m_iLevel = level;
	}

	void activateSpawner();

	void spawnSEnemy();

	void spawnFEnemy();

	void spawnShEnemy();

	void checkLevelChange();

	void killEnemy() {
		m_iTotalNumOfEnemy--;
	}
private:
	GameEngine* m_gEngine;
	
	int m_iTimer;

	int m_iNumOfSEnemy;

	int m_iNumOfFEnemy;

	int m_iTotalNumOfEnemy;

	int m_prevTime;

	int m_iLevel;
};


#endif // !ENEMYSPAWNER_H

