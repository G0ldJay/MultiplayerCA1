#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "OptionID.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context), mGUIContainer()
{

	mTextureSize = context.textures->get(TextureID::MainMenu).getSize();
	mWindowSize = context.window->getSize();

	float ScaleX = (float)mWindowSize.x / mTextureSize.x;
	float ScaleY = (float)mWindowSize.y / mTextureSize.y;

	mBackgroundSprite.setTexture(context.textures->get(TextureID::MainMenu));
	mBackgroundSprite.setScale(ScaleX, ScaleY);

	sf::Texture& texture = context.textures->get(TextureID::MainMenu);

	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	playButton->setPosition(100, 250);
	playButton->setText("Play");
	playButton->setCallback([this]() 
	{
		requestStackPop();
		requestStackPush(StateID::Game);
	});

	auto settingsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	settingsButton->setPosition(100, 300);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
	{
		requestStackPush(StateID::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	exitButton->setPosition(100, 350);
	exitButton->setText("Exit");
	exitButton->setCallback([this]() 
	{
		requestStackClear();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);
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
