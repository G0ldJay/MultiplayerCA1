//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include <stdexcept>
#include <iostream>
#include "Application.hpp"

int main()
{
	try 
	{
		Application theAmazingGame;
		theAmazingGame.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\n EXCEPTION" << e.what() << std::endl;
	}
}