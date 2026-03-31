#pragma once
#include "GameObject.h"
#include "Tile.h"
#include "Coin.h"      
#include "Goomba.h"    
#include "PowerUp.h"   
#include <vector>
#include <td/Types.h>
#include "Camera.h"

class Level {
protected:
    td::INT4 levelNumber;
    td::INT4 worldNumber;
    double levelWidth, levelHeight;
    td::INT4 tileRows, tileCols;
    double tileSize;

    std::vector<std::vector<Tile*>> tileMap;  
    std::vector<Tile*> tiles;                 
    std::vector<GameObject*> enemies;         
    std::vector<GameObject*> items;           
    std::vector<GameObject*> projectiles;     

    double spawnX, spawnY;
    double goalX, goalY;
    bool isCompleted;

public:
    Level(td::INT4 levelNumber, td::INT4 worldNumber, td::INT4 rows, td::INT4 cols, double tileSize);

    void loadLevel();
    void createWorld1_0();
    void createWorld1_1();
    void clear();

    void update(double deltaTime);
    void render(Camera* camera, const gui::Rect& viewPort);

    void addEnemy(GameObject* enemy);
    void addItem(GameObject* item);
    void addProjectile(GameObject* projectile);
    void removeDeadObjects();

    std::vector<Tile*> getTiles() const { return tiles; }
    std::vector<GameObject*> getEnemies() const { return enemies; }
    std::vector<GameObject*> getItems() const { return items; }

    bool checkVictoryCondition(GameObject* mario) const;
    void setCompleted(bool completed);
    bool getIsCompleted() const;

    double getLevelWidth() const { return levelWidth; }
    double getLevelHeight() const { return levelHeight; }
    double getTileSize() const { return tileSize; }
    double getSpawnX() const { return spawnX; }
    double getSpawnY() const { return spawnY; }
    td::INT4 getLevelNumber() const { return levelNumber; }
    td::INT4 getWorldNumber() const { return worldNumber; }

    ~Level();
};
