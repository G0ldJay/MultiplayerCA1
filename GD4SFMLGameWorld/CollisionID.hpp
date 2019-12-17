//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
enum class CollisionID //Created enum for environmental objects - Jason Lynch 
{
	None = 0,
	Barrel = 1 << 0,
	Wall = 1 <<1,
	DestructableWall = 1<<2
};