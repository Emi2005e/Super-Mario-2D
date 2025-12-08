//
// Created by bicsa on 11/19/2025.
//

#ifndef PROIECT_POO_PLATFORM_H
#define PROIECT_POO_PLATFORM_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Enums.h"

class Platform {
private:
    float x, y;
    BlockType type;
    bool isDestructible;
    bool isEmpty;
    bool isSolid;
    PowerUpType forcedReward;
    sf::Sprite* sprite;
    sf::Texture texture;
public:
    Platform();
    Platform(float in_x, float in_y, BlockType in_type, bool in_isDestructible, bool in_isEmpty, bool in_isSolid);
    Platform(const Platform& platform);
    ~Platform();
    friend std::ostream& operator<<(std::ostream& os, const Platform& platform);
    friend std::istream& operator>>(std::istream& is, Platform& platform);
    Platform& operator=(const Platform& other);
    PowerUpType HitByPlayer(const Player& player);

    void loadTextureForType(BlockType blockType);

    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getGlobalBounds() const;

    BlockType getType() const { return type; }
    void setType(BlockType newType) {
        type = newType;
    }

    bool getIsSolid() const { return isSolid; }
    void setIsSolid(bool solid) { isSolid = solid; }
    bool getIsEmpty() const { return isEmpty; }
    void setIsEmpty(bool empty) { isEmpty = empty; }
    void setForcedReward(PowerUpType reward) { forcedReward = reward; }



};


#endif //PROIECT_POO_PLATFORM_H