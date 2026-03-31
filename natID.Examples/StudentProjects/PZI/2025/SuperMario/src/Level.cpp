#include "Level.h"
#include "gui/Shape.h"
Level::Level(td::INT4 levelNumber, td::INT4 worldNumber, td::INT4 rows, td::INT4 cols, double tileSize)
    : levelNumber(levelNumber)
    , worldNumber(worldNumber)
    , tileRows(rows)
    , tileCols(cols)
    , tileSize(tileSize)
    , levelWidth(cols* tileSize)
    , levelHeight(rows* tileSize)
    , spawnX(100)
    , spawnY(100)
    , goalX(0)
    , goalY(0)
    , isCompleted(false)
{
}

void Level::loadLevel()
{
    // Za sada samo pozovi createWorld1_1()
    if (worldNumber == 1 && levelNumber == 1)
        createWorld1_1();
}

void Level::createWorld1_0()// stari level
{
    //GROUND - 50 tile-ova
    double groundY = levelHeight - tileSize;
    for (int i = 0; i < 50; ++i)
    {
        gui::Rect tileRect(i * tileSize, groundY,
            (i + 1) * tileSize, groundY + tileSize);
        tiles.push_back(new Tile(TileType::GROUND, tileRect));
    }

    //PLATFORME
    double platformY = levelHeight - tileSize * 4;

    // Platforma 1
    for (int i = 3; i < 6; ++i)
    {
        gui::Rect tileRect(i * tileSize, platformY,
            (i + 1) * tileSize, platformY + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Platforma 2
    for (int i = 15; i < 18; ++i)
    {
        gui::Rect tileRect(i * tileSize, platformY,
            (i + 1) * tileSize, platformY + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Question block sa MUSHROOM
    gui::Rect qBlock1(8 * tileSize, platformY,
        9 * tileSize, platformY + tileSize);
    tiles.push_back(new Tile(TileType::QUESTION_BLOCK, qBlock1, ItemInBlock::MUSHROOM));

    // Question block sa COIN
    gui::Rect qBlock2(10 * tileSize, platformY,
        11 * tileSize, platformY + tileSize);
    tiles.push_back(new Tile(TileType::QUESTION_BLOCK, qBlock2, ItemInBlock::COIN));

    // Question block sa FIRE FLOWER
    gui::Rect qBlock3(25 * tileSize, platformY,
        26 * tileSize, platformY + tileSize);
    tiles.push_back(new Tile(TileType::QUESTION_BLOCK, qBlock3, ItemInBlock::FIRE_FLOWER));


    // Coin trail iznad platforme 1
    for (int i = 3; i < 6; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = platformY - tileSize * 2;
        items.push_back(new Coin(coinX, coinY));
    }

    // Coin trail na tlu
    for (int i = 10; i < 15; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = groundY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }

    // Coins dalje desno
    for (int i = 20; i < 25; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = groundY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }

    // Goomba 1
    enemies.push_back(new Goomba(4 * tileSize, levelHeight - tileSize * 5));

    // Goomba 2
    enemies.push_back(new Goomba(20 * tileSize, levelHeight - tileSize - 50));

    spawnX = 80;
    spawnY = levelHeight - tileSize - 100;

    goalX = 48 * tileSize;  // Skoro na kraju levela
    goalY = groundY;
}




void Level::createWorld1_1()//level
{
    //tlo - 120 tile-ova
    double groundY = levelHeight - tileSize;
    for (int i = 0; i < 120; ++i)
    {
        gui::Rect tileRect(i * tileSize, groundY,
            (i + 1) * tileSize, groundY + tileSize);
        tiles.push_back(new Tile(TileType::GROUND, tileRect));
    }

    //0-25 pocetak

    // platforma
    double lowPlatformY = groundY - 3 * tileSize;
    for (int i = 10; i < 13; ++i)
    {
        gui::Rect tileRect(i * tileSize, lowPlatformY,
            (i + 1) * tileSize, lowPlatformY + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Question block sa Mushroom (na istoj visini kao platforma)
    gui::Rect qBlock1(15 * tileSize, lowPlatformY,
        16 * tileSize, lowPlatformY + tileSize);
    tiles.push_back(new Tile(TileType::QUESTION_BLOCK, qBlock1, ItemInBlock::MUSHROOM));

    // Coins iznad platforme (dostupni sa skoka)
    for (int i = 10; i < 13; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = lowPlatformY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }

    // Goomba na tlu
    enemies.push_back(new Goomba(8 * tileSize, groundY - tileSize - 10));

    //visoka platforma
    for (int i = 18; i < 25; ++i)
    {
        gui::Rect tileRect(i * tileSize, lowPlatformY - 3 * tileSize, (i + 1) * tileSize, lowPlatformY - 2 * tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }
    // Coins iznad platforme (dostupni sa skoka)
    for (int i = 18; i < 25; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = lowPlatformY - 4 * tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }


    // 25-50 pipovi

    // Mali pipe (2 tile-a visine)
    for (int row = 0; row < 2; ++row)
    {
        double pipeY = groundY - (row + 1) * tileSize;

        gui::Rect pipeLeft(30 * tileSize, pipeY,
            31 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_LEFT, pipeLeft));

        gui::Rect pipeRight(31 * tileSize, pipeY,
            32 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_RIGHT, pipeRight));
    }

    // Srednji pipe (3 tile-a visine)
    for (int row = 0; row < 3; ++row)
    {
        double pipeY = groundY - (row + 1) * tileSize;

        gui::Rect pipeLeft(38 * tileSize, pipeY,
            39 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_LEFT, pipeLeft));

        gui::Rect pipeRight(39 * tileSize, pipeY,
            40 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_RIGHT, pipeRight));
    }
    // Veliki pipe (4 tile-a visine)
    for (int row = 0; row < 4; ++row)
    {
        double pipeY = groundY - (row + 1) * tileSize;

        gui::Rect pipeLeft(46 * tileSize, pipeY,
            47 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_LEFT, pipeLeft));

        gui::Rect pipeRight(47 * tileSize, pipeY,
            48 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_RIGHT, pipeRight));
    }

    // Coins između pipe-ova (na tlu)
    for (int i = 33; i < 37; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = groundY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }
    for (int i = 41; i < 45; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = groundY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }

    // Goombe izmedju
    enemies.push_back(new Goomba(36 * tileSize, groundY - tileSize - 10));
    enemies.push_back(new Goomba(43 * tileSize, groundY - tileSize - 10));

    // 50-75 platforme

    // Platform 1 (nisko - Mario može skočiti sa tla)
    double float1Y = groundY - 4 * tileSize;
    for (int i = 52; i < 55; ++i)
    {
        gui::Rect tileRect(i * tileSize, float1Y,
            (i + 1) * tileSize, float1Y + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Platform 2 (malo viša - Mario može skočiti sa Platform 1)
    double float2Y = groundY - 7 * tileSize;
    for (int i = 58; i < 61; ++i)
    {
        gui::Rect tileRect(i * tileSize, float2Y,
            (i + 1) * tileSize, float2Y + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Question block sa Fire Flower (iznad Platform 2)
    gui::Rect qBlock2(59 * tileSize, float2Y + tileSize,
        60 * tileSize, float2Y + 2 * tileSize);
    tiles.push_back(new Tile(TileType::QUESTION_BLOCK, qBlock2, ItemInBlock::FIRE_FLOWER));

    // Platform 3 (vraća se dole)
    double float3Y = groundY - 4 * tileSize;
    for (int i = 64; i < 67; ++i)
    {
        gui::Rect tileRect(i * tileSize, float3Y,
            (i + 1) * tileSize, float3Y + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Srednji pipe (3 tile-a visine)
    for (int row = 0; row < 3; ++row)
    {
        double pipeY = groundY - (row + 1) * tileSize;

        gui::Rect pipeLeft(71 * tileSize, pipeY,
            72 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_LEFT, pipeLeft));

        gui::Rect pipeRight(72 * tileSize, pipeY,
            73 * tileSize, pipeY + tileSize);
        tiles.push_back(new Tile(TileType::PIPE_BODY_RIGHT, pipeRight));
    }

    // Coins trail (dostupni sa platformi)
    for (int i = 55; i < 58; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = float1Y + tileSize;  // Između Platform 1 i 2
        items.push_back(new Coin(coinX, coinY));
    }

    for (int i = 61; i < 64; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = float2Y + tileSize;  // Između Platform 2 i 3
        items.push_back(new Coin(coinX, coinY));
    }

    // Goomba na Platform 1
    enemies.push_back(new Goomba(53 * tileSize, float1Y - tileSize - 10));


    //75-90 stepenice

    // Stepenice (4 stepena, svaki 1 tile viši)
    for (int step = 0; step < 4; ++step)
    {
        for (int row = 0; row <= step; ++row)
        {
            double stepX = (80 + step) * tileSize;
            double stepY = groundY - (row + 1) * tileSize;

            gui::Rect stepRect(stepX, stepY,
                stepX + tileSize, stepY + tileSize);
            tiles.push_back(new Tile(TileType::BRICK, stepRect));
        }
    }

    // Coins na vrhu stepenica
    for (int i = 80; i < 84; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = groundY - 5 * tileSize;
        items.push_back(new Coin(coinX, coinY));
    }

    // Goomba na stepenicama
    enemies.push_back(new Goomba(82 * tileSize, groundY - 3 * tileSize - 10));


    // 90-120 kraj

    // Ravna površina prije finish-a

    // Final platform (visoka, ali Mario može skočiti sa stepenica)
    double finalPlatY = groundY - 5 * tileSize;
    for (int i = 90; i < 100; ++i)
    {
        gui::Rect tileRect(i * tileSize, finalPlatY,
            (i + 1) * tileSize, finalPlatY + tileSize);
        tiles.push_back(new Tile(TileType::BRICK, tileRect));
    }

    // Bonus coins iznad final platform
    for (int i = 90; i < 100; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = finalPlatY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }

    // Final Goomba
    enemies.push_back(new Goomba(97 * tileSize, finalPlatY - tileSize - 10));

    // Coins na tlu prije flagpole-a
    for (int i = 105; i < 110; ++i)
    {
        double coinX = i * tileSize + tileSize / 2.0 - 15;
        double coinY = groundY - tileSize - 20;
        items.push_back(new Coin(coinX, coinY));
    }


    //POSTAVKE

    spawnX = 100;
    spawnY = groundY - tileSize - 10;

    goalX = 115 * tileSize;  // Flagpole je na 115. tile-u
    goalY = groundY;

    levelWidth = 120 * tileSize;
}

void Level::update(double deltaTime)
{
    // Update enemies
    for (auto enemy : enemies)
    {
        if (enemy && enemy->isAlive())
            enemy->update(deltaTime);
    }

    // Update items (coins, power-ups)
    for (auto item : items)
    {
        if (item && item->isAlive())
            item->update(deltaTime);
    }

    // Update projectiles (later - fireballs)
    for (auto proj : projectiles)
    {
        if (proj && proj->isAlive())
            proj->update(deltaTime);
    }
}

void Level::render(Camera* camera, const gui::Rect& viewPort)
{
    gui::Point cameraOffset = camera ? camera->getOffset() : gui::Point(0, 0);

    // Draw tiles
    for (auto tile : tiles)
    {
        if (tile)
            tile->draw(cameraOffset);
    }

    // Draw items
    for (auto item : items)
    {
        if (item && item->isAlive())
            item->draw(cameraOffset, viewPort);
    }

    // Draw enemies
    for (auto enemy : enemies)
    {
        if (enemy && enemy->isAlive())
            enemy->draw(cameraOffset, viewPort);
    }
}

void Level::addEnemy(GameObject* enemy)
{
    enemies.push_back(enemy);
}

void Level::addItem(GameObject* item)
{
    items.push_back(item);
}

void Level::addProjectile(GameObject* projectile)
{
    projectiles.push_back(projectile);
}

void Level::removeDeadObjects()
{
    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](GameObject* obj) { return !obj->isAlive(); }),
        enemies.end()
    );

    // Remove collected items
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](GameObject* obj) { return !obj->isAlive(); }),
        items.end()
    );
}

void Level::clear()
{
    // Clear tiles
    for (auto tile : tiles)
        delete tile;
    tiles.clear();

    // Clear enemies
    for (auto enemy : enemies)
        delete enemy;
    enemies.clear();

    // Clear items
    for (auto item : items)
        delete item;
    items.clear();

    // Clear projectiles
    for (auto proj : projectiles)
        delete proj;
    projectiles.clear();
}

bool Level::checkVictoryCondition(GameObject* mario) const
{
    if (!mario)
        return false;

    // Provjeri da li je Mario dostigao goal
    gui::Point marioPos = mario->getPos();
    return (marioPos.x >= goalX);
}

void Level::setCompleted(bool completed)
{
    isCompleted = completed;
}

bool Level::getIsCompleted() const
{
    return isCompleted;
}

Level::~Level()
{
    clear();
}
