//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Tank.hpp"
#include "LayerID.hpp"
#include "CommandQueue.hpp"
#include "TankID.hpp"
#include "Pickup.hpp"
#include "TankPickups.hpp"
#include "PostEffect.hpp"
#include "BloomEffect.hpp"
#include "SoundNode.hpp"
#include "SoundPlayer.hpp"
#include "CollisionID.hpp"
#include "ObstacleTest.hpp"

#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/Graphics/Texture.hpp"

#include <array>


//Forward declaration
namespace sf
{
	class RenderTarget;
}


class World : private sf::NonCopyable //Modified by both of us - Dylan Reilly, Jason Lynch
{
public:
	explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue();
	bool hasAlivePlayer() const;
	bool hasAlivePlayerTwo() const;
	bool hasPlayerReachedEnd() const;
	void updateSounds();

private:
	void loadTextures();
	void buildScene();
	void adaptPlayerPosition();
	void adaptPlayerTwoPosition();
	void adaptPlayerVelocity();
	void adaptPlayerTwoVelocity();
	void handleCollisions();
	void KillEmAll(Tank& player); //Added by me for DA BOMB!!! - Jason Lynch 

	void spawnEnemies();
	void addEnemies();
	void addObstacles(); //Adds obstacles to scene - Jason Lynch
	void addPickups(); //Adds pickups to scene - Jason Lynch 
	void playerOneBase(); //Adds obstacles near player one to scene - Jason Lynch 
	void playerTwoBase(); //Adds obstacles near player two to scene - Jason Lynch 
	void teslaobstacles();//Adds obstacles near tesla pickup to scene - Jason Lynch 
	void NukeObstacles();//Adds obstacles near nuke pickup to scene - Jason Lynch 
	void borderObstacles();//Adds obstacles around edge of map near other pickups - Jason Lynch 
	void addPickup(TankPickupID type, float x, float y, float rotation, float scaleX, float scaleY); //Info for creating a pickup - Jason Lynch 
	void addObstacle(ObstacleID type, float posX, float posY, float rotation, float scaleX, float scaleY, TextureID deathAnimation, sf::Vector2i frameSize, int numberOfFrames, int seconds, sf::Vector2f scale); //Info for adding an obstacle - Jason Lynch
	void spawnObstacles(); //Spawns obstacles into scene - Jason Lynch 
	void spawnPickups(); //Spawns pickups into scene - Jason Lynch 
	void addEnemy(TankID type, float relX, float relY);

	sf::FloatRect getBattlefieldBounds() const;
	sf::FloatRect getViewBounds() const;

	void destroyEntitiesOutsideView();

	void guideMissiles();

	struct SpawnPoint
	{
		SpawnPoint(TankID type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
		{
		}

		TankID type;
		float x;
		float y;
	};

	struct ObstacleSpawnPoint //Spawn point for obstacles and all other needed info. Based off above struct - Jason Lynch 
	{
		ObstacleSpawnPoint(ObstacleID type, float x, float y, float rotation, float scaleX, float scaleY, TextureID deathAnimation, sf::Vector2i frameSize, int numberOfFrames, int seconds, sf::Vector2f scale)
		: type(type)
			, x(x)
			, y(y)
			, rotation(rotation)
			, scaleX(scaleX)
			, scaleY(scaleY)
			, deathAnimation(deathAnimation)
			, frameSize(frameSize)
			, numberOfFrames(numberOfFrames)
			, seconds(seconds)
			, scale(scale)
		{
		}

		ObstacleID type;
		float x;
		float y;
		float rotation;
		float scaleX;
		float scaleY;
		TextureID deathAnimation;
		sf::Vector2i frameSize;
		int numberOfFrames;
		int seconds;
		sf::Vector2f scale;
	};

	struct PickupSpawnPoint //Pickup spawn point and relevant info. Based off above struct - Jason Lynch
	{
		PickupSpawnPoint(TankPickupID type, float x, float y, float rotation, float scaleX, float scaleY)
			: type(type)
			, x(x)
			, y(y)
			, rotation(rotation)
			, scaleX(scaleX)
			, scaleY(scaleY)
		{
		}

		TankPickupID type;
		float x;
		float y;
		float rotation;
		float scaleX;
		float scaleY;
	};

private:
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mCamera;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;
	bool mIsNukeTriggered;

	SceneNode mSceneGraph;
	std::array<SceneNode*, static_cast<int>(LayerID::LayerCount)> mSceneLayers;
	CommandQueue mCommandQueue;

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	sf::Vector2f mObstacleSpawnPosition;
	sf::Vector2f mSpawnPositionPlayerTwo;
	float mScrollSpeed;
	Tank* mPlayerTank;
	Tank* mPlayerTwoTank;
	bool isNukeTriggered;

	std::vector<ObstacleSpawnPoint> mObstacles; //Holds obstacle spawn points - Jason Lynch
	std::vector<PickupSpawnPoint> mPickups;
	std::vector<SpawnPoint>	mEnemySpawnPoints;
	std::vector<Tank*> mActiveEnemies;

	BloomEffect	mBloomEffect;
};