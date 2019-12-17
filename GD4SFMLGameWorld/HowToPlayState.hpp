//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once

#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class HowToPlayState : public State //Created by me as a state with instructions on how to play. Accessed from button in menu - Jason Lynch
{
public:
	HowToPlayState(StateStack& stack, Context context); //Constructor - Jason Lynch 

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite; //Hold informational backgorund image - Jason Lynch
	GUI::Container mGUIContainer; //Holds Button for backing - Jason Lynch 
};