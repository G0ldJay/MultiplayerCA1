//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "Label.hpp"
#include "ActionID.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <array>

class TankSelectionState : public State //Created by me for tank selection before game start - Jason Lynch (Unused, may use in future)
{
public:
	TankSelectionState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingButtons;
	std::array<GUI::Label::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingLabels;
};