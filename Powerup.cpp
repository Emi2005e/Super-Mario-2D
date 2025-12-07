#include "Powerup.h"
#include <iostream>
#include "Exceptions.h"

// Constanta pentru scalare (60 pixeli = 1 bloc)
const float scale = 60.0f;

Powerup::Powerup(float startX, float startY, PowerUpType type) : x(startX), y(startY), type(type) {
    this->active=true;
    this->spawning=true;
    this->spawnTargetY=(startY-1.0f)* scale;
    this->px = x * SCALE;
    this->py = y * SCALE;
    this->velocityY = 0.0f;
    this->speed = 0.10f * 60.0f;
    std::string filename;
    switch (type) {
        case MUSHROOM:
            filename = "mushroom.png";
            this->speed = 0.10f * 60.0f;
            break;
        case FIREFLOWER:
            filename = "fireflower.png";
            this->speed = 0.0f;
            break;
        case STAR:
            filename = "star.png";
            this->speed = 0.13f * 60.0f;
            break;
        default:
            filename = "mushroom.png";
            this->speed = 0.10f * 60.0f;
            break;
    }
    if (!texture.loadFromFile(filename))
        throw ResourceException("Lipsa png powerup");
    sprite.setTexture(texture);
    if (type==MUSHROOM)
        this->speed=0.10f * 60.0f;
    else
        if (type==FIREFLOWER)
            this->speed=0.0f;
    else
        if (type==STAR)
            this->speed=0.15f*60.0f;
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    if (type==MUSHROOM)
        sprite.setScale(0.18f, 0.18f); //aici setez marimea sprite ului
    if (type==FIREFLOWER)
        sprite.setScale(0.05f, 0.05f);
    if (type==STAR)
        sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(px, py);

}


Powerup::Powerup(const Powerup& other) {
    this->x = other.x;
    this->y = other.y;
    this->type = other.type;
    this->active = other.active;
    this->spawning = other.spawning;
    this->spawnTargetY = other.spawnTargetY;
    this->px = other.px;
    this->py = other.py;
    this->velocityY = other.velocityY;
    this->speed = other.speed;

    this->texture = other.texture;
    this->sprite = other.sprite;
    this->sprite.setTexture(this->texture);
}

void Powerup::update(float dt, const std::vector<Platform>& platforms) {
    if (!active) return;

    if (spawning) {
        py -= 2.0f * 60.0f * dt; // Viteza de urcare

        if (py < spawnTargetY) {
            py = spawnTargetY;
            spawning = false;
        }

        sprite.setPosition(px, py);
        x = px / 60.0f;
        y = py / 60.0f;
        return;
    }

    velocityY += 50.0f * dt;

    if (velocityY > 20.0f) velocityY = 20.0f;

    float moveY = velocityY * dt * 60.0f;
    float nextY_Pixel = py + moveY;

    sf::FloatRect futureBounds = getGlobalBounds();
    futureBounds.top = nextY_Pixel;

    sf::FloatRect feetBox = futureBounds;
    feetBox.top = futureBounds.top + futureBounds.height - 10.0f;
    feetBox.height = 10.0f;
    feetBox.left += 5.0f;
    feetBox.width -= 10.0f;

    sf::FloatRect headBox = futureBounds;
    headBox.height = 10.0f;
    headBox.left += 5.0f;
    headBox.width -= 10.0f;

    bool hitY = false;

    for (const auto& platform : platforms) {
        sf::FloatRect pBounds = platform.getGlobalBounds();

        if (velocityY > 0) {
            if (feetBox.intersects(pBounds)) {
                float currentBottom = py + getGlobalBounds().height / 2.0f;

                if (currentBottom <= pBounds.top + 10.0f) {
                    hitY = true;
                    float h = getGlobalBounds().height;

                    if (type == STAR) {
                        //bounce
                        velocityY = -18.0f;
                        nextY_Pixel = pBounds.top - h / 2.0f - 1.0f;
                    }
                    else {
                        velocityY = 0.0f;

                        nextY_Pixel = pBounds.top - h / 2.0f;
                    }
                    break;
                }
            }
        }
        else if (velocityY < 0) {
            if (headBox.intersects(pBounds)) {
                if (platform.getType() != EMPTY) {
                    hitY = true;
                    velocityY = 0.0f; // Stop la tavan
                    nextY_Pixel = pBounds.top + pBounds.height;
                    break;
                }
            }
        }
    }

    //noua pozitie pe Y
    py = nextY_Pixel;
    y = py / 60.0f;


    float nextX = x + speed * dt;

    // --- SENZOR CORP
    sf::FloatRect torsoBox = getGlobalBounds();
    torsoBox.left = nextX * 60.0f;
    torsoBox.top = py + 10.0f;

    // Tai mai mult din picioare (25px)
    torsoBox.height -= 35.0f;

    for (const auto& platform : platforms) {
        if (platform.getType() != EMPTY) {

            if (torsoBox.intersects(platform.getGlobalBounds())) {
                // LOVIRE ZID
                speed = -speed;
                nextX = x;
                break;
            }
        }
    }

    //noua pozitie pe X
    x = nextX;
    px = x * 60.0f;

    //Sprite-ul final
    sprite.setPosition(px, py);
}

void Powerup::draw(sf::RenderWindow& window) {
    if (active) window.draw(sprite);
}

sf::FloatRect Powerup::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}