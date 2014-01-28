#pragma once
class Stats
{
public:
	Stats(void);
	~Stats(void);
	
	static int LIVES_LEFT;
	static int ROUND;
	static int CURRENT_SCORE;
	static int HIGH_SCORE;
	static int ENEMIES_LEFT;
	static bool GAME_OVER;

	static void AddCoins(int a_amount);
	//static void LoadHighscore();
	//static void WriteHighscore();
};

