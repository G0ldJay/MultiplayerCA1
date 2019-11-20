#pragma once
#include "State.hpp"
#include"Container.hpp"
#include"Button.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

	void updateOptionText();

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;

	sf::Vector2u mTextureSize;
	sf::Vector2u mWindowSize;
};