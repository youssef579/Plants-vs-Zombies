#pragma once 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

struct WeatherSystem {

    static constexpr float RAIN_VOLUME = 20.f;           // Rain sound volume 
    static constexpr float RAIN_ANGLE = 15.f;            // Angle of Rain (deg)
    static constexpr float MIN_RAIN_SPEED = 600.f;       // Minimum falling speed of raindrops
    static constexpr float MAX_RAIN_SPEED = 1000.f;      // Maximum falling speed of raindrops
    static constexpr float LIGHTNING_CHANCE_MIN = 5.0f;  // Minimum seconds between lightning strikes 
    static constexpr float LIGHTNING_CHANCE_MAX = 15.0f; // Maximum seconds between lightning strikes
    static constexpr float FLASH_DURATION = 0.1f;        // How long the lightning flash stays visible (seconds)
   

    struct RainDrop {
        sf::RectangleShape shape;     // Visual representation of the drop 
        float speed;                  // Vertical falling speed of the drop  
    };

    sf::Sound* rainSound = nullptr;
    sf::Sound* thunderSound = nullptr;
    sf::SoundBuffer rainBuffer, thunderBuffer;
    sf::Clock timer;                 // Tracks time between lightning strikes 
    sf::Clock flashClock;            // Tracks how long the lightning flash stays on screen       // reduce clocks 
    sf::Clock rainClock;             // To calculate delta time (dt) for smooth movement
    bool isRaining = false;          // Switch to start/stop weather system 
    float nextStrikeIn = 5.0f;       // Random delay (seconds) until the next lightning strike  
    bool isFlashing = false;         // True if the screen is currently being lit by lightning
    sf::RectangleShape flashOverlay; // White transparent rectangle to simulate lightning flash 
    std::vector<RainDrop> drops;     // Container holding all 150 raindrop objects                    // change vector 

    WeatherSystem(); 

    ~WeatherSystem(); 

    void init(sf::Vector2u size);   
    void update(sf::Vector2u size);  
    void draw(sf::RenderWindow& targetWindow); 
};

