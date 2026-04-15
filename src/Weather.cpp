
#include <Window.hpp>
#include <cmath>


WeatherSystem gameWeather;

//WeatherSystem::WeatherSystem() {}

//WeatherSystem::~WeatherSystem() {
//    if (rainSound) delete rainSound;
//    if (thunderSound) delete thunderSound;
//}


// --- Initialization ---
// Loads audio assets, sets up the lightning overlay, and generates the initial pool of raindrops.
void WeatherSystem::init() {
    if (rainBuffer.loadFromFile("assets/sounds/rain.wav")) {
        if (!rainSound) rainSound = new sf::Sound(rainBuffer);
        rainSound->setLooping(true);                                            // Keep rain playing
        rainSound->setVolume(settings.weatherFXVolume*RAIN_VOLUME_MULTIPLIER);  // Set volume from constant
    }

    if (thunderBuffer.loadFromFile("assets/sounds/thunder.wav")) {
        if (!thunderSound) thunderSound = new sf::Sound(thunderBuffer);
        thunderSound->setVolume(settings.weatherFXVolume);
    }

    flashOverlay.setSize(sf::Vector2f(2000.0f , 2000.0f));   // Handle Screen Size
    flashOverlay.setFillColor(sf::Color::Transparent);                                            // Start invisible


    // --- Rain System ---
    drops.clear();
    for (int i = 0; i < 300; ++i) {                            // Create 150 raindrops
        RainDrop d;
        d.shape.setSize(sf::Vector2f(2.f, 15.f));              // Set drop dimenstions
        d.shape.setFillColor(sf::Color(200, 200, 255, 100));   // rain color
        d.shape.setRotation(sf::degrees(RAIN_ANGLE));          // Apply Angle
        d.shape.setPosition(sf::Vector2f(static_cast<float>(rand() % 2500), static_cast<float>(rand() % 1500)));   // Random x,y to handle rain in screen
        float speedRange = MAX_RAIN_SPEED - MIN_RAIN_SPEED;                                                            // Calculate Speed of rain variance
        d.speed = MIN_RAIN_SPEED + static_cast<float>(rand() % static_cast<int>(speedRange));                          // Assign random speed
        drops.push_back(d);
    }
}

// --- Logic Update ---
// Manages movement, audio state, and the timing of lightning strikes.
void WeatherSystem::update(float dt) {
    if (!isRaining || isPaused) {                                                                   // Not is Raining ?
      if (rainSound && rainSound->getStatus() == sf::SoundSource::Status::Playing) {    // & sound ON
            rainSound->stop();                                                            // Turn it off
        }
        return;
    }

    if (rainSound && rainSound->getStatus() != sf::SoundSource::Status::Playing) {       // To Play Audio
      rainSound->play();
    }

    //float dt = rainClock.restart().asSeconds();                                          // Calculate delta time to balance rain speed

    static float sine=std::sin(sf::degrees(RAIN_ANGLE).asRadians()),
      cosine=std::cos(sf::degrees(RAIN_ANGLE).asRadians());

    for (auto& d : drops) {
        //float angle = d.shape.getRotation().asRadians();
        //d.shape.move(sf::Vector2f(-d.speed * std::sin(angle) * dt, d.speed * std::cos(angle) * dt));    // Move raindrops
        d.shape.move(sf::Vector2f(-d.speed * sine * dt, d.speed * cosine * dt));    // Move raindrops
        if (d.shape.getPosition().y > 1500.f || d.shape.getPosition().x < -100.f) {               // Out of bonds ?
            d.shape.setPosition(sf::Vector2f(static_cast<float>(rand() % 2500), -50.f));      // reset to top
        }
    }

    timer += dt;
    flashClock += dt;
    rainClock += dt;

    if (timer >= nextStrikeIn) {                                             // Time for lightning ?
        isFlashing = true;                                                                                // Enable flash light
        flashClock = 0;                                                                             // Start flash timer
        if (thunderSound) thunderSound->play();                                                           // Play thunder sound
        timer = 0;                                                                                  // Reset Strike timer
        float strikeRange = LIGHTNING_CHANCE_MAX - LIGHTNING_CHANCE_MIN;                                  // Calculate delay range
        nextStrikeIn = LIGHTNING_CHANCE_MIN + static_cast<float>(rand() % static_cast<int>(strikeRange)); // Set next strike
    }

    if (isFlashing) {                                                    // Wihle flash is active
        if (flashClock < FLASH_DURATION) {  // Within Duration ?
            flashOverlay.setFillColor(sf::Color(255, 255, 255, 100));    // Show white flash
        }
        else {
            flashOverlay.setFillColor(sf::Color::Transparent);           // Hide flash
            isFlashing = false;                                          // Disable effect
        }
    }
}

//Updates Volume of currently active weather
void WeatherSystem::updateVolume() {
  if (thunderSound) thunderSound->setVolume(settings.weatherFXVolume);
  if (rainSound) rainSound->setVolume(settings.weatherFXVolume*RAIN_VOLUME_MULTIPLIER);
}

// --- Rendering ---
// Draws the raindrops and the lightning effect to the window.
void WeatherSystem::draw(sf::RenderWindow& targetWindow) {      // Rendering function
    if (!isRaining || !settings.weatherActive) return;          // No draw if no raining

    sf::View oldView = targetWindow.getView();
    targetWindow.setView(targetWindow.getDefaultView());
    for (auto& d : drops) {                                     // Draw all raindrops
        targetWindow.draw(d.shape);
    }
    if (isFlashing) {                                           // draw lightning on top
        flashOverlay.setSize(sf::Vector2f(targetWindow.getSize()));
        targetWindow.draw(flashOverlay);
    }
    targetWindow.setView(oldView);
}




void initWeather() {
  gameWeather.init();
  gameWeather.isRaining = false;
}
