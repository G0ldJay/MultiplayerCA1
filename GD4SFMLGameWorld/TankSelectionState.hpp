#pragma once
#include"State.hpp"
#include"Container.hpp"
#include"Button.hpp"
#include"Player.hpp"
#include"Label.hpp"
#include"ActionID.hpp"
#include"Utility.hpp"

#include<SFML/Graphics/Sprite.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/RenderWindow.hpp>

#include<array>

class TankSelectionState :public State {
public:
	TankSelectionState(StateStack& stack, Context context);

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