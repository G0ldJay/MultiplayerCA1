//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "CommandQueue.hpp"

void CommandQueue::push(const Command& command)
{
	mQueue.push(command);
}

Command CommandQueue::pop()
{
	Command command = mQueue.front();
	mQueue.pop();
	return Command(command);
}

bool CommandQueue::isEmpty() const
{
	return mQueue.empty();
}
