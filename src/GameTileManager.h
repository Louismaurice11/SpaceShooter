#ifndef GAMETILEMANAGER_H
#define GAMETILEMANAGER_H

#include "TileManager.h"
#include "GameEngine.h"

class GameEngine;

class GameTileManager :
    public TileManager
{
public:

	struct sNode
	{
		bool bCover = false;
		bool bVisited = false;
		bool bInPath = false;
		float fGlobalGoal;
		float fLocalGoal;
		int x;
		int y;
		std::vector<sNode*> vecNeighbours;
		sNode* parent;
	};

	GameTileManager(GameEngine* e);

	~GameTileManager()
	{
		try {
			delete[] m_nodes;
		}catch(int num) {
			std::cout << "ERROR: cannot delete nodes" << std::endl;
		}
		delete m_iManager;
	}

	void setLevel();

	sNode* solveAStar();

	sNode* nodeList() {
		return m_nodes;
	}

	void generateMap();

	void setStartAndEndNodes(int iMapX1, int iMapY1, int iMapX2, int iMapY2)
	{
		m_nodeStart = &m_nodes[iMapY1 * m_iMapWidth + iMapX1];
		m_nodeEnd = &m_nodes[iMapY2 * m_iMapWidth + iMapX2];
	}

	sNode* getEndNode() {
		return m_nodeEnd;
	}
	
    virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

private:

	GameEngine* m_gEngine;

	sNode* m_nodes = nullptr;

	sNode* m_nodeStart = nullptr;

	sNode* m_nodeEnd = nullptr;

	ImageManager* m_iManager;

	SimpleImage m_tiles;

	int iProb = 20;

	std::string m_sLevel;
};


#endif // !GAMETILEMANAGER_H
