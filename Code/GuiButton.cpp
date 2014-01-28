#include "GuiButton.h"

GuiButton::GuiButton(float a_fX, float a_fY, std::string a_buttonUnique, void (*a_funcionPointer)())
{
	m_position.x = a_fX;
	m_position.y = a_fY;
	m_pButtonNormal = new Tmpl8::Surface(std::string("assets/menu/button_"+a_buttonUnique+"_normal.png").c_str());
	m_pButtonHover = new Tmpl8::Surface(std::string("assets/menu/button_"+a_buttonUnique+"_hover.png").c_str());
	m_pButtonDown = new Tmpl8::Surface(std::string("assets/menu/button_"+a_buttonUnique+"_down.png").c_str());
	m_state = BUTTON_NORMAL;

	m_funcionPointer = a_funcionPointer;
}


GuiButton::~GuiButton(void)
{
}


void GuiButton::Update(float a_fDeltaTime)
{
	Vector2i mousePos = Input->GetMousePosition();
	//std::cout << mousePos.x << " " << mousePos.y << std::endl;
	if(	mousePos.x > m_position.x && mousePos.x < 
		m_position.x + m_pButtonNormal->GetWidth() &&
		mousePos.y > m_position.y && mousePos.y < 
		m_position.y + m_pButtonNormal->GetHeight())
	{
		if(Input->IsMouseButtonDown(SDL_BUTTON_LEFT))
		{
			m_state = BUTTON_DOWN;
		}
		else if(Input->IsMouseButtonUp(SDL_BUTTON_LEFT))
		{
			m_funcionPointer();
		}
		else
		{
			m_state = BUTTON_HOVER;
		}
	}
	else
	{
		m_state = BUTTON_NORMAL;
	}
}


void GuiButton::Draw(Tmpl8::Surface* a_target)
{
	switch(m_state)
	{
	case BUTTON_NORMAL:
		m_pButtonNormal->CopyTo(a_target, m_position.x, m_position.y, true);
		break;
	case BUTTON_HOVER:
		m_pButtonHover->CopyTo(a_target, m_position.x, m_position.y, true);
		break;
	case BUTTON_DOWN:
		m_pButtonDown->CopyTo(a_target, m_position.x, m_position.y, true);
		break;
	}
}