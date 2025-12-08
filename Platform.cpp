#include "Platform.h"
#include <iostream>
#include "Player.h"
#include "Enums.h"
#include "Exceptions.h"

Platform::Platform()
    : x(0), y(0), type(EMPTY),
      isDestructible(false), isEmpty(false), isSolid(false),
      forcedReward(NONE), sprite(nullptr)
{}

Platform::Platform(float in_x, float in_y, BlockType in_type, bool in_isDestructible, bool in_isEmpty, bool in_isSolid)
    : x(in_x), y(in_y), type(in_type),
      isDestructible(in_isDestructible), isEmpty(in_isEmpty), isSolid(in_isSolid),
      forcedReward(NONE), sprite(nullptr)
{
    loadTextureForType(in_type);
}


Platform::Platform(const Platform& platform)
    : x(platform.x), y(platform.y), type(platform.type),
      isDestructible(platform.isDestructible), isEmpty(platform.isEmpty),
      isSolid(platform.isSolid), forcedReward(platform.forcedReward),
      sprite(nullptr), texture(platform.texture)
{
    sprite = new sf::Sprite(texture);
    sprite->setPosition({x * SCALE, y * SCALE});

    if (platform.sprite) {
        sprite->setScale(platform.sprite->getScale());
    }
}

Platform::~Platform() {
    delete sprite;
    sprite = nullptr;
}


Platform& Platform::operator=(const Platform& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        type = other.type;
        isDestructible = other.isDestructible;
        isEmpty = other.isEmpty;
        isSolid = other.isSolid;
        forcedReward = other.forcedReward;
        texture = other.texture;

        if (sprite) delete sprite;
        sprite = new sf::Sprite(texture);
        sprite->setPosition({x * SCALE, y * SCALE});
        if (other.sprite) {
            sprite->setScale(other.sprite->getScale());
        }
    }
    return *this;
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
    if (!image.loadFromFile(filename))
        throw ResourceException("Lipsa png platforma");

    texture.loadFromImage(image);

    if (!sprite)
        sprite = new sf::Sprite(texture);
    else
        sprite->setTexture(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setScale({
        SCALE / bounds.size.x,
        SCALE / bounds.size.y
    });

    sprite->setPosition({x * SCALE, y * SCALE});
}

PowerUpType Platform::HitByPlayer(const Player& player) {
    if (type == LUCKYBL && !isEmpty) {
        type = USEDBL;
        isEmpty = true;
        loadTextureForType(USEDBL);

        if (forcedReward != NONE)
            return forcedReward;

        if (player.getState() == 0)
            return MUSHROOM;
        if (player.getState() == 1)
            return FIREFLOWER;
    }

    if (isDestructible && player.getState() > 0) {
        isDestructible = false;
        isEmpty = true;
        loadTextureForType(EMPTY);
        return NONE;
    }

    if (isSolid)
        std::cout << "Solid Block";

    return NONE;
}

void Platform::draw(sf::RenderWindow& window) {
    if (sprite)
        window.draw(*sprite);
}

sf::FloatRect Platform::getGlobalBounds() const {
    return sprite ? sprite->getGlobalBounds() : sf::FloatRect{};
}
