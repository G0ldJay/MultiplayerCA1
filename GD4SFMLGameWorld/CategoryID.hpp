#pragma once

//Entity/SceneNode category, used to dispatch commands
enum class CategoryID
{
	//Updated enums for new tanks/projectiles
	None = 0,
	SceneAirLayer = 1 << 0,
	PlayerTank = 1 << 1,
	Collidable = 1 << 2,
	PlayerTwoTank = 1 << 3,
	Pickup = 1 << 4,
	AlliedProjectile = 1 << 5,
	EnemyProjectile = 1 << 6,
	ParticleSystem = 1 << 7,
	SoundEffect = 1 << 8,

	Tank = PlayerTank | PlayerTwoTank,
	Projectile = AlliedProjectile | EnemyProjectile
};