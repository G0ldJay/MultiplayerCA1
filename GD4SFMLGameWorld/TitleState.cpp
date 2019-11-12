#include "TitleState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context) 
	:State(stack, context), mText(), mShowText(true), mTextEffectTime(sf::Time::Zero)
{
	mTextureSize = context.textures->get(TextureID::TitleScreen).getSize();
	mWindowSize = context.window->getSize();

	float ScaleX = (float)mWindowSize.x / mTextureSize.x;
	float ScaleY = (float)mWindowSize.y / mTextureSize.y;

	mBackgroundSprite.setTexture(context.textures->get(TextureID::TitleScreen));
	mBackgroundSprite.setScale(ScaleX, ScaleY);

	mText.setFont(context.fonts->get(FontID::Main));
	mText.setString("Press any key start");
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

bool TitleState::handleEvent(const sf::Event& event)
{
	//If key pressed, trigger the next state
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}
	return true;
}
