#include "TankSelectionState.hpp"

TankSelectionState::TankSelectionState(StateStack& stack, Context context) :State(stack, context), mGUIContainer()
{
	sf::Texture& texture = context.textures->get(TextureID::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(100, 250);
	playButton->setText("Select Tank");
	playButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(StateID::Game);
		});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(100, 300);
	settingsButton->setText("Next Tank");
	settingsButton->setCallback([this]()
		{
			
		});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(100, 350);
	exitButton->setText("Previous Tank");
	exitButton->setCallback([this]()
		{
		
		});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(100, 620.f);
	backButton->setText("Back");
	exitButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(StateID::Menu);
		});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);
	mGUIContainer.pack(backButton);

	//context.music->play(MusicID::MenuTheme);
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
