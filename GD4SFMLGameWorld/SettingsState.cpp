//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "SettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "OptionID.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SettingsState::SettingsState(StateStack& stack, Context context) //Modified by me to be a sub menu for player input selection - Jason Lynch 
	: State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(TextureID::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playerOneControls = std::make_shared<GUI::Button>(context);
	playerOneControls->setPosition(100, 250);
	playerOneControls->setText("Player 1 Controls");
	playerOneControls->setCallback([this]()
		{
			requestStackPush(StateID::PLayerOneControllerSettings);
		});

	auto playerTwoControls = std::make_shared<GUI::Button>(context);
	playerTwoControls->setPosition(100, 300);
	playerTwoControls->setText("Player 2 Controls");
	playerTwoControls->setCallback([this]()
		{
			requestStackPush(StateID::PlayerTwoControllerSettings);
		});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(100, 350);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(playerOneControls);
	mGUIContainer.pack(playerTwoControls);
	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

