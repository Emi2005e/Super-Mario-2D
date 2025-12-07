

#include "Platform.h"
#include <iostream>
#include "Player.h"
#include "Enums.h"
#include "Exceptions.h"

Platform::Platform(float in_x, float in_y, BlockType in_type, bool in_isDestructible, bool in_isEmpty, bool in_isSolid) {
    this->x = in_x;
    this->y = in_y;
    this->type = in_type;
    this->isDestructible = in_isDestructible;
    this->isEmpty = in_isEmpty;
    this->isSolid = in_isSolid;
    this->forcedReward = NONE;

    loadTextureForType(in_type);
}

void Platform::loadTextureForType(BlockType blockType) {
    std::string filename;
    switch (blockType) {
        case BRICK:     filename = "brick.png"; break;
        case ROCK:      filename = "rock.png"; break;
        case LUCKYBL:   filename = "luckybl.png"; break;
        case USEDBL:    filename = "usedbl.png"; break;
        case PIPE:      filename = "pipe.png"; break;
        case PIPETOP:   filename = "pipetop.png"; break;
        case EMPTY:     filename = "empty.png"; break;
        default:        filename = "default_block.png"; break;
    }

    sf::Image image;
    if (!image.loadFromFile(filename)) {
        throw ResourceException("Lipsa png platforma");
    }
    image.createMaskFromColor(sf::Color::White);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // Scalare
    sprite.setScale(SCALE / sprite.getLocalBounds().width, SCALE / sprite.getLocalBounds().height);
    sprite.setPosition(x * SCALE, y * SCALE);


}

Platform::Platform(const Platform& platform)
{
    this->x = platform.x;
    this->y = platform.y;
    this->type = platform.type;
    this->isDestructible = platform.isDestructible;
    this->isEmpty = platform.isEmpty;
    this->isSolid = platform.isSolid;
    this->forcedReward = platform.forcedReward;
    this->texture = platform.texture;
    this->sprite = platform.sprite;
    this->sprite.setTexture(texture);
}

Platform& Platform::operator=(const Platform& other) {
    if (this != &other) {
        this->x = other.x;
        this->y = other.y;
        this->type = other.type;
        this->isDestructible = other.isDestructible;
        this->isEmpty = other.isEmpty;
        this->isSolid = other.isSolid;
        this->forcedReward = other.forcedReward;
        this->texture = other.texture;
        this->sprite = other.sprite;
        this->sprite.setTexture(this->texture);
    }
    return *this;
}

PowerUpType Platform::HitByPlayer(const Player& player) {

    if (this->type==LUCKYBL && !this->isEmpty) {
        this->type=USEDBL;
        this->isEmpty=true;
        loadTextureForType(USEDBL);

        if (this->forcedReward != NONE) {
            return this->forcedReward;
        }

        if (player.getState()==0)
            return MUSHROOM;
        else
            if (player.getState()==1)
                return FIREFLOWER;
    }
    if (this->isDestructible && player.getState()>0) {
            this->isDestructible=false;
            this->isEmpty=true;
        loadTextureForType(EMPTY);
        return NONE;
    }
    if (this->isSolid)
        std::cout<<"Solid Block";
    return NONE;
}

void Platform::draw(sf::RenderWindow& window) {
    if (!isEmpty || type != EMPTY) {
        window.draw(sprite);
    }
}

sf::FloatRect Platform::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}