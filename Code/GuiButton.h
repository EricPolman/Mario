#pragma once
#include "includes.h"

enum ButtonState{BUTTON_NORMAL,BUTTON_HOVER,BUTTON_DOWN};

class GuiButton
{
public:
	GuiButton(float a_fX, float a_fY, std::string a_buttonUnique, void (*a_funcionPointer)());
	~GuiButton(void);

	Tmpl8::Surface* m_pButtonNormal;
	Tmpl8::Surface* m_pButtonHover;
	Tmpl8::Surface* m_pButtonDown;
	ButtonState m_state;
	Vector2 m_position;

	void (*m_funcionPointer)();

	void Update(float a_fDeltaTime);
	void Draw(Tmpl8::Surface* a_target);
};

