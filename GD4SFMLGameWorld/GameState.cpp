//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "GameState.hpp"

GameState::GameState(StateStack& stack, Context context)
	:State(stack, context)
	, mWorld(*context.window, *context.fonts, *context.sounds)
	, mPlayer(*context.player)
	, mPlayerTwo(*context.playerTwo)
{
	mPlayer.setMissionStatus(MissionStatusID::MissionRunning);
	mPlayerTwo.setMissionStatus(MissionStatusID::MissionRunning);
	context.music->play(MusicID::MissionTheme);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setMissionStatus(MissionStatusID::MissionFailure);
		requestStackPush(StateID::GameOver);
	}
	else if (!mWorld.hasAlivePlayerTwo())
	{
		mPlayerTwo.setMissionStatus(MissionStatusID::MissionFailure);
		requestStackPush(StateID::GameOver);
	}

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);
	mPlayerTwo.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);
	mPlayerTwo.handleEvent(event, commands);

	//Pause if esc is pressed
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape || event.joystickButton.button == 9)
	{
		requestStackPush(StateID::Pause);
	}
	return true;
}
