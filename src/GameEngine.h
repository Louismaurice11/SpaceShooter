#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "BaseEngine.h"
#include "Button.h"
#include "Projectile.h"
#include "GameTileManager.h"
#include "MovingObject.h"
#include "UtilCollisionDetection.h"
#include "EnemySpawner.h"
#include "TextBox.h"
#include "MenuBackdrop.h"

#include <string>
#include <list>
#include <corecrt_math.h>
#include <fstream>
#include "ColourPicker.h"

// State identifiers
#define STARTSTATE 0
#define RUNNINGSTATE 1
#define PAUSESTATE 2
#define GAMEOVERSTATE 3
#define WINSTATE 4

// Enemy identifiers
#define STANDARDENEMY 0
#define FLYINGENEMY 1
#define BOSSENEMY 2

#define TOTALENEMYCOUNT 8

class GameEngine;
class GameTileManager;
class Projectile;

class State
{
public:
    int getStateID() {
        return m_stateId;
    }

    virtual ~State() {}

    virtual void drawBackground(GameTileManager* m_tM) = 0;

    virtual void initialiseObjects() = 0;

    virtual void mouseDown(int iButton, int iX, int iY){}

    virtual void keyDown(int iKeyCode){}

    virtual void virtDrawStringsOnTop() {}

    virtual void virtMainLoopDoBeforeUpdate() {}

    virtual void copyAllBackgroundBuffer();

protected:
    GameEngine* m_gEngine;
    int m_stateId;
};

class MovingPlayerObject;

class GameEngine :
    public BaseEngine
{
public:
    GameEngine();

    ~GameEngine();

    void resetEngine();
    
    int getPlayerColour() {
        return m_iColour;
    }

    std::string getName() {
        return m_sName;
    }

    void setColour(int iColour) {
        m_iColour = iColour;
    }

    void setName(std::string name) {
        m_sName = name;
    }

    int getOffset() {
        return m_iOffset;
    }

    void setOffset(int n) {
        m_iOffset = n;
    }

    void setState(State* state);

    State* getState() {
        return m_state;
    }

    GameTileManager* getTileManager() {
        return m_tM;
    }

    EnemySpawner* getEnemySpawner() {
        return m_eS;
    }

    MovingPlayerObject* getPlayer() {
        return m_player;
    }

    int getLevel() {
        return m_iLevel;
    }

    void setLevel(int level);

    virtual void virtSetupBackgroundBuffer() override;

    virtual int virtInitialiseObjects() override;

    virtual void virtMouseDown(int iButton, int iX, int iY) override;

    virtual void virtKeyDown(int iKeyCode) override;

    virtual void virtDrawStringsOnTop() override;

    virtual void virtMainLoopDoBeforeUpdate() override;

    void copyAllBackgroundBuffer() override;

private:
    State* m_state;
    GameTileManager* m_tM;
    EnemySpawner* m_eS;
    MovingPlayerObject* m_player;
    int m_iOffset;
    int m_iLevel;
    std::string m_sName;
    int m_iColour;
};




class StartState :
    public State 
{
public:
    StartState(GameEngine* e, bool bStart);

    ~StartState() {
        delete m_colourPicker;
    }

    virtual void drawBackground(GameTileManager* m_tM) override;

    virtual void initialiseObjects() override;

    virtual void mouseDown(int iButton, int iX, int iY) override;

    virtual void keyDown(int iKeyCode) override;

    virtual void virtDrawStringsOnTop() override;

    virtual void virtMainLoopDoBeforeUpdate() override;

    virtual void copyAllBackgroundBuffer() override;

private:
    ColourPicker* m_colourPicker;
};

class RunningState :
    public State
{
public:
    RunningState(GameEngine* e, int pSID, bool bLoad);

    virtual void drawBackground(GameTileManager* m_tM) override;

    virtual void initialiseObjects() override;

    virtual void virtDrawStringsOnTop() override;

    virtual void mouseDown(int iButton, int iX, int iY) override;

    virtual void keyDown(int iKeyCode) override;

    void resumeGame();
};

class Button;
class MenuBackdrop;

class PauseState :
    public State
{
public:
    PauseState(GameEngine* e);

    ~PauseState() {
        m_gEngine->unpause();
    }
    virtual void drawBackground(GameTileManager* m_tM) override;

    virtual void initialiseObjects() override;

    virtual void virtDrawStringsOnTop() override;

    virtual void mouseDown(int iButton, int iX, int iY) override;

    virtual void keyDown(int iKeyCode) override;

private:
    Button* m_bSaveBtn;
    Button* m_bStartBtn;
    Button* m_bExitBtn;
    MenuBackdrop* m_backDrop;
};

class GameOverState :
    public State
{
public:
    GameOverState(GameEngine* e);

    ~GameOverState();

    virtual void drawBackground(GameTileManager* m_tM) override;

    virtual void initialiseObjects() override;

    virtual void mouseDown(int iButton, int iX, int iY) override;

    virtual void keyDown(int iKeyCode) override;

    virtual void virtDrawStringsOnTop() override;

    virtual void virtMainLoopDoBeforeUpdate() override;

    virtual void copyAllBackgroundBuffer() override;

};

class WinState :
    public State
{
public:
    WinState(GameEngine* e);

    ~WinState() {
    }

    virtual void drawBackground(GameTileManager* m_tM) override;

    virtual void initialiseObjects() override;

    virtual void mouseDown(int iButton, int iX, int iY) override;

    virtual void keyDown(int iKeyCode) override;

    virtual void virtDrawStringsOnTop() override;

    virtual void virtMainLoopDoBeforeUpdate() override;

    virtual void copyAllBackgroundBuffer() override;

};
#endif // !GAMEENGINE_H