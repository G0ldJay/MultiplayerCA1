//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "OptionID.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context) //Modified by - Jason Lynch 
	: State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(TextureID::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(100, 250);
	playButton->setText("Play");
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(StateID::Game);
	});

	auto howToPlayButton = std::make_shared<GUI::Button>(context); //Added button to get to how to play state - Jason Lynch
	howToPlayButton->setPosition(100, 300);
	howToPlayButton->setText("How To Play");
	howToPlayButton->setCallback([this]()
	{
			//Player select 
		requestStackPush(StateID::HowToPlay); //Push state
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(100, 350);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
		{
			//Player select 
			requestStackPush(StateID::Settings);
		});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(100, 400);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()
	{
		requestStackClear();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(howToPlayButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);

	context.music->play(MusicID::MenuTheme);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

