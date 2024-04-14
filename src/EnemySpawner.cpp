#include "header.h"
#include "EnemySpawner.h"


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


EnemySpawner::EnemySpawner(GameEngine* gEngine)
{
	m_gEngine = gEngine;
	m_iTimer = 5000;
	m_iNumOfSEnemy = 0;
	m_iNumOfFEnemy = 0;
	m_iTotalNumOfEnemy = 0;
	m_prevTime = m_gEngine->getModifiedTime();
	m_iLevel = 0;
}

void EnemySpawner::activateSpawner()
{
	int l = m_gEngine->getLevel();
	switch (l) {
	case 1:
		switch (m_iLevel) {
		case 1:
			spawnSEnemy();
			break;
		case 2:
			for (int i = 0; i < 2; i++) {
				spawnSEnemy();
			}
			break;
		case 3:
			for (int i = 0; i < 2; i++) {
				spawnSEnemy();
			}
			spawnFEnemy();
			break;
		case 4:
			for (int i = 0; i < 2; i++) {
				spawnSEnemy();
			}
			spawnFEnemy();
			break;
		case 5:
			for (int i = 0; i < 1; i++) {
				spawnSEnemy();
			}
			for (int i = 0; i < 2; i++) {
				spawnFEnemy();
			}
			break;
		case 6:
			spawnShEnemy();
			break;
		default:
			MovingPlayerObject* player = (MovingPlayerObject*)m_gEngine->getDisplayableObject(0);
			player->win();
			break;
		}
		break;
	case 2:
		switch (m_iLevel) {
		case 1:
			for (int i = 0; i < 3; i++) {
				spawnSEnemy();
			}
			break;
		case 2:
			for (int i = 0; i < 5; i++) {
				spawnFEnemy();
			}
			break;
		case 3:
			for (int i = 0; i < 2; i++) {
				spawnShEnemy();
			}
			break;
		case 4:
			for (int i = 0; i < 3; i++) {
				spawnShEnemy();
			}
			break;
		default:
			MovingPlayerObject* player = (MovingPlayerObject*)m_gEngine->getDisplayableObject(0);
			player->win();
			break;
		}
		break;
	default:
		break;
	}

}

void EnemySpawner::spawnSEnemy()
{
	m_iTotalNumOfEnemy++;
	m_iNumOfSEnemy++;
	int i = m_gEngine->getContentCount();
	m_gEngine->appendObjectToArray(new StandardEnemyObject(m_gEngine, i));
}

void EnemySpawner::spawnFEnemy()
{
	m_iTotalNumOfEnemy++;
	m_iNumOfFEnemy++;
	int i = m_gEngine->getContentCount();
	m_gEngine->appendObjectToArray(new FlyingEnemyObject(m_gEngine, i));
}

void EnemySpawner::spawnShEnemy()
{
	m_iTotalNumOfEnemy++;
	m_iNumOfFEnemy++;
	int i = m_gEngine->getContentCount();
	m_gEngine->appendObjectToArray(new ShooterEnemyObject(m_gEngine, i));
}

void EnemySpawner::checkLevelChange()
{
	if (m_iTotalNumOfEnemy == 0)
	{
		m_iLevel++;
		if (m_iLevel == 11) {
			//win
			return;
		}
		activateSpawner();
	}
}

