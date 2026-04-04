#include "Weather.hpp"
#include <cmath> 

WeatherSystem::WeatherSystem() {}

WeatherSystem::~WeatherSystem() {
    if (rainSound) delete rainSound;
    if (thunderSound) delete thunderSound;
}


// --- Initialization --- 
// Loads audio assets, sets up the lightning overlay, and generates the initial pool of raindrops.
void WeatherSystem::init(sf::Vector2u size) {
    if (rainBuffer.loadFromFile("assets/rain.wav")) {
        if (!rainSound) rainSound = new sf::Sound(rainBuffer);
        rainSound->setLooping(true);                              // Keep rain playing  
        rainSound->setVolume(RAIN_VOLUME);                        // Set volume from constant
    }

    if (thunderBuffer.loadFromFile("assets/thunder.wav")) {
        if (!thunderSound) thunderSound = new sf::Sound(thunderBuffer);
    }

    flashOverlay.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));   // Handle Screen Size
    flashOverlay.setFillColor(sf::Color::Transparent);                                            // Start invisible 


    // --- Rain System --- 
    drops.clear();     
    for (int i = 0; i < 150; ++i) {                            // Create 150 raindrops
        RainDrop d; 
        d.shape.setSize(sf::Vector2f(2.f, 15.f));              // Set drop dimenstions
        d.shape.setFillColor(sf::Color(200, 200, 255, 100));   // rain color
        d.shape.setRotation(sf::degrees(RAIN_ANGLE));          // Apply Angle
        d.shape.setPosition(sf::Vector2f(static_cast<float>(rand() % size.x), static_cast<float>(rand() % size.y)));   // Random x,y to handle rain in screen 
        float speedRange = MAX_RAIN_SPEED - MIN_RAIN_SPEED;                                                            // Calculate Speed of rain variance
        d.speed = MIN_RAIN_SPEED + static_cast<float>(rand() % static_cast<int>(speedRange));                          // Assign random speed
        drops.push_back(d);
    }
}

// --- Logic Update --- 
// Manages movement, audio state, and the timing of lightning strikes.
void WeatherSystem::update(sf::Vector2u size) {
    if (!isRaining) {                                                                     // Not is Raining ? 
        if (rainSound && rainSound->getStatus() == sf::SoundSource::Status::Playing) {    // & sound ON 
            rainSound->stop();                                                            // Turn it off 
        }
        return;
    }

    if (rainSound && rainSound->getStatus() != sf::SoundSource::Status::Playing) {       // To Play Audio 
        rainSound->play();
    }

    float dt = rainClock.restart().asSeconds();                                          // Calculate delta time to balance rain speed 

    for (auto& d : drops) {
        float angle = d.shape.getRotation().asRadians();
        d.shape.move(sf::Vector2f(-d.speed * std::sin(angle) * dt, d.speed * std::cos(angle) * dt));    // Move raindrops 
        if (d.shape.getPosition().y > (float)size.y || d.shape.getPosition().x < -50.f) {               // Out of bonds ? 
            d.shape.setPosition(sf::Vector2f(static_cast<float>(rand() % (size.x + 400)), -20.f));      // reset to top 
        }
    }

    if (timer.getElapsedTime().asSeconds() >= nextStrikeIn) {                                             // Time for lightning ? 
        isFlashing = true;                                                                                // Enable flash light
        flashClock.restart();                                                                             // Start flash timer
        if (thunderSound) thunderSound->play();                                                           // Play thunder sound 
        timer.restart();                                                                                  // Reset Strike timer
        float strikeRange = LIGHTNING_CHANCE_MAX - LIGHTNING_CHANCE_MIN;                                  // Calculate delay range 
        nextStrikeIn = LIGHTNING_CHANCE_MIN + static_cast<float>(rand() % static_cast<int>(strikeRange)); // Set next strike
    }

    if (isFlashing) {                                                    // Wihle flash is active
        if (flashClock.getElapsedTime().asSeconds() < FLASH_DURATION) {  // Within Duration ? 
            flashOverlay.setFillColor(sf::Color(255, 255, 255, 100));    // Show white flash 
        }
        else {
            flashOverlay.setFillColor(sf::Color::Transparent);           // Hide flash 
            isFlashing = false;                                          // Disable effect
        }
    }
}


// --- Rendering ---
// Draws the raindrops and the lightning effect to the window.
void WeatherSystem::draw(sf::RenderWindow& targetWindow) {      // Rendering function 
    if (!isRaining) return;                                     // No draw if no raining 
    for (auto& d : drops) {                                     // Draw all raindrops 
        targetWindow.draw(d.shape);
    } 
    if (isFlashing) {                                           // draw lightning on top 
        targetWindow.draw(flashOverlay);
    }
}