//Dylan Reilly D00194504 
//Jason Lynch D00137655
#include "World.hpp"
#include "ParticleID.hpp"
#include "ParticleNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>



World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)//Modified by both of us to fit game needs - Jason Lynch, Dylan Reilly
	: mTarget(outputTarget)
	, mSceneTexture()
	, mCamera(outputTarget.getDefaultView())
	, mFonts(fonts)
	, mSounds(sounds)
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mCamera.getSize().x, mCamera.getSize().y)
	, mSpawnPosition(mCamera.getSize().x * .25f- 30.f, mCamera.getSize().y / 2.f -10.f)
	, mSpawnPositionPlayerTwo(mCamera.getSize().x * .75f +30.f, mCamera.getSize().y / 2.f -10.f)
	, mScrollSpeed(0)
	, mObstacleSpawnPosition(mCamera.getSize().x * .25f, mCamera.getSize().y / 2.f)
	, mPlayerTank(nullptr)
	, mPlayerTwoTank(nullptr)
	, mObstacles()
	, mPickups()
	, mEnemySpawnPoints()
	, mActiveEnemies()
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
	loadTextures();
	buildScene();

	// Prepare the view
	mCamera.setCenter(mCamera.getSize().x/2.f, mCamera.getSize().y/2.f);
}

void World::update(sf::Time dt)
{
	mPlayerTank->setVelocity(0.f, 0.f);
	mPlayerTwoTank->setVelocity(0.f, 0.f);

	// Setup commands to destroy entities
	destroyEntitiesOutsideView();

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();
	adaptPlayerTwoVelocity();

	// Collision detection and response (may destroy entities)
	handleCollisions();

	// Remove all destroyed entities, create new ones
	mSceneGraph.removeWrecks();
	//spawnEnemies();
	spawnObstacles();
	spawnPickups();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
	adaptPlayerTwoPosition();

	updateSounds();
}

void World::draw()
{
	if (PostEffect::isSupported())
	{
		mSceneTexture.clear();
		mSceneTexture.setView(mCamera);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);
	}
	else
	{
		mTarget.setView(mCamera);
		mTarget.draw(mSceneGraph);
	}
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
	return !mPlayerTank->isMarkedForRemoval();
}

bool World::hasAlivePlayerTwo() const
{
	return !mPlayerTwoTank->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerTank->getPosition());
}

void World::updateSounds()
{
	//Set the listener to the player position
	mSounds.setListenPosition(mPlayerTank->getWorldPosition());
	//Remove unused sounds
	mSounds.removeStoppedSounds();

}

void World::loadTextures()
{
	//Both added some new textures - Jason Lynch, Dylan Reilly
	mTextures.load(TextureID::Tanks, "Media/Textures/TankSpriteSheet.png");
	mTextures.load(TextureID::Entities, "Media/Textures/Entities.png");
	mTextures.load(TextureID::Barrel, "Media/Textures/Barell_01.png");
	mTextures.load(TextureID::Wall, "Media/Textures/Arena/Blocks/Block_B_01.png");
	mTextures.load(TextureID::DestructableWall, "Media/Textures/Arena/Blocks/Block_B_01.png");
	mTextures.load(TextureID::Jungle, "Media/Textures/Gamebackground.png");
	mTextures.load(TextureID::Explosion, "Media/Textures/Explosion.png");
	mTextures.load(TextureID::Particle, "Media/Textures/Particle.png");
	mTextures.load(TextureID::FinishLine, "Media/Textures/FinishLine.png");
	mTextures.load(TextureID::LmgBullet, "Media/Textures/Bullet.png");
	mTextures.load(TextureID::HmgBullet, "Media/Textures/HeavyBullet.png");
	mTextures.load(TextureID::GatlingBullet, "Media/Textures/Bullet.png");
	mTextures.load(TextureID::TeslaBullet, "Media/Textures/LightningBallScaled.png");
	mTextures.load(TextureID::HeavyGunPickup, "Media/Textures/Arena/Props/Dot_A.png");
	mTextures.load(TextureID::GatlingGunPickup, "Media/Textures/Arena/Props/Dot_B.png");
	mTextures.load(TextureID::TeslaGunPickup, "Media/Textures/Arena/Props/Artifact.png");
	mTextures.load(TextureID::Nuke, "Media/Textures/NukeBomb.png");
	mTextures.load(TextureID::NukeExplosion, "Media/Textures/Nuke.png");
	mTextures.load(TextureID::Repair, "Media/Textures/Health.png");
	mTextures.load(TextureID::FireRate, "Media/Textures/Speed.png");
}

bool matchesCategories(SceneNode::Pair& colliders, CategoryID type1, CategoryID type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (((static_cast<int>(type1))& category1) && ((static_cast<int>(type2))& category2))
	{
		return true;
	}
	else if (((static_cast<int>(type1))& category2) && ((static_cast<int>(type2))& category1))
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

bool matchesCategories(SceneNode::Pair& colliders, CategoryID type1, ObstacleID type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (((static_cast<int>(type1))& category1) && ((static_cast<int>(type2))& category2))
	{
		return true;
	}
	else if (((static_cast<int>(type1))& category2) && ((static_cast<int>(type2))& category1))
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

//Both modified as collision was needed - Jason Lynch, Dylan Reilly
void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (SceneNode::Pair pair : collisionPairs)
	{
		//Green tank shooting red tank - Dylan Reilly
		if (matchesCategories(pair, CategoryID::AlliedProjectile, CategoryID::PlayerTwoTank))
		{
			auto& projectile = static_cast<Projectile&>(*pair.first);
			auto& enemy = static_cast<Tank&>(*pair.second);
			projectile.destroy();
			enemy.playerLocalSound(mCommandQueue, SoundEffectID::TankHitBullet); //Plays impact sound - Jason Lynch
			enemy.damage(projectile.getDamage());
		}

		//Red tank shooting green tank - Dylan Reilly
		else if (matchesCategories(pair, CategoryID::EnemyProjectile, CategoryID::PlayerTank))
		{
			auto& projectile = static_cast<Projectile&>(*pair.first);
			auto& enemy = static_cast<Tank&>(*pair.second);
			projectile.destroy();
			enemy.playerLocalSound(mCommandQueue, SoundEffectID::TankHitBullet); //Plays impact sound - Jason Lynch
			enemy.damage(projectile.getDamage());
		}

		//Collision for player 1 with Tank pick up - Jason Lynch
		else if (matchesCategories(pair, CategoryID::PlayerTank, CategoryID::Pickup))
		{
			auto& player = static_cast<Tank&>(*pair.first);
			auto& pickup = static_cast<TankPickups&>(*pair.second);

			// Apply pickup effect to player, destroy projectile
			pickup.apply(player);
			player.playerLocalSound(mCommandQueue, SoundEffectID::CollectPickup);
			pickup.destroy();
		}
		//Collision for player 2 with Tank pick up - Jason Lynch
		else if (matchesCategories(pair, CategoryID::PlayerTwoTank, CategoryID::Pickup))
		{
			auto& player = static_cast<Tank&>(*pair.first);
			auto& pickup = static_cast<TankPickups&>(*pair.second);

			// Apply pickup effect to player, destroy projectile
			pickup.apply(player);
			player.playerLocalSound(mCommandQueue, SoundEffectID::CollectPickup);
			pickup.destroy();
		}

		//Collision for player 1 with anything registered as obstacle
		else if (matchesCategories(pair, CategoryID::PlayerTank, CategoryID::Collidable)) {
			auto& player = static_cast<Tank&>(*pair.first);
			auto& obstacle = static_cast<ObstacleTest&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(obstacle.getDamage());
			player.playerLocalSound(mCommandQueue, SoundEffectID::TankHitBullet);

			if (obstacle.getType() == static_cast<int>(ObstacleID::Barrel)) //Destroy obstacle if its just a barrel - Jason Lynch
				obstacle.destroy();
			else if (obstacle.getType() == static_cast<int>(ObstacleID::Nuke))
			{
				obstacle.destroy();
				KillEmAll(player);
			}
			else
				obstacle.damage(1); //Apply 1 damage to it as long as tank is colliding - Jason Lynch 
		}

		//Handle Player 2 hitting collidables - Jason Lynch
		else if (matchesCategories(pair, CategoryID::PlayerTwoTank, CategoryID::Collidable)) {
			auto& player = static_cast<Tank&>(*pair.first);
			auto& obstacle = static_cast<ObstacleTest&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(obstacle.getDamage());
			player.playerLocalSound(mCommandQueue, SoundEffectID::TankHitBullet);

			if (obstacle.getType() == static_cast<int>(ObstacleID::Barrel)) //Destroy obstacle if its just a barrel - Jason Lynch
				obstacle.destroy();
			else if (obstacle.getType() == static_cast<int>(ObstacleID::Nuke))
			{
				obstacle.destroy();
				KillEmAll(player);
			}
			else
				obstacle.damage(1); //Apply 1 damage to it as long as tank is colliding - Jason Lynch 
		}

		else if (matchesCategories(pair, CategoryID::AlliedProjectile, CategoryID::Collidable)) { //Detects collision with barrel obstacle - Jason Lynch
			auto& projectile = static_cast<Projectile&>(*pair.first);
			auto& obstacle = static_cast<ObstacleTest&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			projectile.destroy();
			obstacle.damage(projectile.getDamage());
		}

		else if (matchesCategories(pair, CategoryID::EnemyProjectile, CategoryID::Collidable)) { //Detects collision with barrel obstacle - Jason Lynch
			auto& projectile = static_cast<Projectile&>(*pair.first);
			auto& obstacle = static_cast<ObstacleTest&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			projectile.destroy();
			obstacle.damage(projectile.getDamage());
		}
	}
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(LayerID::LayerCount); ++i)
	{
		CategoryID category = (i == (static_cast<int>(LayerID::LowerAir))) ? CategoryID::SceneAirLayer : CategoryID::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background

	sf::Texture& texture = mTextures.get(TextureID::Jungle);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[static_cast<int>(LayerID::Background)]->attachChild(std::move(backgroundSprite));

	//Add particle nodes for smoke and propellant
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(ParticleID::Smoke, mTextures));
	mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(smokeNode));

	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(ParticleID::Propellant, mTextures));
	mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(propellantNode));

	std::unique_ptr<ParticleNode> bulletSmokeNode(new ParticleNode(ParticleID::BulletSmoke, mTextures));
	mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(bulletSmokeNode));

	std::unique_ptr<ParticleNode> teslaSmokeNode(new ParticleNode(ParticleID::TeslaSmoke, mTextures));
	mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(teslaSmokeNode));

	//Add the sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

	// Add player's Tank
	std::unique_ptr<Tank> player(new Tank(CategoryID::PlayerTank,TankID::GreenLMG1, mTextures, mFonts));
	mPlayerTank = player.get();
	mPlayerTank->setScale(.5f, .5f);
	mPlayerTank->setPosition(mSpawnPosition);
	mSceneLayers[static_cast<int>(LayerID::UpperAir)]->attachChild(std::move(player));

	// Add player two Tank
	std::unique_ptr<Tank> player2(new Tank(CategoryID::PlayerTwoTank,TankID::RedLMG1, mTextures, mFonts)); //Added player two - Jason Lynch
	mPlayerTwoTank = player2.get();
	mPlayerTwoTank->setScale(.5f, .5f); //Scaled down player - Jason Lynch
	mPlayerTwoTank->setPosition(mSpawnPositionPlayerTwo); //Set up player 2 spawn - Jason Lynch
	mSceneLayers[static_cast<int>(LayerID::UpperAir)]->attachChild(std::move(player2));

	//addExtras();
	addObstacles();
	addPickups();
}

//Destroys whoever doesnt grab the nuke - Jason Lynch 
void World::KillEmAll(Tank& player) {
	if (player.getCategory() == static_cast<int>(CategoryID::PlayerTank)) {
		mPlayerTwoTank->destroy();
	}
	else 
	{
		mPlayerTank->destroy();
	}
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerTank->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerTank->setPosition(position);
}

void World::adaptPlayerTwoPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerTwoTank->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerTwoTank->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerTank->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerTank->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	mPlayerTank->accelerate(0.f, mScrollSpeed);
}

void World::adaptPlayerTwoVelocity()
{
	sf::Vector2f velocity = mPlayerTwoTank->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerTwoTank->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	mPlayerTwoTank->accelerate(0.f, mScrollSpeed);
}

void World::addObstacles() //Set up obstacles - Jason Lynch
{
	playerOneBase();
	playerTwoBase();
	teslaobstacles();
	NukeObstacles();
	borderObstacles();
}

//Popultaes world with obstacles - Jason Lynch 
void World::NukeObstacles() {
	//DA BOMB
	addObstacle(ObstacleID::Nuke, 500, 290, 0.f, 0.05f, 0.05f, TextureID::NukeExplosion, sf::Vector2i(323, 182), 9, 2, sf::Vector2f(100.f, 100.f));

	addObstacle(ObstacleID::Barrel, mObstacleSpawnPosition.x + 100, mObstacleSpawnPosition.y + 100, 0.f, 0.25f, 0.25f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Barrel, mObstacleSpawnPosition.x + 250, mObstacleSpawnPosition.y + 100, 0.f, 0.25f, 0.25f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 170, mObstacleSpawnPosition.y + 100, 90.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 170, mObstacleSpawnPosition.y, 90.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 170, mObstacleSpawnPosition.y - 100, 90.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 340, mObstacleSpawnPosition.y + 100, 90.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 340, mObstacleSpawnPosition.y, 90.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 340, mObstacleSpawnPosition.y - 100, 90.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 205, mObstacleSpawnPosition.y - 176, 0.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 305, mObstacleSpawnPosition.y - 176, 0.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 260, mObstacleSpawnPosition.y, 0.0f, .4f, .4f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
}

//Popultaes world with obstacles - Jason Lynch 
void World::playerOneBase() {
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 50, mObstacleSpawnPosition.y + 100, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 50, mObstacleSpawnPosition.y, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 50, mObstacleSpawnPosition.y - 100, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 20, mObstacleSpawnPosition.y + 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + -80, mObstacleSpawnPosition.y + 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 20, mObstacleSpawnPosition.y - 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + -70, mObstacleSpawnPosition.y - 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
}

//Popultaes world with obstacles - Jason Lynch 
void World::playerTwoBase() {
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 460, mObstacleSpawnPosition.y + 100, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 460, mObstacleSpawnPosition.y, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 460, mObstacleSpawnPosition.y - 100, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 500, mObstacleSpawnPosition.y + 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 580, mObstacleSpawnPosition.y + 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 500, mObstacleSpawnPosition.y - 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, mObstacleSpawnPosition.x + 580, mObstacleSpawnPosition.y - 160, 0, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
}

//Popultaes world with obstacles - Jason Lynch 
void World::teslaobstacles() {
	addObstacle(ObstacleID::Wall, 460, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 510, 680, 0.f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 560, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
}

//Popultaes world with obstacles - Jason Lynch 
void World::borderObstacles() {
	addObstacle(ObstacleID::Wall, 100, 10, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, 300, 10, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 420, 10, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, 600, 10, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 720, 10, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, 100, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, 250, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 370, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, 930, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 930, 10, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));

	addObstacle(ObstacleID::Wall, 780, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));
	addObstacle(ObstacleID::Wall, 660, 740, 90.0f, .4f, .2f, TextureID::Explosion, sf::Vector2i(256, 256), 16, 1, sf::Vector2f(1.f, 1.f));


}

//Sets up obstacles - Jason Lynch 
void World::addObstacle(ObstacleID type, float posX, float posY, float rotation, float scaleX, float scaleY, TextureID deathAnimation, sf::Vector2i frameSize, int numberOfFrames, int seconds, sf::Vector2f scale) //Add obstacles to Vector of ObstacleSpawnPoint structs - Jason Lynch
{
	ObstacleSpawnPoint spawn(type, posX, posY, rotation, scaleX, scaleY, deathAnimation, frameSize, numberOfFrames, seconds, scale);
	mObstacles.push_back(spawn);
}

//Spawn obstacles, set scale, rotation, and position - Jason Lynch
void World::spawnObstacles() 
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mObstacles.empty())
	{
		ObstacleSpawnPoint spawn = mObstacles.back();

		std::unique_ptr<ObstacleTest> obstacle(new ObstacleTest(spawn.type, mTextures, mFonts, spawn.deathAnimation, spawn.frameSize, spawn.numberOfFrames,spawn.seconds, spawn.scale));
		obstacle->setScale(spawn.scaleX, spawn.scaleY);
		obstacle->setPosition(spawn.x, spawn.y);
		obstacle->setRotation(spawn.rotation);
		//obstacle->setRotation(180.f);

		if(obstacle->getType() == static_cast<int>(ObstacleID::Nuke))
			mSceneLayers[static_cast<int>(LayerID::UpperAir)]->attachChild(std::move(obstacle));
		else
			mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(obstacle));

		// Enemy is spawned, remove from the list to spawn
		mObstacles.pop_back();
	}
}

//Populates world with pickups - Jason Lynch
void World::addPickups() 
{
	addPickup(TankPickupID::HeavyGun, 40, 30, 0.f, .3f, .3f);
	addPickup(TankPickupID::GatlingGun, 40, 740, 0.f, .3f, .3f);
	addPickup(TankPickupID::HeavyGun, 980, 740, 0.f, .3f, .3f);
	addPickup(TankPickupID::HeavyGun, 980, 30, 0.f, .3f, .3f);
	addPickup(TankPickupID::TeslaGun, 512, 740, 0.f, .3f, .3f);
	//addPickup(TankPickupID::Nuke, 512, 300, 90.f, .05f, .05f);
	addPickup(TankPickupID::Repair, 720, 740, 0.f, .2f, .2f);
	addPickup(TankPickupID::Repair, 360, 30, 0.f, .2f, .2f);
	addPickup(TankPickupID::FireRate, 310, 740, 0.f, .2f, .2f);
	addPickup(TankPickupID::FireRate, 660, 30, 0.f, .2f, .2f);
}

//Sets up Pickups, set scale, rotation, and position - Jason Lynch
void World::addPickup(TankPickupID type, float posX, float posY, float rotation, float scaleX, float scaleY)//Add Tank Pickups to Vector of PickupSpawnPoint structs - Jason Lynch
{
	PickupSpawnPoint spawn(type, posX, posY, rotation, scaleX, scaleY);
	mPickups.push_back(spawn);
}

//Spawn pickups, set scale, rotation, and position - Jason Lynch
void World::spawnPickups()//Spawn Tank pickups, set scale, rotation, and position - Jason Lynch
{
	// Spawn all pickups - Jason Lynch
	while (!mPickups.empty())
	{
		PickupSpawnPoint spawn = mPickups.back();

		std::unique_ptr<TankPickups> pickup(new TankPickups(spawn.type, mTextures));
		pickup->setScale(spawn.scaleX, spawn.scaleY);
		pickup->setRotation(spawn.rotation);
		pickup->setPosition(spawn.x, spawn.y);

		mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(pickup));

		// Enemy is spawned, remove from the list to spawn
		mPickups.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<int>(CategoryID::Projectile) | static_cast<int>(CategoryID::PlayerTwoTank);
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.destroy();
	});

	mCommandQueue.push(command);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mCamera.getCenter() - mCamera.getSize() / 2.f, mCamera.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}