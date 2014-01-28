#include "TextureManager.h"

TextureManager::~TextureManager(void)
{
	instanceFlag = false;
	for(std::map<std::string, Tmpl8::Surface*>::iterator i = m_Textures.begin(); i != m_Textures.end(); ++i)
	{
		m_Textures.erase(i);
	}
}

bool TextureManager::instanceFlag = false;
TextureManager* TextureManager::single = NULL;
TextureManager* TextureManager::getInstance()
{
    if(! instanceFlag)
    {
        single = new TextureManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

void TextureManager::Add(std::string name, std::string path)
{
	Tmpl8::Surface* tex = new Tmpl8::Surface(const_cast<char*>(path.c_str()));
	TextureManager::getInstance()->m_Textures[name] = tex;
}

void TextureManager::LoadTextures()
{
	Add("player", "assets/sprites/player.png");
	Add("blank", "assets/sprites/blank.png");
	Add("mystery_cube", "assets/sprites/mystery_cube.png");
	Add("brick_block", "assets/sprites/brick_block.png");
	Add("mushroom_red", "assets/sprites/mushroom_red.png");
	Add("flagpole", "assets/sprites/flagpole.png");
	Add("flag", "assets/sprites/flag.png");
	Add("pipe_up", "assets/sprites/pipe_up.png");
	Add("pipe_down", "assets/sprites/pipe_down.png");
	Add("pipe_left", "assets/sprites/pipe_left.png");
	Add("coin", "assets/sprites/coin.png");
	Add("coin_effect", "assets/sprites/coin_effect.png");
	Add("goomba", "assets/sprites/goomba.png");
	Add("turtle", "assets/sprites/turtle.png");
	Add("beanstalk_cube", "assets/sprites/beanstalk_cube.png");
	Add("beanstalk_part", "assets/sprites/beanstalk_part.png");
	Add("invisible_wall", "assets/sprites/invisible_wall.png");
	Add("cloud", "assets/sprites/cloud.png");
	Add("star", "assets/sprites/star.png");
}