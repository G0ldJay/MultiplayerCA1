//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "State.hpp"
#include "StateStack.hpp"

State::State(StateStack& stack, Context context) : mStack(&stack), mContext(context)
{
}

State::~State()
{
}


void State::requestStackPush(StateID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& font, Player& player, PlayerTwo& playerTwo, MusicPlayer& music, SoundPlayer& sounds) : 
	window(&window), textures(&textures), fonts(&font), player(&player), playerTwo(&playerTwo), music(&music), sounds(&sounds)
{
}
