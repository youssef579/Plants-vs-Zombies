#include <Zombies/Zombie.hpp>
#include <Window.hpp>

std::string Zombie::types[] = {"Regular", "Conehead", "Buckethead", "Flag", "Newspaper"};
std::string Zombie::states[] = {"zombie", "attack", "die"};

Zombie Zombie::createZombie(float x, float y, Type type) {
    sf::Texture tempTexture; sf::Sprite tempSprite(tempTexture); 

    Zombie zombie = {tempSprite};

    zombie.type = type;
    zombie.state = Walking;

    zombie.position = {x, y};
    zombie.velocity = {-speeds[type], 0};

    zombie.health = healths[type];
    zombie.strength = strengths[type];

    zombie.setSprite();

    return zombie;
}

void Zombie::setSprite() {
    std::string location = "assets/Zombies/" + types[type] + (headless ? "/headless_" : "/") + states[state] + ".png";
    sf::Texture& texture = getTexture(location);
    sprite.setTexture(texture);
    sprite.setTextureRect({{0, 0}, {166, 144}});
    sprite.setOrigin(sprite.getLocalBounds().size / 2.0f);
    int count = frameCount[type][state][headless];
    sheet = Spritesheet{nullptr, 166, 144, count, 2.f / count};
}

bool Zombie::update(float dt) {
    takeDamage(0);  // Until Hit Detection is Implemented
    checkState(dt);
    draw(dt);
    return health > 0;
}

void Zombie::takeDamage(float damage) {
    health -= damage;
    if(health <= 0) {
        die();
        return;
    } else if(health <= 20) {
        headless = true;
    }
    if(damage) freezeTimer = FreezeTimer;
}

void Zombie::checkState(float dt) {
    if(state == Walking) move(dt);
    else if(state == Attacking) attack(dt);
}

void Zombie::move(float dt) {
    if(/* PlantInCell() */ 0) {
        state = Attacking;
        setSprite();
        return;
    }
    position += velocity * dt;
    sprite.setPosition(position);
}

void Zombie::attack(float dt) {
    if(/* !PlantInCell() */ 0) {
        state = Walking;
        setSprite();
        return;
    }
    attackTimer -= dt;
    if(attackTimer <= 0) {
        // hitGridCell();
        attackTimer = AttackTimer;
    }
}

void Zombie::die() {
    state = Dying;
    velocity = {0, 0};
    setSprite();
}

void Zombie::draw(float dt) {
    sheet.sprite = &sprite;
    window->draw(sprite);
    if(freezeTimer <= 0) {
        animateSpritesheet(sheet, dt);
    }
}