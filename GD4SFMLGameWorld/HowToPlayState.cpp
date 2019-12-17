//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "HowToPlayState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "OptionID.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

HowToPlayState::HowToPlayState(StateStack& stack, Context context)//Created by me to show people a general idea of how to play. Based off menu state - Jason Lynch 
	: State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(TextureID::HowToPlay); //Image with info on how to play 
	mBackgroundSprite.setTexture(texture);

	//Back button for exiting 
	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(412.f, 680.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&HowToPlayState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void HowToPlayState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool HowToPlayState::update(sf::Time dt)
{
	return true;
}

bool HowToPlayState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

