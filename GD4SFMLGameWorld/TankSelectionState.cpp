#include "TankSelectionState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "OptionID.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

TankSelectionState::TankSelectionState(StateStack& stack, Context context)
	: State(stack, context), mGUIContainer()
{

	mTextureSize = context.textures->get(TextureID::TankSelectionBackground).getSize();
	mWindowSize = context.window->getSize();

	float ScaleX = (float)mWindowSize.x / mTextureSize.x;
	float ScaleY = (float)mWindowSize.y / mTextureSize.y;

	mBackgroundSprite.setTexture(context.textures->get(TextureID::TankSelectionBackground));
	mBackgroundSprite.setScale(ScaleX, ScaleY);

	sf::Texture& texture = context.textures->get(TextureID::TankSelectionBackground);

	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	playButton->setPosition(100, 200);
	playButton->setText("Start");
	playButton->setCallback([this]()
	{
			requestStackPop();
			requestStackPush(StateID::Game);
	});

	auto nextTankButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	nextTankButton->setPosition(100, 250);
	nextTankButton->setText("Next Tank");
	nextTankButton->setCallback([this]()
	{
			//requestStackPop();
			//requestStackPush(StateID::Game);
	});

	auto previousTankButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	previousTankButton->setPosition(100, 300);
	previousTankButton->setText("Previous Tank");
	previousTankButton->setCallback([this]()
	{
			/*requestStackPop();
			requestStackPush(StateID::Game);*/
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	exitButton->setPosition(100, 350);
	exitButton->setText("Main Menu");
	exitButton->setCallback([this]()
	{
			requestStackPop();
			requestStackPush(StateID::Menu);
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(nextTankButton);
	mGUIContainer.pack(previousTankButton);
	mGUIContainer.pack(exitButton);
}

void TankSelectionState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool TankSelectionState::update(sf::Time dt)
{
	return true;
}

bool TankSelectionState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
