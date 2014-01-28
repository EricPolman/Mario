#include "GamePlayingState.h"
#include "EntityManager.h"

bool GamePlayingState::instanceFlag = false;

GamePlayingState* GamePlayingState::single = NULL;
GamePlayingState* GamePlayingState::getInstance()
{
    if(!instanceFlag)
    {
        single = new GamePlayingState();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

GamePlayingState::~GamePlayingState()
{
	instanceFlag = false;
}


GamePlayingState::GamePlayingState(void)
{
	Sounds->Add("smb_pipe");
	Sounds->Add("smb_coin");
	Sounds->Add("smb_breakblock");
	Sounds->Add("smb_bump");
	Sounds->Add("smb_powerup_appears");
	Sounds->Add("smb_powerup");
	Sounds->Add("smb_jump");
	Sounds->Add("smb_jump_large");
	Sounds->Add("smb_kick");
	Sounds->Add("smb_mariodie");
	Sounds->Add("smb_vine");
	Sounds->Add("smb_flagpole");
	Sounds->Add("smb2_shrink");
	Sounds->Add("smw_smash");
	Sounds->Add("bgm_star");
}


void GamePlayingState::Enter(Game* a_game)
{
	a_game->m_pWorld = new World();
	a_game->m_pWorld->Init();	
}


void GamePlayingState::Update(Game* a_game, float a_fDeltaTime)
{
	if(a_game->m_pWorld)
	{
		a_game->m_pWorld->Update(a_fDeltaTime);
	}
}


void GamePlayingState::Draw(Game* a_game, Tmpl8::Surface* a_screen)
{
	if(a_game->m_pWorld)
		a_game->m_pWorld->Draw(a_screen);
}


void GamePlayingState::Exit(Game* a_game)
{
	ClearEntities();
	delete a_game->player;
	delete a_game->m_pWorld;
}


void GamePlayingState::ClearEntities()
{
	Entities->ClearAll();
}