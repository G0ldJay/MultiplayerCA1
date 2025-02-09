//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "MusicID.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

class MusicPlayer : private sf::NonCopyable
{
public:
	MusicPlayer();
	void play(MusicID theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);

private:
	sf::Music mMusic;
	std::map<MusicID, std::string> mFilenames;
	float mVolume;
};


