#include "header.h"
#include "GameEngine.h"

/***State Functions***/

void State::copyAllBackgroundBuffer() {
	m_gEngine->getForegroundSurface()->copyEntireSurface(m_gEngine->getBackgroundSurface());
}


/***Game Engine Functions***/

GameEngine::GameEngine()
{
	m_iLevel = 1;
	m_iOffset = 0;
	m_tM = new GameTileManager(this);
	m_eS = new EnemySpawner(this);
	m_tM->generateMap();
	m_iWindowHeight = 64*20;
	m_iWindowWidth = 64*15;
	m_player = new MovingPlayerObject(this, m_eS, 0);
	m_state = new StartState(this, true);
	m_sName = "";
	m_iColour = 0;
}

GameEngine::~GameEngine()
{
	delete m_tM;
	delete m_state;
	delete m_eS; 
	removeDisplayableObject(m_player);
	delete m_player;
	destroyOldObjects(true);
}

void GameEngine::resetEngine()
{
	delete m_eS;
	delete m_tM;
	delete m_player;
	m_iOffset = 0;
	m_tM = new GameTileManager(this);
	m_eS = new EnemySpawner(this);
	m_tM->generateMap();
	m_player = new MovingPlayerObject(this, m_eS, m_iColour);
}

void GameEngine::setState(State* state)
{
	delete m_state;
	m_state = state;
	if (state->getStateID() == STARTSTATE)
	{
		resetEngine();
	}
	redrawDisplay();
}

void GameEngine::setLevel(int level) {
	m_iLevel = level;
	m_tM->setLevel();
}

void GameEngine::virtSetupBackgroundBuffer()
{
	m_state->drawBackground(m_tM);
}

int GameEngine::virtInitialiseObjects()
{
	m_state->initialiseObjects();
	return 0;
}

void GameEngine::virtMouseDown(int iButton, int iX, int iY)
{
	m_state->mouseDown(iButton, iX, iY);
}

void GameEngine::virtKeyDown(int iKeyCode)
{
	m_state->keyDown(iKeyCode);
}

void GameEngine::virtDrawStringsOnTop()
{
	m_state->virtDrawStringsOnTop();
}

void GameEngine::virtMainLoopDoBeforeUpdate()
{
	m_state->virtMainLoopDoBeforeUpdate();
	this->redrawDisplay();
}

void GameEngine::copyAllBackgroundBuffer()
{
	m_state->copyAllBackgroundBuffer();
}



/***StartState Functions***/

StartState::StartState(GameEngine* e, bool bStart) {
	m_stateId = STARTSTATE;
	m_gEngine = e;
	m_colourPicker = new ColourPicker();
	initialiseObjects();
	if (!bStart)
	{
		drawBackground(e->getTileManager());
		m_gEngine->redrawDisplay();
	}
}

void StartState::drawBackground(GameTileManager* m_tM)
{
	m_gEngine->lockBackgroundForDrawing();
	m_gEngine->fillBackground(0x404040);
	SimpleImage image = ImageManager::loadImage("startbackground.png", true);
	image.renderImage(m_gEngine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());
	m_gEngine->unlockBackgroundForDrawing();
}

void StartState::initialiseObjects()
{
	m_gEngine->drawableObjectsChanged();

	m_gEngine->destroyOldObjects(true);

	m_gEngine->createObjectArray(4);

	m_gEngine->storeObjectInArray(0, new MenuBackdrop((m_gEngine->getWindowHeight()/2) - 120, 70, m_gEngine));

	m_gEngine->storeObjectInArray(1, new Button(500, 400, m_gEngine, 288, 96, "Start"));// Start game button

	m_gEngine->storeObjectInArray(2, new TextBox(450, 220, m_gEngine, 400, 40));// Name text box

	m_gEngine->storeObjectInArray(3, new Button(500, 500, m_gEngine, 300, 40, "Continue"));// Name text box

	m_gEngine->drawableObjectsChanged();
}

void StartState::mouseDown(int iButton, int iX, int iY)
{

	Button* startBtn = (Button*)m_gEngine->getDisplayableObject(1);
	TextBox* tb = (TextBox*) m_gEngine->getDisplayableObject(2);
	Button* conBtn = (Button*)m_gEngine->getDisplayableObject(3);
	std::string name = tb->getName();
	if (iButton == SDL_BUTTON_LEFT) {
		if (m_colourPicker->isValidTilePosition(iX, iY))
		{
			int x = m_colourPicker->getMapXForScreenX(iX);
			m_colourPicker->setColour(x);
			m_colourPicker->drawAllTiles(m_gEngine, m_gEngine->getForegroundSurface());
			m_gEngine->getPlayer()->setColour(x);
			m_gEngine->setColour(x);
		}
		if (startBtn->isValidPosition(iX, iY)) {
			if (name == "")
				return;
			m_gEngine->getPlayer()->setColour(m_colourPicker->getColour());
			m_gEngine->setColour(m_colourPicker->getColour());
			m_gEngine->setName(name);
			m_gEngine->setState(new RunningState(m_gEngine, m_gEngine->getState()->getStateID(), false));
			return;
		}
		if (conBtn->isValidPosition(iX, iY)) {
			std::ifstream file("level.txt");	
			std::string firstLine, secondLine, thirdLine;

			getline(file, firstLine);
			getline(file, secondLine);
			getline(file, thirdLine);

			file.close();
			m_gEngine->setName(secondLine);
			m_gEngine->setColour(std::stoi(thirdLine));
			m_gEngine->getPlayer()->setColour(m_gEngine->getPlayerColour());
			m_gEngine->setState(new RunningState(m_gEngine, m_gEngine->getState()->getStateID(), true));
			return;
		}

		
		if (tb->isValidPosition(iX, iY)) {
			tb->setIsClicked(true);
		}
		else {
			tb->setIsClicked(false);
		}
		
		
	}
}

void StartState::keyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_ESCAPE) {
		m_gEngine->setExitWithCode(0);
	}
}

void StartState::virtDrawStringsOnTop()
{
	m_colourPicker->setTopLeftPositionOnScreen(500, 300);
	m_gEngine->lockForegroundForDrawing();
	m_colourPicker->drawAllTiles(m_gEngine, m_gEngine->getForegroundSurface());
	m_gEngine->drawForegroundString(420, 100, "Robo Shooter Ultimate!!!", 0xffffff, m_gEngine->getFont("m5x7.ttf", 60));
	m_gEngine->drawForegroundString(545, 272, "Pick colour:", 0xffffff, m_gEngine->getFont("m5x7.ttf", 48));
	m_gEngine->unlockForegroundForDrawing();
}

void StartState::virtMainLoopDoBeforeUpdate()
{
	m_gEngine->setOffset((m_gEngine->getOffset() + 1) % m_gEngine->getWindowHeight());
}



void StartState::copyAllBackgroundBuffer()
{
	m_gEngine->getForegroundSurface()->copyRectangleFrom(m_gEngine->getBackgroundSurface(), 0, 0,
		m_gEngine->getWindowWidth(), m_gEngine->getWindowHeight(), 0, m_gEngine->getOffset());
	m_gEngine->getForegroundSurface()->copyRectangleFrom(m_gEngine->getBackgroundSurface(), 0, m_gEngine->getWindowHeight() - m_gEngine->getOffset(),
		m_gEngine->getWindowWidth(), m_gEngine->getWindowHeight(), 0, m_gEngine->getOffset() - m_gEngine->getWindowHeight());
}




/***RunningState Functions***/

RunningState::RunningState(GameEngine* e, int pSID, bool bLoad) {
	m_stateId = RUNNINGSTATE;
	m_gEngine = e;

	int l = m_gEngine->getLevel();

	if (pSID == GAMEOVERSTATE)
		m_gEngine->resetEngine();
	if (pSID != PAUSESTATE) {
		if (bLoad == true)
		{
			std::ifstream file("level.txt");
			file >> l;
			file.close();
			m_gEngine->setLevel(l);
		}
		else
		{
			m_gEngine->setLevel(1);
		}
		e->getTileManager()->generateMap();
		e->getTileManager()->drawAllTiles(e, e->getBackgroundSurface());
		initialiseObjects();
		drawBackground(e->getTileManager());
		m_gEngine->redrawDisplay();
	}
	else {
		//drawBackground(e->getTileManager());
		m_gEngine->redrawDisplay();
	}
}

void RunningState::drawBackground(GameTileManager* m_tM)
{
	m_gEngine->lockBackgroundForDrawing();
	m_gEngine->fillBackground(0x000000);
	m_tM->setTopLeftPositionOnScreen(0, 0);
	for (int i = 0; i < m_tM->getMapWidth(); i++)
		for (int j = 0; j < m_tM->getMapHeight(); j++)
			m_tM->setMapValue(i, j, j + i);
	m_tM->setTopLeftPositionOnScreen(0, 2);
	m_gEngine->redrawDisplay();
	m_gEngine->getTileManager()->drawAllTiles(m_gEngine, m_gEngine->getBackgroundSurface());
	char buf[128];
	sprintf(buf, "Level: %d", m_gEngine->getLevel());
	m_gEngine->drawBackgroundString(800, 7, buf, 0x00000, m_gEngine->getFont("pixel-inversions.ttf", 40));
	m_gEngine->unlockBackgroundForDrawing();

}

void RunningState::initialiseObjects()
{
	m_gEngine->drawableObjectsChanged();

	m_gEngine->destroyOldObjects(true);

	m_gEngine->createObjectArray(TOTALENEMYCOUNT + 1);

	m_gEngine->storeObjectInArray(0, m_gEngine->getPlayer());

	m_gEngine->drawableObjectsChanged();
}

void RunningState::virtDrawStringsOnTop()
{
}


void RunningState::mouseDown(int iButton, int iX, int iY)
{
	MovingPlayerObject* obj = (MovingPlayerObject*)m_gEngine->getDisplayableObject(0);
	m_gEngine->drawableObjectsChanged();
	m_gEngine->appendObjectToArray(new Projectile(m_gEngine, obj->getXCentre(), obj->getYCentre()+16, iX, iY, 3));
}

void RunningState::keyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_ESCAPE) {
		m_gEngine->setState(new PauseState(m_gEngine));
	}
}

void RunningState::resumeGame()
{
}


/***PauseState Functions***/

PauseState::PauseState(GameEngine* e) {
	m_stateId = PAUSESTATE;
	m_gEngine = e;
	initialiseObjects();
	drawBackground(e->getTileManager());
	m_gEngine->redrawDisplay();
	m_gEngine->pause();
};

void PauseState::drawBackground(GameTileManager* m_tM)
{
}

void PauseState::initialiseObjects()
{
	m_gEngine->drawableObjectsChanged();
	m_backDrop = new MenuBackdrop((m_gEngine->getWindowHeight() / 2) - 120, 200, m_gEngine);
	m_bSaveBtn = new Button(500, 300, m_gEngine, 200, 30, "Save");
	m_bStartBtn = new Button(500, 400, m_gEngine, 200, 30, "Start menu");
	m_bExitBtn = new Button(500, 500, m_gEngine, 200, 30, "Exit");
	m_gEngine->appendObjectToArray(m_backDrop);
	m_gEngine->appendObjectToArray(m_bSaveBtn);
	m_gEngine->appendObjectToArray(m_bStartBtn);
	m_gEngine->appendObjectToArray(m_bExitBtn);
	m_gEngine->drawableObjectsChanged();
}

void PauseState::virtDrawStringsOnTop()
{
	m_gEngine->drawForegroundString(550, 225, "Pause menu:", 0xffffff, m_gEngine->getFont("m5x7.ttf", 48));
}

void PauseState::mouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (m_bSaveBtn->isValidPosition(iX, iY))
		{
			std::ofstream file;
			file.open("level.txt", std::ofstream::out | std::ofstream::trunc);
			file << m_gEngine->getLevel() << std::endl;
			file << m_gEngine->getName() << std::endl;
			file << m_gEngine->getPlayerColour() << std::endl;
			file.close();
		}

		if (m_bStartBtn->isValidPosition(iX, iY))
		{
			m_gEngine->removeDisplayableObject(m_gEngine->getDisplayableObject(0));
			m_gEngine->destroyOldObjects(true);
			m_gEngine->setState(new StartState(m_gEngine, false));
			return;
		}

		if (m_bExitBtn->isValidPosition(iX, iY))
		{
			exit(0);
		}
	}
}

void PauseState::keyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_ESCAPE) {
		m_gEngine->removeDisplayableObject(m_bSaveBtn);
		m_gEngine->removeDisplayableObject(m_backDrop);
		m_gEngine->removeDisplayableObject(m_bStartBtn);
		m_gEngine->removeDisplayableObject(m_bExitBtn);
		delete m_bSaveBtn;
		delete m_backDrop;
		delete m_bStartBtn;
		delete m_bExitBtn;
		m_gEngine->setState(new RunningState(m_gEngine, m_stateId, false));
	}
}


/***GameOverState ***/

GameOverState::GameOverState(GameEngine* e) {
	m_stateId = GAMEOVERSTATE;
	m_gEngine = e;
	initialiseObjects();
	drawBackground(e->getTileManager());
	m_gEngine->redrawDisplay();
}

GameOverState::~GameOverState()
{
}

void GameOverState::drawBackground(GameTileManager* m_tM)
{
	m_gEngine->fillBackground(0xFFFF00);
	SimpleImage image = ImageManager::loadImage("startbackground.png", true);
	m_gEngine->lockBackgroundForDrawing();
	image.renderImage(m_gEngine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());
	m_gEngine->unlockBackgroundForDrawing();
}

void GameOverState::initialiseObjects()
{
	m_gEngine->drawableObjectsChanged();

	m_gEngine->destroyOldObjects(true);

	m_gEngine->createObjectArray(4);

	m_gEngine->storeObjectInArray(0, new MenuBackdrop((m_gEngine->getWindowHeight() / 2) - 120, 200, m_gEngine));

	m_gEngine->storeObjectInArray(1, new Button(500, 300, m_gEngine, 200, 30, "Load save"));

	m_gEngine->storeObjectInArray(2, new Button(500, 400, m_gEngine, 200, 30, "Start menu"));

	m_gEngine->storeObjectInArray(3, new Button(500, 500, m_gEngine, 200, 30, "Exit game"));

	m_gEngine->drawableObjectsChanged();
}

void GameOverState::mouseDown(int iButton, int iX, int iY)
{
	for (int i = 1; i < 4; i++) {
		Button* btn = (Button*) m_gEngine->getDisplayableObject(i);
		if (btn == nullptr)
			return;
		if (btn->isValidPosition(iX, iY))
		{
			m_gEngine->destroyOldObjects(true);
			switch (i) {
			case 1:
				m_gEngine->setState(new RunningState(m_gEngine, GAMEOVERSTATE, true));
				return;
				break;
			case 2:
				m_gEngine->setState(new StartState(m_gEngine, true));
				return;
				break;
			case 3:
				exit(0);
				break;
			default:
				break;
			}
		}
	}
}

void GameOverState::keyDown(int iKeyCode)
{
}

void GameOverState::virtDrawStringsOnTop()
{
	m_gEngine->drawForegroundString(550, 225, "Game Over!!!", 0xffffff, m_gEngine->getFont("m5x7.ttf", 48));
}

void GameOverState::virtMainLoopDoBeforeUpdate()
{
	m_gEngine->setOffset((m_gEngine->getOffset() + 1) % m_gEngine->getWindowHeight());
}

void GameOverState::copyAllBackgroundBuffer()
{
	m_gEngine->getForegroundSurface()->copyRectangleFrom(m_gEngine->getBackgroundSurface(), 0, 0,
		m_gEngine->getWindowWidth(), m_gEngine->getWindowHeight(), 0, m_gEngine->getOffset());
	m_gEngine->getForegroundSurface()->copyRectangleFrom(m_gEngine->getBackgroundSurface(), 0, m_gEngine->getWindowHeight() - m_gEngine->getOffset(),
		m_gEngine->getWindowWidth(), m_gEngine->getWindowHeight(), 0, m_gEngine->getOffset() - m_gEngine->getWindowHeight());
}



/***WinState Functions***/

WinState::WinState(GameEngine* e) {
	m_stateId = WINSTATE;
	m_gEngine = e;
	initialiseObjects();
	drawBackground(e->getTileManager());
	m_gEngine->redrawDisplay();
}

void WinState::drawBackground(GameTileManager* m_tM)
{
	m_gEngine->fillBackground(0xFFFF00);
	SimpleImage image = ImageManager::loadImage("startbackground.png", true);
	m_gEngine->lockBackgroundForDrawing();
	image.renderImage(m_gEngine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());
	m_gEngine->unlockBackgroundForDrawing();
}

void WinState::initialiseObjects()
{
	m_gEngine->drawableObjectsChanged();

	m_gEngine->destroyOldObjects(true);

	m_gEngine->createObjectArray(4);

	m_gEngine->storeObjectInArray(0, new MenuBackdrop((m_gEngine->getWindowHeight() / 2) - 120, 200, m_gEngine));

	switch (m_gEngine->getLevel()) {
	case 1:
		m_gEngine->storeObjectInArray(1, new Button(500, 300, m_gEngine, 200, 30, "Next level"));
		break;
	case 2:
		m_gEngine->storeObjectInArray(1, new Button(500, 300, m_gEngine, 200, 30, "Restart"));
		break;
	default:
		break;
	}

	m_gEngine->storeObjectInArray(2, new Button(500, 400, m_gEngine, 200, 30, "Start menu"));

	m_gEngine->storeObjectInArray(3, new Button(500, 500, m_gEngine, 200, 30, "Exit game"));

	m_gEngine->drawableObjectsChanged();
	
}

void WinState::mouseDown(int iButton, int iX, int iY)
{
	for (int i = 1; i <= 3; i++) {
		Button* btn = (Button*)m_gEngine->getDisplayableObject(i);
		if (btn->isValidPosition(iX, iY))
		{
			m_gEngine->destroyOldObjects(true);
			switch (i) {
			case 1:
				if (m_gEngine->getLevel() < 2)
				{
					std::ofstream file;
					file.open("level.txt", std::ofstream::out | std::ofstream::trunc);
					file << m_gEngine->getLevel() + 1 << std::endl;
					file << m_gEngine->getName() << std::endl;
					file << m_gEngine->getPlayerColour() << std::endl;
					file.close();
					m_gEngine->setLevel(2);
				}
				else
				{
					std::ofstream file;
					file.open("level.txt", std::ofstream::out | std::ofstream::trunc);
					file << "1" << std::endl;
					file << m_gEngine->getName() << std::endl;
					file << m_gEngine->getPlayerColour() << std::endl;
					file.close();
					m_gEngine->setLevel(1);
				}
				m_gEngine->setState(new RunningState(m_gEngine, GAMEOVERSTATE, true));
				return;
				break;
			case 2:
				m_gEngine->setState(new StartState(m_gEngine, true));
				return;
				break;
			case 3:
				exit(0);
				break;
			default:
				break;
			}
		}
	}
}

void WinState::keyDown(int iKeyCode)
{
}

void WinState::virtDrawStringsOnTop()
{
	m_gEngine->drawForegroundString(550, 225, "You Won!!!", 0xffffff, m_gEngine->getFont("m5x7.ttf", 48));
}

void WinState::virtMainLoopDoBeforeUpdate()
{
	m_gEngine->setOffset((m_gEngine->getOffset() + 1) % m_gEngine->getWindowHeight());
}

void WinState::copyAllBackgroundBuffer()
{
	m_gEngine->getForegroundSurface()->copyRectangleFrom(m_gEngine->getBackgroundSurface(), 0, 0,
		m_gEngine->getWindowWidth(), m_gEngine->getWindowHeight(), 0, m_gEngine->getOffset());
	m_gEngine->getForegroundSurface()->copyRectangleFrom(m_gEngine->getBackgroundSurface(), 0, m_gEngine->getWindowHeight() - m_gEngine->getOffset(),
		m_gEngine->getWindowWidth(), m_gEngine->getWindowHeight(), 0, m_gEngine->getOffset() - m_gEngine->getWindowHeight());
}