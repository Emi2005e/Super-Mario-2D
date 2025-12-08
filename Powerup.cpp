#include "Powerup.h"
#include <iostream>
#include "Exceptions.h"

const float scale = 60.0f;

Powerup::Powerup(float startX, float startY, PowerUpType type)
    : x(startX), y(startY), type(type), sprite(nullptr)
{
    active = true;
    spawning = true;

    // Pozitii initiale in coord pixel
    px = x * scale;
    py = y * scale; //Porneste idn aprox centrul blocului
    velocityY = 0.0f;

    // Viteze
    switch (type) {
        case MUSHROOM:  speed = 100.0f; break;
        case FIREFLOWER: speed = 0.0f;   break;
        case STAR:      speed = 180.0f; break;
        default:        speed = 100.0f; break;
    }

    // Încărcare textură
    std::string filename =
        (type == MUSHROOM) ? "mushroom.png" :
        (type == FIREFLOWER) ? "fireflower.png" :
        (type == STAR) ? "star.png" : "mushroom.png";

    if (!texture.loadFromFile(filename))
        throw ResourceException("Lipsa png powerup");

    sprite = new sf::Sprite(texture);

    // 1. Setare origine în Centru
    auto localBounds = sprite->getLocalBounds();
    sprite->setOrigin({localBounds.size.x / 2.f, localBounds.size.y / 2.f});

    // 2. Setăm Scalarea
    if (type == MUSHROOM)   sprite->setScale({0.18f, 0.18f});
    if (type == FIREFLOWER) sprite->setScale({0.05f, 0.05f});
    if (type == STAR)       sprite->setScale({0.1f, 0.1f});



    sf::FloatRect globalBounds = sprite->getGlobalBounds();
    float blockTopY = startY * scale; // Coordonata Y a partii de sus a blocului
    float halfHeight = globalBounds.size.y / 2.0f;

    //scad pentru a nu intra in coliziune imediat
    spawnTargetY = blockTopY - halfHeight - 0.1f;

    //sprite ul sta in spatele blocului
    sprite->setPosition({px, py});
}

Powerup::Powerup(const Powerup& other)
    : x(other.x), y(other.y), px(other.px), py(other.py),
      velocityY(other.velocityY), speed(other.speed),
      active(other.active), spawning(other.spawning),
      spawnTargetY(other.spawnTargetY),
      type(other.type), texture(other.texture), sprite(nullptr)
{
    sprite = new sf::Sprite(texture);
    sprite->setPosition({px, py});
}

Powerup::~Powerup() {
    delete sprite;
}

Powerup& Powerup::operator=(const Powerup& other) {
    if (this != &other) {
        x = other.x; y = other.y;
        px = other.px; py = other.py;
        active = other.active;
        spawning = other.spawning;
        spawnTargetY = other.spawnTargetY;
        velocityY = other.velocityY;
        speed = other.speed;
        type = other.type;
        texture = other.texture;

        if (sprite) delete sprite;
        sprite = new sf::Sprite(texture);
        sprite->setPosition({px, py});
    }
    return *this;
}

void Powerup::update(float dt, const std::vector<Platform>& platforms) {
    if (!active) return;

    if (spawning) {
        // Iese in sus
        py -= 1.5f * scale * dt;


        if (py < spawnTargetY) {
            py = spawnTargetY; // Corectie pozitie
            spawning = false;


            if (type == STAR) {
                velocityY = -1000.0f; // Saritura initiala
            } else {
                velocityY = 0.0f;
            }
        }

        sprite->setPosition({px, py});
        x = px / scale;
        y = py / scale;
        return;
    }
        //miscare pe x
    float nextPx = px + speed * dt;
    sf::FloatRect playerBounds = sprite->getGlobalBounds();

    // Verificare coliziune pe x
    playerBounds.position.x = nextPx;
    bool collisionX = false;

    for (const auto& platform : platforms) {
        if (playerBounds.findIntersection(platform.getGlobalBounds())) {
            collisionX = true;
            break;
        }
    }

    if (collisionX) {
        speed = -speed; //coliziune se intoarce
    } else {
        px = nextPx;
    }

    // B. Mișcare pe Y gravity
    velocityY += 60.0f * scale * dt;
    if (velocityY > 20.0f * scale) velocityY = 20.0f * scale;

    float nextPy = py + velocityY * dt;


    playerBounds.position.x = px;
    playerBounds.position.y = nextPy;

    bool collisionY = false;
    for (const auto& platform : platforms) {
        sf::FloatRect wallBounds = platform.getGlobalBounds();

        if (playerBounds.findIntersection(wallBounds)) {
            //daca vine de sus
            if (velocityY > 0 && py + playerBounds.size.y / 2.0f < wallBounds.position.y + 10.0f) {

                //se pune pe bloc
                nextPy = wallBounds.position.y - playerBounds.size.y / 2.0f;

                collisionY = true;

                if (type == STAR) {
                    velocityY = -1000.0f; //sare
                } else {
                    velocityY = 0.0f;    // Ciuperca sta
                }
            }
        }
    }

    py = nextPy;
    x = px / scale;
    y = py / scale;
    sprite->setPosition({px, py});
}

void Powerup::draw(sf::RenderWindow& window) {
    if (active && sprite)
        window.draw(*sprite);
}

sf::FloatRect Powerup::getGlobalBounds() const {
    return sprite ? sprite->getGlobalBounds() : sf::FloatRect{};
}
