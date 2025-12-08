#ifndef PROIECT_POO_POWERUP_H
#define PROIECT_POO_POWERUP_H

#include "Enums.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Platform.h"

class Powerup {
private:
    float x, y;
    float px, py;
    float velocityY;
    float speed;

    bool active;
    bool spawning;
    float spawnTargetY;

    PowerUpType type;
    sf::Sprite* sprite;
    sf::Texture texture;

public:
    Powerup(float startX, float startY, PowerUpType type);
    Powerup(const Powerup& other);
    ~Powerup();

    Powerup& operator=(const Powerup& other);

    void update(float dt, const std::vector<Platform>& platforms);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getGlobalBounds() const;

    bool isActive() const { return active; }
    void kill() { active = false; }
    PowerUpType getType() const { return type; }
};

#endif
