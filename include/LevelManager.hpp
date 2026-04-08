#pragma once
#include <SFML/Graphics.hpp>
#include <string>


struct LevelManager {


	sf::Texture backGroundTexture;
	sf::Sprite* backGroundSprite = nullptr;
	sf::Texture grassTexture;
	sf::Sprite* grassSprite = nullptr;
	sf::Texture rollTexture;
	sf::Sprite* rollSprite = nullptr;
	sf::Texture capTexture;
	sf::Sprite* capSprite = nullptr;

	float currentX = 0.0f;
	float targetX = 0.0f;
	bool isRolling = false;

	static constexpr float rollSpeed = 250.0f;


	void init();
	void update(float dt);
	void draw(sf::RenderWindow& window);
	bool isFinished() const { return !isRolling && currentX >= targetX; }

};


extern LevelManager dayLevel;