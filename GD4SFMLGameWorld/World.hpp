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
#include "Obstacle.hpp"
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


class World : private sf::NonCopyable
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
	void KillEmAll(Tank& player);

	void spawnEnemies();
	void addEnemies();
	void addObstacles();
	void addPickups();
	void playerOneBase();
	void playerTwoBase();
	void addPickup(TankPickupID type, float x, float y, float rotation, float scaleX, float scaleY);
	void addObstacle(ObstacleID type, float posX, float posY, float rotation, float scaleX, float scaleY, TextureID deathAnimation, sf::Vector2i frameSize, int numberOfFrames, int seconds, sf::Vector2f scale);
	void spawnObstacles();
	void spawnPickups();
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

	struct ObstacleSpawnPoint
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

	struct PickupSpawnPoint
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

	std::vector<ObstacleSpawnPoint> mObstacles;
	std::vector<PickupSpawnPoint> mPickups;
	std::vector<SpawnPoint>	mEnemySpawnPoints;
	std::vector<Tank*> mActiveEnemies;

	BloomEffect	mBloomEffect;
};