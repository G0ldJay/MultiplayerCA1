//Dylan Reilly D00194504 
//Jason Lynch D00137655
#include "TitleState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context) //Modified for controller input - Jason Lynch 
	:State(stack, context), mText(), mShowText(true), mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::TitleScreen));
	mText.setFont(context.fonts->get(FontID::Main));
	mText.setString("Press any key/start button to start");
	centreOrigin(mText);
	mText.setPosition(context.window->getView().getSize() / 2.f);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);

	if (mShowText)
	{
		window.draw(mText);
	}
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}
	return true;
}

//Modified for controller input - Jason Lynch 
bool TitleState::handleEvent(const sf::Event& event)
{
	//If key pressed, trigger the next state
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}

	if (sf::Joystick::isButtonPressed(0, 9)) {
		requestStackPop();
		requestStackPush(StateID::Menu);
	}
	return true;
}
