#pragma once

#include <Animation.hpp>
#include <ReAnimation/ReAnimation.hpp>
#include <Grid.hpp>

struct Zombie {
    
    // Statics

    static constexpr int numberOfTypes = 5;

    static constexpr float speeds[] = {1.0, 1.0, 1.0, 1.2}; // not used ?
    static constexpr float healths[] = {270, 370, 1100, 270 };
    static constexpr float strengths[] = { 36, 36, 36, 36 };

    static constexpr float AttackTimer = 0.33f;
    static constexpr float FreezeTimer = 10.0f; // time the zombie stays frozen

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

    static void createZombie(float x, float y, Type type, int row);
    static void manageZombies(float dt);

    //

    //sf::Sprite sprite;
    //Spritesheet sheet;
    ReAnimator reAnimator;

    Type type;
    State state;

    sf::Vector2f position;
    sf::Vector2i gridPosition;
    //sf::Vector2f velocity;
    bool onGrid = false;

    float health;
    float strength;

    float attackTimer = 0;
    float freezeTimer = 0;

    float corpseDissapearTimer = 0.0f;


    bool headless = false;
    bool enraged = false;   // For Newspaper Zombie
    bool remove = false;

    int deathCause = 0;     // 0 -> normal death, 1 -> explosion/fire


    sf::Sound sound_zombieBite;
    sf::Sound sound_zombieGulp;
    // General variables for all zombies
    static sf::SoundBuffer soundBuffer_zombieBite;
    static sf::SoundBuffer soundBuffer_zombieGulp;


    Zombie(sf::Vector2f pos, ReAnimationDefinition *def, int row);
    bool update(float dt);  // Return True if Zombie is Alive
    void setSprite();
    void takeDamage(float damage, int effect = 0);
    void checkState(float dt);
    void move(float dt);
    void attack(float dt);
    void die(int effect = 0);
    void updateDeath(float dt);
    void draw();

    // General Functions for all zombies
    static void init();
    static void updateAll(float dt);
    static void drawAll();
    static bool isZombieAliveInRow(int row, float startPosX = 0.0f);
    static void updateVolumes();
};

extern Array<Zombie> zombies[ROWS_NUMBER];
