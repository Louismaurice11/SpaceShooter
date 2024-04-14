#include "header.h"
#include "GameTileManager.h"
GameTileManager::GameTileManager(GameEngine* e)
    : TileManager(64, 64, 20, 15)
{
    switch (e->getLevel()) {
    case 1:
        m_sLevel = "map1.txt";
        break;
    case 2:
        m_sLevel = "map2.txt";
        break;
    default:
        break;
    }
    m_iManager = new ImageManager();
    m_tiles = m_iManager->loadImage("tileset.png");
    // Setup nodes
    m_nodes = new sNode[m_iMapWidth * m_iMapHeight];
    for (int x = 0; x < m_iMapWidth; x++)
    {
        for (int y = 0; y < m_iMapHeight; y++)
        {
            m_nodes[(y * m_iMapWidth) + x].x = x;
            m_nodes[(y * m_iMapWidth) + x].y = y;
            m_nodes[(y * m_iMapWidth) + x].bCover = false;
            m_nodes[(y * m_iMapWidth) + x].parent = nullptr;
            m_nodes[(y * m_iMapWidth) + x].bVisited = false;
            m_nodes[(y * m_iMapWidth) + x].bVisited = false;
        }
    }

    // Create connections
    for (int x = 0; x < m_iMapWidth; x++)
    {
        for (int y = 0; y < m_iMapHeight; y++)
        {
            if (y > 0)
                m_nodes[(y * m_iMapWidth) + x].vecNeighbours.push_back(&m_nodes[(y - 1) * m_iMapWidth + (x + 0)]);
            if (y < m_iMapHeight - 1)
                m_nodes[(y * m_iMapWidth) + x].vecNeighbours.push_back(&m_nodes[(y + 1) * m_iMapWidth + (x + 0)]);
            if (x > 0)
                m_nodes[(y * m_iMapWidth) + x].vecNeighbours.push_back(&m_nodes[(y + 0) * m_iMapWidth + (x - 1)]);
            if (x < m_iMapWidth - 1)
                m_nodes[(y * m_iMapWidth) + x].vecNeighbours.push_back(&m_nodes[(y + 0) * m_iMapWidth + (x + 1)]);
        }
    }

    m_nodeStart = &m_nodes[(m_iMapHeight / 2) * m_iMapWidth + 1];
    m_nodeEnd = &m_nodes[(m_iMapHeight / 2) * m_iMapWidth + m_iMapHeight - 2];
    m_gEngine = e;
}

void GameTileManager::virtDrawTileAt(
    BaseEngine* pEngine, 
    DrawingSurface* pSurface,
    int iMapX, int iMapY,
    int iStartPositionScreenX, int iStartPositionScreenY) const
{
    // Determine tile color based on whether it is adjacent to a white tile
    //m_pData[iMapX + iMapY * m_iMapWidth] = 0;

    int ran = rand() % 10;

    std::ifstream file(m_sLevel);
    int n;
    for (int iY = 0; iY < getMapHeight(); iY++) {
        for (int iX = 0; iX < getMapWidth(); iX++) {
            char c;
            file >> c;
            if (iY == iMapY && iX == iMapX)
            {
                switch (c) {
                case '*':
                    
                    switch (ran) {
                    case 0:
                        m_tiles.renderImage(pSurface, 192, 640, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 1:
                        m_tiles.renderImage(pSurface, 256, 640, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 2:
                        m_tiles.renderImage(pSurface, 192, 704, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 3:
                        m_tiles.renderImage(pSurface, 256, 704, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 4:
                        m_tiles.renderImage(pSurface, 192, 640, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 5:
                        m_tiles.renderImage(pSurface, 256, 640, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 6:
                        m_tiles.renderImage(pSurface, 192, 704, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 7:
                        m_tiles.renderImage(pSurface, 256, 704, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 8:
                        m_tiles.renderImage(pSurface, 256, 704, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 9:
                        m_tiles.renderImage(pSurface, 256, 704, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    default:
                        break;
                    }
                    
                    if (m_nodes[iMapY * m_iMapWidth + iMapX].bCover == false && m_nodes[(iMapY - 1) * m_iMapWidth + iMapX].bCover == true && iMapY > 0) {
                        
                        if (ran % 2 == 0)
                        {
                            m_tiles.renderImage(pSurface, 640, 640, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        }
                        else
                        {
                            m_tiles.renderImage(pSurface, 704, 640, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        }
                        
                        return;
                    }
                    m_pData[iMapX + iMapY * m_iMapWidth] = 0;
                    break;
                case 'w':
                    switch (ran) {
                    case 0:
                        m_tiles.renderImage(pSurface, 640, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 1:
                        m_tiles.renderImage(pSurface, 640, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 2:
                        m_tiles.renderImage(pSurface, 832, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 3:
                        m_tiles.renderImage(pSurface, 832, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 4:
                        m_tiles.renderImage(pSurface, 832, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 5:
                        m_tiles.renderImage(pSurface, 896, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 6:
                        m_tiles.renderImage(pSurface, 1280, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 7:
                        m_tiles.renderImage(pSurface, 640, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 8:
                        m_tiles.renderImage(pSurface, 640, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    case 9:
                        m_tiles.renderImage(pSurface, 640, 576, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                        break;
                    default:
                        break;
                    }
                    
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    break;
                case '.':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 896, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '+':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 896, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '7':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 640, 384, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '8':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 704, 384, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '9':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 768, 384, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '4':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 640, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '5':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 704, 448, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '6':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 768, 384, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '1':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 640, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '2':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 704, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '3':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 768, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '-':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 896, 384, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '<':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 1024, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '>':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 1152, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '@':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 1088, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case ';':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 1024, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '#':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 1152, 512, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                case '/':
                    m_pData[iMapX + iMapY * m_iMapWidth] = 1;
                    m_tiles.renderImage(pSurface, 1024, 384, iStartPositionScreenX, iStartPositionScreenY, 64, 64);
                    break;
                default:
                    break;
                }
            }
        }
    }

    file.close();
}



void GameTileManager::setLevel() {
    switch (m_gEngine->getLevel()) {
    case 1:
        m_sLevel = "map1.txt";
        break;
    case 2:
        m_sLevel = "map2.txt";
        break;
    default:
        break;
    }
}

GameTileManager::sNode* GameTileManager::solveAStar()
{
    sNode* nextNode = nullptr;
    // Reset Navigational Graph - default all node states
    for (int x = 0; x < m_iMapWidth; x++)
    {
        for (int y = 0; y < m_iMapHeight; y++)
        {
            m_nodes[y * m_iMapWidth + x].bVisited = false; 
            m_nodes[y * m_iMapWidth + x].fGlobalGoal = INFINITY;
            m_nodes[y * m_iMapWidth + x].fLocalGoal = INFINITY;
            m_nodes[y * m_iMapWidth + x].parent = nullptr;
        }
    }


    auto distance = [](sNode* a, sNode* b) // For convenience
    {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    };

    auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
    {
        return distance(a, b);
    };

    // Setup starting conditions
    sNode* nodeCurrent = m_nodeStart;
    m_nodeStart->fLocalGoal = 0.0f;
    m_nodeStart->fGlobalGoal = heuristic(m_nodeStart, m_nodeEnd);

    std::list<sNode*> listNotTestedNodes;
    listNotTestedNodes.push_back(m_nodeStart);

    while (!listNotTestedNodes.empty() && nodeCurrent != m_nodeEnd)
    {
        // Sort untested nodes by global goal
        listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });
        
        while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
            listNotTestedNodes.pop_front();

        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->bVisited = true;

        // Check each of the nodes neighbours
        for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
        {
            if (!nodeNeighbour->bVisited && nodeNeighbour->bCover == 0)
                listNotTestedNodes.push_back(nodeNeighbour);
            
            // Calculate local goals
            float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);
            if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
            {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

                nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, m_nodeEnd);
            }
        }
    }

    if (m_nodeEnd != nullptr) {
        sNode* p = m_nodeEnd;
        while (p->parent != nullptr) {
            if (p->parent == m_nodeStart)
            {
                nextNode = p;
            }
            p->bInPath = true;
            p = p->parent;
        }
    }

    return nextNode;
}

void GameTileManager::generateMap()
{
    bool bEdge = false;
    bool bIsSpawnTile = false;
    char c;    
    std::ifstream file(m_sLevel);
    
    for (int iY = 0; iY < getMapHeight(); iY++) {

        for (int iX = 0; iX < getMapWidth(); iX++) {
            file >> c;
            if(c != '*')
                m_nodes[iY * m_iMapWidth + iX].bCover = true;
            else
                m_nodes[iY * m_iMapWidth + iX].bCover = false;
        }
    }
    
    file.close();
}
