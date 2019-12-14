#pragma once
enum class CollisionID
{
	None = 0,
	Barrel = 1 << 0,
	Wall = 1 <<1,
	DestructableWall = 1<<2
};