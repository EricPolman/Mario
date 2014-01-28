#include "Stats.h"
#include "includes.h"

int Stats::LIVES_LEFT = 3;
int Stats::CURRENT_SCORE = 0;
int Stats::HIGH_SCORE = 0;
bool Stats::GAME_OVER = false;

Stats::Stats(void)
{
}


Stats::~Stats(void)
{
}


/*void Stats::LoadHighscore()
{
	std::ifstream highscoreDat("assets/highscore.dat");
	std::string highscoreStr[1];
	int highScore;
	if (highscoreDat.is_open())
	{
		int row = 0;
		while ( std::getline(highscoreDat,highscoreStr[row]) || row < 1 )
		{
			row++;
		}
		highscoreDat.close();
	}
	highScore = atoi(highscoreStr[0].c_str());
	HIGH_SCORE = highScore;
}


void Stats::WriteHighscore()
{
	std::ofstream highscoreDat("assets/highscore.dat");
	highscoreDat << CURRENT_SCORE;
	highscoreDat.clear();
	highscoreDat.close();
}*/

void Stats::AddCoins(int a_amount)
{
	CURRENT_SCORE += a_amount;
}