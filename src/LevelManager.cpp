#include <SFML/Graphics.hpp>
#include "LevelManager.hpp"

LevelManager dayLevel;

void LevelManager::init() {

	if (backGroundTexture.loadFromFile("assets/BackGround/background.png")) {
		if (!backGroundSprite) backGroundSprite = new sf::Sprite(backGroundTexture);
		else backGroundSprite->setTexture(backGroundTexture);
		backGroundSprite->setPosition(sf::Vector2f(0, 0));
	}


	float startX = 115.0f;
	float groundY = 260.0f;
	float rollSize = 0.8f;



	if (grassTexture.loadFromFile("assets/BackGround/sod1row.png")) {
		if (!grassSprite) grassSprite = new sf::Sprite(grassTexture);
		else grassSprite->setTexture(grassTexture);


		grassSprite->setPosition(sf::Vector2f(startX, groundY));
		targetX = static_cast<float>(grassTexture.getSize().x);
		currentX = 0.0f;
		isRolling = true;
	}


	if (rollTexture.loadFromFile("assets/BackGround/SodRoll.png")) {
		if (!rollSprite) rollSprite = new sf::Sprite(rollTexture);
		else rollSprite->setTexture(rollTexture);

		rollSprite->setScale(sf::Vector2f(rollSize, rollSize));
		rollSprite->setOrigin(sf::Vector2f(rollTexture.getSize()) / 2.0f);
	}
	

	if (capTexture.loadFromFile("assets/BackGround/SodRollCap.png")) {
		if (!capSprite) capSprite = new sf::Sprite(capTexture);
		else capSprite->setTexture(capTexture);
		capSprite->setOrigin(sf::Vector2f(capTexture.getSize()) / 2.0f);
		capSprite->setScale(sf::Vector2f(rollSize, rollSize));
		capSprite->setScale(sf::Vector2f(rollSize, rollSize));
	}
}

void LevelManager::update(float dt) {

	if (isRolling && currentX < targetX) {
		currentX += rollSpeed * dt;

		if (currentX >= targetX) {
			currentX = targetX;
			isRolling = false;
		}
		int grassWidth = static_cast<int>(currentX);
		int grassHeight = static_cast<int>(grassTexture.getSize().y);

		float startX = 105.0f;
		float groundY = 280.0f;
		float rollSize = 0.8f;
		float capOffsetY = 15.0f;

		float finalX = startX + currentX;
		float finalY = groundY + (grassHeight / 2.0f);

		float scale = 1 - ((currentX - startX) / (targetX - startX));

		grassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { grassWidth , grassHeight }));

		rollSprite->setPosition({ finalX , finalY - 25.0f });
		rollSprite->setScale(sf::Vector2f(scale * rollSize , rollSize));

		capSprite->setPosition({ finalX , finalY + capOffsetY });
		capSprite->setScale(sf::Vector2f(scale *rollSize , scale * rollSize ));

		float angle = 400.0f * dt * 0.5f;
		capSprite->rotate(sf::degrees(angle));
	}
}

void LevelManager::draw(sf::RenderWindow& window) {
	if (backGroundSprite) window.draw(*backGroundSprite);
	if (grassSprite)      window.draw(*grassSprite);
	if (isRolling && currentX < targetX) {
		if (rollSprite)       window.draw(*rollSprite);
		if (capSprite)        window.draw(*capSprite);
	}
}