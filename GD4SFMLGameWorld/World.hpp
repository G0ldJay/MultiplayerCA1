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
#include "PostEffect.hpp"
#include "BloomEffect.hpp"
#include "SoundNode.hpp"
#include "SoundPlayer.hpp"
#include "Obstacle.hpp"

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

	void spawnEnemies();
	void addEnemies();
	void addObstacles();
	void addObstacle(ObstacleID type , float posX, float posY);
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
	sf::Vector2f mSpawnPositionPlayerTwo;
	float mScrollSpeed;
	Tank* mPlayerTank;
	Tank* mPlayerTwoTank;

	std::vector<Obstacle> mObstacles;
	std::vector<SpawnPoint>	mEnemySpawnPoints;
	std::vector<Tank*> mActiveEnemies;

	BloomEffect	mBloomEffect;
};