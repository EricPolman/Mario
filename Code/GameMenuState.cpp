#include "GameMenuState.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "GamePlayingState.h"

bool GameMenuState::instanceFlag = false;

GameMenuState* GameMenuState::single = NULL;
GameMenuState* GameMenuState::getInstance()
{
    if(!instanceFlag)
    {
        single = new GameMenuState();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

GameMenuState::~GameMenuState()
{
	instanceFlag = false;
	delete m_background;
}


GameMenuState::GameMenuState(void)
{	
	Textures->Add("background", "assets/menu/background.png");
	Textures->Add("gameTitle", "assets/menu/gameTitle.png");
	
	m_fColorTransitionTimer = 0;
	m_background = new Tmpl8::Surface(SCRWIDTH,SCRHEIGHT);
	m_gameTitle = Textures->m_Textures["gameTitle"];
	m_button = new GuiButton(SCRWIDTH / 2 - 116, SCRHEIGHT - 100, "playgame", &GameMenuState::StartGame);
	m_quitButton = new GuiButton(SCRWIDTH / 2 - 116, SCRHEIGHT - 50, "quit", &GameMenuState::QuitGame);
}

void GameMenuState::Enter(Game* a_game)
{
	m_game = a_game;
}


void GameMenuState::Update(Game* a_game, float a_fDeltaTime)
{
	m_fColorTransitionTimer += a_fDeltaTime / 8;
	if(m_fColorTransitionTimer > 1)
		m_fColorTransitionTimer -= 1;

	Pixel* sBuffer = m_background->GetBuffer();
	Pixel* nBuffer = Textures->m_Textures["background"]->GetBuffer();
	for(int y = 0; y < SCRHEIGHT; y+=2)
		{
		for(int x = 0; x < SCRWIDTH; x+=2)
		{
			int i = y * SCRWIDTH + x;

			RgbColor rgb;
			rgb.r = (nBuffer[i] & 0xFF0000) >> 16;
			rgb.g = (nBuffer[i] & 0xFF00) >> 8;
			rgb.b = nBuffer[i] & 0xFF;
			HsvColor hsv = RgbToHsv(rgb);
			hsv.h = 255 * m_fColorTransitionTimer;
			rgb = HsvToRgb(hsv);
			sBuffer[i] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
			sBuffer[i+1] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
			sBuffer[i+SCRWIDTH] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
			sBuffer[i+SCRWIDTH+1] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
		}
	}

	m_button->Update(a_fDeltaTime);
	m_quitButton->Update(a_fDeltaTime);
}

void GameMenuState::Draw(Game* a_game, Tmpl8::Surface* a_screen)
{
	m_background->CopyTo(a_screen, 0, 0);
	m_gameTitle->CopyTo(a_screen, SCRWIDTH / 2 - 150, 20, true);
	m_button->Draw(a_screen);
	m_quitButton->Draw(a_screen);
}


void GameMenuState::StartGame()
{
	GameMenuState::getInstance()->m_game->ChangeState(GamePlayingState::getInstance());
}


void GameMenuState::QuitGame()
{
	Stats::GAME_OVER = true;
}