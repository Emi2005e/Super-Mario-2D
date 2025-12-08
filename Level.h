#ifndef PROIECT_POO_LEVEL_H
#define PROIECT_POO_LEVEL_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Platform.h"
#include "Enums.h"
#include "Powerup.h"

class Level {
private:
    std::string name;
    std::vector<Enemy*> enemies;
    std::vector<Platform> platforms;
    std::vector<Powerup*> powerups;

public:
    Level();
    Level(const Level& other);
    Level& operator=(const Level& other);
    ~Level();

    void addEnemy(Enemy* enemy);
    void addPlatform(Platform platform);

    void update(Player& player, float dt);
    void draw(sf::RenderWindow& window);

    void handlePlayerInteractions(Player& player);
    void handlePlayerPlatformCollision(Player& player, bool isXAxis);

    void loadFromFile(const std::string& filename);
    void handleEnemyPlatformCollision(Enemy* enemy, bool isAxis);
};

#endif
