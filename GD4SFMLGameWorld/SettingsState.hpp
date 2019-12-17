//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SettingsState : public State //Modified by me to act as a sub menu for selecting which players controls to edit - Jason Lynch 
{
public:
	SettingsState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
};