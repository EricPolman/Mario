#pragma once
#include "state.h"
#include "game.h"
#include "GuiButton.h"

using namespace Tmpl8;

class GameMenuState :
	public State<Game>
{
private:
	GameMenuState(void);
	
    static bool instanceFlag;
    static GameMenuState *single;

	float m_fColorTransitionTimer;
public:
	~GameMenuState(void);
    static GameMenuState* getInstance();

	void Enter(Game* a_game);
	void Update(Game* a_game, float a_fDeltaTime);
	void Draw(Game* a_game, Tmpl8::Surface* a_screen);
	void Hit(Game* a_game, Entity* a_pOther){};
	void Exit(Game* a_game){};
	
	static void StartGame();
	static void QuitGame();

	Tmpl8::Surface* m_background;
	Tmpl8::Surface* m_gameTitle;
	GuiButton* m_button;
	GuiButton* m_quitButton;

	Game* m_game;
};

