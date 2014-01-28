#include "UserInterface.h"
#include "TextureManager.h"
#include "World.h"
#include "Stats.h"

Player* UserInterface::m_pPlayer;
std::queue<UIText> UserInterface::m_uiTexts;
Tmpl8::Font* UserInterface::g_fontLarge;
Tmpl8::Font* UserInterface::g_fontSmall;

UserInterface::UserInterface(void)
{
	Player* m_pPlayer = 0;
	g_fontLarge = new Tmpl8::Font("assets/sprites/font_large.png", "abcdefghijklmnopqrstuvwxyz0123456789-x");
	g_fontSmall = new Tmpl8::Font("assets/sprites/font_small.png", "abcdefghijklmnopqrstuvwxyz0123456789-x");
}


UserInterface::~UserInterface(void)
{
}


void UserInterface::Update(float a_fDeltaTime)
{

}

std::ostringstream sstreamStats;
void UserInterface::Draw(Tmpl8::Surface* a_screen)
{
	while(m_uiTexts.size() > 0)
	{
		UIText uiText = m_uiTexts.front();
		a_screen->Print(uiText.str, uiText.x, uiText.y, 0xFFFFFFFF);
		m_uiTexts.pop();
	}
	
	g_fontLarge->Print(a_screen, "mario", 10, 10, 0xFFFFFFFF);
	g_fontLarge->Print(a_screen, "coins", 300, 10, 0xFFFFFFFF);
	sstreamStats << Stats::CURRENT_SCORE;
	g_fontLarge->Print(a_screen, const_cast<char*>(sstreamStats.str().c_str()), 326, 26, 0xFFFFFFFF);
	sstreamStats.str("");	
}


void UserInterface::DrawText(const char* a_string, int a_x, int a_y)
{
	UIText uiText;
	uiText.str = a_string;
	uiText.x = a_x;
	uiText.y = a_y;
	m_uiTexts.push(uiText);
}