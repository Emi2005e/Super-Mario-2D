#include "Level.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>

Level::Level() {}

Level::Level(const Level& other) {
    platforms = other.platforms;
    for (const auto* enemy : other.enemies)
        enemies.push_back(enemy->clone());
    for (const auto* p : other.powerups)
        powerups.push_back(new Powerup(*p));
}

Level& Level::operator=(const Level& other) {
    if (this != &other) {
        for (auto* e : enemies) delete e;
        enemies.clear();
        for (auto* p : powerups) delete p;
        powerups.clear();
        platforms.clear();

        platforms = other.platforms;
        for (const auto* enemy : other.enemies)
            enemies.push_back(enemy->clone());
        for (const auto* p : other.powerups)
            powerups.push_back(new Powerup(*p));
    }
    return *this;
}

Level::~Level() {
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    for (auto* p : powerups) delete p;
    powerups.clear();
}

void Level::addEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void Level::addPlatform(Platform platform) {
    platforms.push_back(platform);
}


void Level::handleEnemyPlatformCollision(Enemy* enemy, bool isXAxis) {
    sf::FloatRect eBounds = enemy->getGlobalBounds();
    float buffer = 0.1f;

    for (const auto& platform : platforms) {
        sf::FloatRect bBounds = platform.getGlobalBounds();
        sf::FloatRect sensorBounds = eBounds;

        if (isXAxis) {
            float safeZoneRatio = 0.4f;
            float sensorHeight = eBounds.size.y * safeZoneRatio;
            float verticalMargin = (eBounds.size.y - sensorHeight) / 2.0f;

            sensorBounds.position.y += verticalMargin;
            sensorBounds.size.y = sensorHeight;
        }
        else {

            float safeZoneRatio = 0.2f;
            float sensorWidth = eBounds.size.x * safeZoneRatio;
            float horizontalMargin = (eBounds.size.x - sensorWidth) / 2.0f;

            sensorBounds.position.x += horizontalMargin;
            sensorBounds.size.x = sensorWidth;
        }

        if (sensorBounds.findIntersection(bBounds)) {
            if (isXAxis) {
                float halfW = eBounds.size.x / 2.0f;
                float eCenterX = eBounds.position.x + halfW;
                float bCenterX = bBounds.position.x + (bBounds.size.x / 2.0f);

                if (eCenterX < bCenterX) {
                    enemy->setX((bBounds.position.x - halfW - buffer) / SCALE);
                } else {
                    enemy->setX((bBounds.position.x + bBounds.size.x + halfW + buffer) / SCALE);
                }
                enemy->changeDirection();
            }
            else {

                if (eBounds.position.y < bBounds.position.y) {
                    float halfH = eBounds.size.y / 2.0f;
                    enemy->setY((bBounds.position.y - halfH) / SCALE);
                }
            }
        }
    }
}


void Level::update(Player& player, float dt) {
    // 1. Garbage Collector
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->getIsAlive()) {
            delete *it;
            it = enemies.erase(it);
        } else ++it;
    }

    // fizica player
    player.updateX(dt);
    handlePlayerPlatformCollision(player, true);
    player.updateY(dt);
    handlePlayerInteractions(player);
    handlePlayerPlatformCollision(player, false);

    for (auto* enemy : enemies) {

        //Miscare Orizontala
        enemy->update(dt);

        // Verificare Pereți
        if (enemy->hasGravity()) {
            handleEnemyPlatformCollision(enemy, true);
        }

        // Miscare verticala(gravitatie)
        if (enemy->hasGravity()) {
            float currentY = enemy->getSprite().getPosition().y / SCALE;
            float fallSpeed = 20.0f; //Gravitatie constanta
            enemy->setY(currentY + fallSpeed * dt);

            handleEnemyPlatformCollision(enemy, false);
        }
    }

    for (auto it = powerups.begin(); it != powerups.end();) {
        (*it)->update(dt, platforms);
        if ((*it)->getGlobalBounds().findIntersection(player.getGlobalBounds())) {
            PowerUpType type = (*it)->getType();
            if (type == MUSHROOM) player.setState(1);
            else if (type == FIREFLOWER) player.setState(2);
            (*it)->kill();
        }
        if (!(*it)->isActive()) {
            delete *it;
            it = powerups.erase(it);
        } else ++it;
    }

    player.updateAnimation(dt);
}

void Level::draw(sf::RenderWindow& window) {
    for (auto* enemy : enemies) enemy->draw(window);
    for (auto* p : powerups) p->draw(window);
    for (auto& platform : platforms) platform.draw(window);
}

void Level::handlePlayerInteractions(Player& player) {
    sf::FloatRect pBounds = player.getGlobalBounds();

    for (auto* enemy : enemies) {
        if (!enemy->getIsAlive()) continue;

        sf::FloatRect eBounds = enemy->getGlobalBounds();

        auto inter = pBounds.findIntersection(eBounds);
        if (!inter) continue;

        // SFML 3.0: size.x/y
        bool stomp = inter->size.x > inter->size.y && player.getJump() > 0;

        if (stomp && enemy->canBeStomped()) {
            enemy->reaction();
            enemy->kill();
            player.bounce();
        } else {
            enemy->reaction();
            player.takedmg();
        }
    }
}

void Level::handlePlayerPlatformCollision(Player& player, bool isXAxis) {
    sf::FloatRect pBounds = player.getGlobalBounds();

    float pHalfW = pBounds.size.x / 2.0f;
    float pHalfH = pBounds.size.y / 2.0f;

    float skin = 2.0f;

    for (auto& platform : platforms) {
        sf::FloatRect bBounds = platform.getGlobalBounds();

        sf::FloatRect checkBounds = pBounds;

        if (isXAxis) {
            checkBounds.position.y += 6.0f;
            checkBounds.size.y -= 12.0f;
        }
        else {
            checkBounds.position.x += skin;
            checkBounds.size.x -= skin * 2;
        }

        auto intersection = checkBounds.findIntersection(bBounds);

        if (!intersection) continue;

        if (isXAxis) {
            float pLeft = pBounds.position.x;
            float pRight = pBounds.position.x + pBounds.size.x;
            float bLeft = bBounds.position.x;
            float bRight = bBounds.position.x + bBounds.size.x;

            float overlapLeft = pRight - bLeft;
            float overlapRight = bRight - pLeft;

            if (overlapLeft < overlapRight) {
                player.setX((bLeft - pHalfW) / SCALE);
            } else {
                player.setX((bRight + pHalfW) / SCALE);
            }
        }
        else {
            if (pBounds.position.y < bBounds.position.y) {
                player.setY((bBounds.position.y - pHalfH) / SCALE);
                player.setOnGround(true);
                player.setJump(0);
            } else {
                player.setY((bBounds.position.y + bBounds.size.y + pHalfH) / SCALE);
                player.setJump(0);

                PowerUpType reward = platform.HitByPlayer(player);
                if (reward != NONE) {
                    float bx = bBounds.position.x / SCALE;
                    float by = bBounds.position.y / SCALE;
                    powerups.push_back(new Powerup(bx, by, reward));
                }
            }
        }
    }
}

void Level::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw LevelLoadException(filename);

    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        for (int x = 0; x < (int)line.size(); x++) {
            char c = line[x];
            switch (c) {
                case '#': platforms.emplace_back(x, y, BRICK, false, false, true); break;
                case 'R': platforms.emplace_back(x, y, ROCK, false, false, true); break;
                case 'L': platforms.emplace_back(x, y, LUCKYBL, false, false, true); break;
                case 'S': { Platform p(x, y, LUCKYBL, false, false, true);
                            p.setForcedReward(STAR);
                            platforms.push_back(p);
                          } break;
                case 'U': platforms.emplace_back(x, y, USEDBL, false, true, true); break;
                case '|': platforms.emplace_back(x, y, PIPE, false, false, true); break;
                case 'T': platforms.emplace_back(x, y, PIPETOP, false, false, true); break;
                case 'G': enemies.push_back(new Goomba(x, y, x - 3, x + 3)); break;
                case 'K': enemies.push_back(new Koopa(x, y, x - 4, x + 4)); break;
                case 'P': enemies.push_back(new PiranhaPlant(x, y, 1, 1)); break;
                default: break;
            }
        }
        y++;
    }
    file.close();
}