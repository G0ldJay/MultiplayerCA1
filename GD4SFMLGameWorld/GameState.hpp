//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "PlayerTwo.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	World mWorld;
	Player& mPlayer;
	PlayerTwo& mPlayerTwo;
};