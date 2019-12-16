#include "World.hpp"
#include "ParticleID.hpp"
#include "ParticleNode.hpp"
#include "Obstacle.hpp"

#include <SFML/Graphics/RenderWindow.hpp>



World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(outputTarget)
	, mSceneTexture()
	, mCamera(outputTarget.getDefaultView())
	, mFonts(fonts)
	, mSounds(sounds)
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mCamera.getSize().x, mCamera.getSize().y)
	, mSpawnPosition(mCamera.getSize().x * .25f, mCamera.getSize().y / 2.f)
	, mSpawnPositionPlayerTwo(mCamera.getSize().x * .75f, mCamera.getSize().y / 2.f)
	, mScrollSpeed(0)
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

	// Setup commands to destroy entities, and guide missiles
	destroyEntitiesOutsideView();
	guideMissiles();

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
	mTextures.load(TextureID::Tanks, "Media/Textures/TankSpriteSheet.png");
	mTextures.load(TextureID::Entities, "Media/Textures/Entities.png");
	mTextures.load(TextureID::Barrel, "Media/Textures/Arena/Props/Barell_01.png");
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

		else if (matchesCategories(pair, CategoryID::PlayerTank, ObstacleID::Barrel)) {
			auto& player = static_cast<Tank&>(*pair.first);
			auto& obstacle = static_cast<Obstacle&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(obstacle.getDamage());
			obstacle.destroy();
		}

		else if (matchesCategories(pair, CategoryID::AlliedProjectile, ObstacleID::Barrel)) { //Detects collision with barrel obstacle - Jason Lynch
			auto& projectile = static_cast<Projectile&>(*pair.first);
			auto& obstacle = static_cast<Obstacle&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			projectile.destroy();
			obstacle.damage(projectile.getDamage());
		}

		else if (matchesCategories(pair, CategoryID::PlayerTank, ObstacleID::Wall)) { //BROKEN : DOESNT PICK UP WALL - Jason Lynch
			auto& player = static_cast<Tank&>(*pair.first);
			auto& obstacle = static_cast<Obstacle&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(obstacle.getDamage());
			obstacle.damage(5);
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
	addObstacle(ObstacleID::Barrel, mSpawnPosition.x+100, mSpawnPosition.y+100, 0.f, 0.25f, 0.25f);

	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 190, mSpawnPosition.y + 100, 90.0f, .4f,.4f);
	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 190, mSpawnPosition.y , 90.0f, .4f, .4f);
	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 190, mSpawnPosition.y-100, 90.0f, .4f, .4f);

	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 320, mSpawnPosition.y + 100, 90.0f, .4f, .4f);
	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 320, mSpawnPosition.y, 90.0f, .4f, .4f);
	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 320, mSpawnPosition.y - 100, 90.0f, .4f, .4f);

	addObstacle(ObstacleID::Wall, mSpawnPosition.x + 260, mSpawnPosition.y-170, 0.0f, .4f, .4f);
}

void World::addObstacle(ObstacleID type, float posX, float posY, float rotation, float scaleX, float scaleY) //Add obstacles to Vector of ObstacleSpawnPoint structs - Jason Lynch
{
	ObstacleSpawnPoint spawn(type, posX, posY, rotation, scaleX, scaleY);
	mObstacles.push_back(spawn);
}

void World::spawnObstacles() //Spawn obstacles, set scale, rotation, and position - Jason Lynch
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mObstacles.empty())
	{
		ObstacleSpawnPoint spawn = mObstacles.back();

		std::unique_ptr<Obstacle> obstacle(new Obstacle(spawn.type, mTextures, mFonts));
		obstacle->setScale(spawn.scaleX, spawn.scaleY);
		obstacle->getCategory();
		obstacle->setPosition(spawn.x, spawn.y);
		obstacle->setRotation(spawn.rotation);
		//obstacle->setRotation(180.f);

		mSceneLayers[static_cast<int>(LayerID::LowerAir)]->attachChild(std::move(obstacle));

		// Enemy is spawned, remove from the list to spawn
		mObstacles.pop_back();
	}
}

void World::addPickups() //Set up pickups - Jason Lynch
{
	addPickup(TankPickupID::HeavyGun, 100, 100 );
	addPickup(TankPickupID::GatlingGun, 912, 650);
	addPickup(TankPickupID::TeslaGun, 512, 380);
}

void World::addPickup(TankPickupID type, float posX, float posY)//Add Tank Pickups to Vector of PickupSpawnPoint structs - Jason Lynch
{
	PickupSpawnPoint spawn(type, posX, posY);
	mPickups.push_back(spawn);
}

void World::spawnPickups()//Spawn Tank pickups, set scale, rotation, and position - Jason Lynch
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mPickups.empty())
	{
		PickupSpawnPoint spawn = mPickups.back();

		std::unique_ptr<TankPickups> pickup(new TankPickups(spawn.type, mTextures));
		pickup->setScale(0.3f, 0.3f);
		pickup->setPosition(spawn.x, spawn.y);
		//obstacle->setRotation(180.f);

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

void World::guideMissiles()
{
	// Setup command that stores all enemies in mActiveEnemies
	Command enemyCollector;
	enemyCollector.category = static_cast<int>(CategoryID::PlayerTwoTank);
	enemyCollector.action = derivedAction<Tank>([this](Tank& enemy, sf::Time)
	{
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	// Setup command that guides all missiles to the enemy which is currently closest to the player
	Command missileGuider;
	missileGuider.category = static_cast<int>(CategoryID::AlliedProjectile);
	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time)
	{
		// Ignore unguided bullets
		if (!missile.isGuided())
			return;

		float minDistance = std::numeric_limits<float>::max();
		Tank* closestEnemy = nullptr;

		// Find closest enemy
		for (Tank* enemy : mActiveEnemies)
		{
			float enemyDistance = distance(missile, *enemy);

			if (enemyDistance < minDistance)
			{
				closestEnemy = enemy;
				minDistance = enemyDistance;
			}
		}

		if (closestEnemy)
			missile.guideTowards(closestEnemy->getWorldPosition());
	});

	// Push commands, reset active enemies
	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemies.clear();
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