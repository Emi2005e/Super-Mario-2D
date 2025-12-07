//
// Created by bicsa on 11/19/2025.
//

#include "Level.h"
#include "Enemy.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

#include "Player.h"
#include "Platform.h"
#include "Enums.h"
#include "Powerup.h"
#include "Exceptions.h"

Level::Level() {
    // Constructor gol momentan
}

Level::Level(const Level& other) {
    this->platforms = other.platforms;
    for (const auto* enemy : other.enemies) {
        this->enemies.push_back(enemy->clone());
    }

    for (const auto* p : other.powerups) {
        this->powerups.push_back(new Powerup(*p));
    }
}

Level& Level::operator=(const Level& other) {
    if (this != &other) {
        for (auto* e : enemies) delete e;
        enemies.clear();
        for (auto* p : powerups) delete p;
        powerups.clear();
        platforms.clear();
        this->platforms = other.platforms;

        for (const auto* enemy : other.enemies) {
            this->enemies.push_back(enemy->clone());
        }
        for (const auto* p : other.powerups) {
            this->powerups.push_back(new Powerup(*p));
        }
    }
    return *this;
}

Level::~Level() {
    for (auto* enemy:enemies)
        delete enemy;
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

void Level::update(Player& player, float dt) {

    // 1. Garbage Collector
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->getIsAlive()) {
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // FIZICA PLAYER - AXA X
    player.updateX(dt);
    handlePlayerPlatformCollision(player, true);

    // FIZICA PLAYER - AXA Y
    player.updateY(dt);
    handlePlayerInteractions(player); //
    handlePlayerPlatformCollision(player, false);

    // 4. FIZICA INAMICI
    for (auto* enemy : enemies) {
        if (enemy == nullptr) continue;
        Koopa* koopaPtr = dynamic_cast<Koopa*>(enemy);
        if (koopaPtr) {
            std::cout << "Koopa detectat\n";
        }
        if (enemy->hasGravity()) {

            // Gravitatia
            float gravityDistance = 10.0f * dt;
            float currentY = enemy->getY();
            float nextY = currentY + gravityDistance;

            //Hitbox Viitor
            sf::FloatRect currentBounds = enemy->getGlobalBounds();
            sf::FloatRect nextBounds = currentBounds;
            nextBounds.top = nextY * SCALE;

            bool willHitFloor = false;

            //Podeaua
            for (auto& platform : platforms) {
                sf::FloatRect pBounds = platform.getGlobalBounds();

                if (nextBounds.intersects(pBounds)) {
                    if (currentBounds.top + currentBounds.height <= pBounds.top + 5.0f) {
                        willHitFloor = true;
                        float enemyH = currentBounds.height;

                        // Snap pe podea
                        enemy->setY((pBounds.top - enemyH) / SCALE + 0.37f);
                        break;
                    }
                }
            }

            if (!willHitFloor) {
                enemy->setY(nextY);
            }
        }

        enemy->update(dt);
    }
    for (auto it = powerups.begin(); it != powerups.end(); ) {
        (*it)->update(dt,platforms);
        if ((*it)->getGlobalBounds().intersects(player.getGlobalBounds())) {
            PowerUpType type = (*it)->getType();
            if (type == MUSHROOM) {
                std::cout << "MUSHROOM! Mario devine mare.\n";
                player.setState(1);
            }
            else if (type == FIREFLOWER) {
                std::cout << "FIREFLOWER! Mario trage cu foc.\n";
                player.setState(2);
            }
            else if (type == STAR) {
                std::cout << "STAR! Invincibil.\n";
            }

            (*it)->kill();//Marcaj pt stergere
        }

        //Stergere daca nu mai e activ
        if (!(*it)->isActive()) {
            delete *it;
            it = powerups.erase(it);
        } else {
            ++it;
        }
    }

    player.updateAnimation(dt);
}

void Level::draw(sf::RenderWindow& window) {
    // 1. Inamici
    for (auto* enemy : enemies) enemy->draw(window);

    // 2. Powerups
    for (auto* p : powerups) {
        p->draw(window);
    }

    // 3. Platforme
    for (auto& platform : platforms) platform.draw(window);
}

void Level::handlePlayerInteractions(Player& player) {
    sf::FloatRect pBounds = player.getGlobalBounds();

    for (auto* enemy : enemies) {
        if (!enemy->getIsAlive()) continue;

        sf::FloatRect eBounds = enemy->getGlobalBounds();

        // Hitbox inamic mai mic
        sf::FloatRect eHitbox = eBounds;
        eHitbox.left += 5.0f;
        eHitbox.width -= 10.0f;
        eHitbox.top += 5.0f;
        eHitbox.height -= 10.0f;

        // suprapunere?
        sf::FloatRect intersection;
        if (pBounds.intersects(eHitbox, intersection)) {

            // 1. Logica geometrica (Vertical vs Orizontal)
            bool isVerticalCollision = intersection.width > intersection.height;
            bool isAbove = (pBounds.top + pBounds.height) < (eBounds.top + eBounds.height * 0.7f);

            if (isVerticalCollision && isAbove && enemy->canBeStomped()) {
                //STRIVIRE
                enemy->reaction();
                enemy->kill();
                player.bounce();
            }
            else {
                player.takedmg();

                // Knockback
                if (player.getX() < enemy->getX())
                    player.setX(player.getX() - 0.2f);
                else
                    player.setX(player.getX() + 0.2f);
            }
        }
    }
}

void Level::handlePlayerPlatformCollision(Player& player, bool isXAxis) {
    sf::FloatRect pBounds = player.getGlobalBounds();
    float pHalfW = pBounds.width / 2.0f;
    float pHalfH = pBounds.height / 2.0f;

    // "Skin Width" - toleranta.
    // Ignor primii 2 pixeli din margini cand verific sus/jos.
    float skinWidth = 2.0f;

    for (auto& platform : platforms) {
        sf::FloatRect bBounds = platform.getGlobalBounds();
        sf::FloatRect intersection;

        if (isXAxis) {
            // --- logica axa x
            if (pBounds.intersects(bBounds, intersection)) {

                // Daca intersectia e mai mult pe verticala decat orizontala, e gresit (ignoram)

                if (pBounds.left < bBounds.left) {
                    player.setX((bBounds.left - pHalfW) / SCALE);
                } else {
                    player.setX((bBounds.left + bBounds.width + pHalfW) / SCALE);
                }
            }
        }
        else {

            sf::FloatRect verticalSensor = pBounds;
            verticalSensor.left += skinWidth;      // Il ingustez din stanga
            verticalSensor.width -= (skinWidth * 2); // Il ingustez din dreapta

            if (verticalSensor.intersects(bBounds, intersection)) {

                if (pBounds.top < bBounds.top) {
                    // CADE PE PLATFORMA
                    player.setY((bBounds.top - pHalfH) / SCALE);
                    player.setOnGround(true);
                    player.setJump(0);
                }
                else {
                    // LOVESTE CU CAPUL
                    player.setY((bBounds.top + bBounds.height + pHalfH) / SCALE);
                    player.setJump(0);

                    // Blocuri speciale
                    PowerUpType reward = platform.HitByPlayer(player);
                    if (reward != NONE) {
                        float blockX = platform.getGlobalBounds().left / SCALE;
                        float blockY = platform.getGlobalBounds().top / SCALE;
                        powerups.push_back(new Powerup(blockX, blockY, reward));
                        std::cout << "Powerup Spawned!\n";
                    }
                }
            }
        }
    }
}

void Level::handlePlayerEnemyCollision(Player& player) {
    sf::FloatRect pBounds = player.getGlobalBounds();

    for (auto* enemy : enemies) {
        if (!enemy->getIsAlive()) continue;

        sf::FloatRect eBounds = enemy->getGlobalBounds();

        if (!pBounds.intersects(eBounds))
            continue;

        float playerBottom = pBounds.top + pBounds.height;
        float enemyTop = eBounds.top;

        bool playerIsFalling = player.getJump() > 0;

        if (playerIsFalling && playerBottom-enemyTop<20.f) {
            enemy->reaction();
            enemy->kill();

            player.setOnGround(false);
            player.setY(player.getY()-0.2f);
            return;
        }
        player.takedmg();
    }
}

void Level::loadFromFile(const std::string& level1) {
    std::ifstream file(level1);


    if (!file.is_open()) {
        throw LevelLoadException(level1);
        return;
    }

    std::string line;
    int y = 0;

    while (std::getline(file, line)) {
        for (int x = 0; x < line.size(); x++) {
            char c = line[x];

            switch (c) {
                case '#':
                    platforms.emplace_back(x, y, BRICK, false, false, true);
                    break;

                case 'R':
                    platforms.emplace_back(x, y, ROCK, false, false, true);
                    break;

                case 'L':
                    platforms.emplace_back(x, y, LUCKYBL, false, false, true);
                    break;

                case 'S': {
                    Platform p(x, y, LUCKYBL, false, false, true);

                    p.setForcedReward(STAR);

                    platforms.push_back(p);
                    break;
                }

                case 'U':
                    platforms.emplace_back(x, y, USEDBL, false, true, true);
                    break;

                case '|':
                    platforms.emplace_back(x, y, PIPE, false, false, true);
                    break;

                case 'T':
                    platforms.emplace_back(x, y, PIPETOP, false, false, true);
                    break;

                case 'G':
                    enemies.push_back(new Goomba(x, y, x - 3, x + 3));
                    break;

                case 'K':
                    enemies.push_back(new Koopa(x, y, x - 4, x + 4));
                    break;

                case 'P':
                    enemies.push_back(new PiranhaPlant(x, y, 1, 1));
                    break;

                case '.':
                default:
                    break;
            }
        }
        y++;
    }

    file.close();
}





