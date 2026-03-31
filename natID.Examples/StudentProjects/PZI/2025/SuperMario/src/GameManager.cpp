#include "GameManager.h"

void GameManager::init()
{
    GameState = State::MENU;
    HighScore = 0;
    LastScore = 0;
    Lives = 3;
    CurrentScore = 0;
    Coins = 0;
    CurrentLevel = 1;
}

void GameManager::startGame()
{
    GameState = State::PLAYING;
    Lives = 3;
    CurrentScore = 0;
    Coins = 0;
}

void GameManager::pauseGame()
{
    if (GameState == State::PLAYING)
        GameState = State::PAUSED;
}

void GameManager::resumeGame()
{
    if (GameState == State::PAUSED)
        GameState = State::PLAYING;
}

void GameManager::addScore(td::INT4 points)
{
    CurrentScore += points;
    updateHighScore();
}

void GameManager::addCoin()
{
    Coins++;

    // 100 coins = novi zivot
    if (Coins >= 100)
    {
        addLife();
        Coins = 0;
    }
}

void GameManager::addLife()
{
    Lives++;
}

void GameManager::loseLife()
{
    Lives--;

    if (Lives <= 0)
        gameOver();
}

void GameManager::updateHighScore()
{
    if (CurrentScore > HighScore)
        HighScore = CurrentScore;
}

void GameManager::gameOver()
{
    GameState = State::GAME_OVER;
    LastScore = CurrentScore;
}

void GameManager::victoryCondition()
{
    addScore(1000);
    // CurrentLevel++;
    // loadLevel(CurrentLevel);
}

void GameManager::setState(State newState)
{
    GameState = newState;
}

void GameManager::reset()
{
    init();
}

void GameManager::update(float deltaTime)
{

}

void GameManager::loadLevel(int levelNumber)
{
    // novi level
    CurrentLevel = levelNumber;
}

