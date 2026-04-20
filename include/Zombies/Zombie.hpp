#pragma once

#include <Animation.hpp>

struct Zombie {
    
    // Statics

    static constexpr int numberOfTypes = 5;

    static constexpr float speeds[] = {60, 50, 40, 80};
    static constexpr float healths[] = {100, 200, 200, 100};
    static constexpr float strengths[] = {10, 10, 10, 10};

    static constexpr float AttackTimer = 0.5;
    static constexpr float FreezeTimer = 0.2;

    enum Type {Regular, Conehead, Buckethead, Flag, Newspaper};
    enum State {Walking, Attacking, Dying};

    // For simple assets access
    static std::string types[];
    static std::string states[];

    static constexpr int frameCount[5][3][2] = {    // [Type][State][Headless / Boom]
        {{{22}, {18}}, {{21}, {11}}, {{10}, {20}}},
        {{{21}, {11}}, {{}, {}}, {{}, {}}},
        {{{15}, {11}}, {{}, {}}, {{}, {}}},
        {{{12}, {11}}, {{12}, {11}}, {{}, {}}},
        {{{}, {}}, {{}, {}}, {{}, {}}},
    };

    static Zombie createZombie(float x, float y, Type type);
    static void manageZombies(float dt);

    //

    sf::Sprite sprite;
    Spritesheet sheet;

    Type type;
    State state;

    sf::Vector2f position;
    sf::Vector2f velocity;

    float health;
    float strength;

    float attackTimer = 0;
    float freezeTimer = 0;

    bool headless = false;
    bool enraged = false;   // For Newspaper Zombie

    bool update(float dt);  // Return True if Zombie is Alive
    void setSprite();
    void takeDamage(float damage);
    void checkState(float dt);
    void move(float dt);
    void attack(float dt);
    void die();
    void draw(float dt);
};