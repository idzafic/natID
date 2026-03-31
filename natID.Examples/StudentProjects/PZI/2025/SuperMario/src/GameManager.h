#pragma once
#include "td/Types.h"
enum class State { MENU, PLAYING, PAUSED, GAME_OVER };
class GameManager {
protected:
	State GameState;
	td::INT4 HighScore;
	td::INT4 LastScore;
	td::INT4 Lives;
	td::INT4 CurrentScore;
	td::INT4 Coins;
	td::INT4 CurrentLevel;

public:
	void init();//inicijalizacija igre
	void update(float deltaTime);//glavni loop za igricu
	void loadLevel(int levelNumber);//ucitavanje levela
	void reset();
	void startGame();
	void pauseGame();
	void resumeGame();

	void addScore(td::INT4 points = 100);
	void addCoin();// dodaje coin, gleda da li ih je 100
	void addLife(); 
	void loseLife();
	void updateHighScore();

	void setState(State newState);
	void gameOver(); //prelazak u GAME_OVER state
	void victoryCondition(); // kraj levela

	State getState() const { return GameState; }
	td::INT4 getScore() const { return CurrentScore; }
	td::INT4 getCoins() const { return Coins; }
	td::INT4 getLives() const { return Lives; }
	td::INT4 getHighScore() const { return HighScore; }
	td::INT4 getCurrentLevel() const { return CurrentLevel; }
};

