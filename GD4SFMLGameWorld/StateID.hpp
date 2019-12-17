//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
enum class StateID
{
	None,
	Title,
	Menu,
	TankSelection, //Added by me for future tank selection - Jason Lynch (unused)
	Game, 
	Pause,
	HowToPlay,//Added by me for how to play state - Jason Lynch 
	Settings,
	PLayerOneControllerSettings,//Added by me for player one controller setup - Jason Lynch 
	PlayerTwoControllerSettings,//Added by me for player two controller setup - Jason Lynch 
	GameOver
};