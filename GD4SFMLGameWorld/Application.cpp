//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include <iostream>
#include "Application.hpp"
#include "Utility.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "GameOverState.hpp"
#include "TankSelectionState.hpp"
#include "HowToPlayState.hpp"
#include "PlayerOneInputSettingsState.hpp"
#include "PlayerTwoInputSettings.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(1024, 768), "Game Play", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer()
	, mPlayerTwo()
	, mMusic()
	, mSoundPlayer()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mPlayerTwo, mMusic, mSoundPlayer))
	, mStatisticText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(FontID::Main, "Media/EvilEmpire.otf");//Added font - Jason Lynch 
	mTextures.load(TextureID::TitleScreen, "Media/Textures/Title.png");
	mTextures.load(TextureID::HowToPlay, "Media/Textures/HowToPlay.png");//Added background - Jason Lynch 
	mTextures.load(TextureID::PlayerOneInputSetupBackground, "Media/Textures/PlayerOneControlSetupBackground.png");//Added background - Jason Lynch 
	mTextures.load(TextureID::PlayerTwoInputSetupBackground, "Media/Textures/PlayerTwoControlSetupBackground.png");//Added background - Jason Lynch 
	mTextures.load(TextureID::Buttons, "Media/Textures/Buttons.png");
	mStatisticText.setFont(mFonts.get(FontID::Main));
	mStatisticText.setPosition(5.f, 5.f);
	mStatisticText.setCharacterSize(20);
	registerStates();
	mStateStack.pushState(StateID::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);

			//Check if the statestack is empty
			if (mStateStack.isEmpty())
			{
				mWindow.close();
			}
		}
		updateStatistics(elapsedTime);
		draw();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::draw()
{
	mWindow.clear();
	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticText);
	mWindow.display();
}

//Stopped statistic display
void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticText.setString("Frames/Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time/Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(StateID::Title);
	mStateStack.registerState<MenuState>(StateID::Menu);
	mStateStack.registerState<GameState>(StateID::Game);
	mStateStack.registerState<PauseState>(StateID::Pause);
	mStateStack.registerState<HowToPlayState>(StateID::HowToPlay); //Added state - Jason Lynch 
	mStateStack.registerState<SettingsState>(StateID::Settings);//Modified state - Jason Lynch 
	mStateStack.registerState<PlayerOneInputSettingsState>(StateID::PLayerOneControllerSettings);//Added state - Jason Lynch 
	mStateStack.registerState<PlayerTwoInputSettingsState>(StateID::PlayerTwoControllerSettings);//Added state - Jason Lynch 
	mStateStack.registerState<GameOverState>(StateID::GameOver);
	mStateStack.registerState<TankSelectionState>(StateID::TankSelection);//Added state - Jason Lynch (Unused)
}
