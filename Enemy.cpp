#include "Enemy.h"
#include "Player.h"
#include <iostream>
#include "Exceptions.h"

int Enemy::enemyCount = 0;

Enemy::Enemy()
    : x(0), y(0), speed(0), isAlive(true), sprite(nullptr)
{
    enemyCount++;
}

Enemy::Enemy(float startX, float startY, float startSpeed)
    : x(startX), y(startY), speed(startSpeed), isAlive(true), sprite(nullptr)
{
    enemyCount++;
}

Enemy::~Enemy() {
    delete sprite;
    sprite = nullptr;
    enemyCount--;
}

void Enemy::setX(float newX) {
    x = newX;
    // Important: Actualizăm și poziția vizuală (Sprite-ul)
    // Calculăm pixelii (x * SCALE) și păstrăm Y-ul curent
    if(sprite) {
        float currentPixelY = sprite->getPosition().y;
        sprite->setPosition({x * SCALE, currentPixelY});
    }
}

void Enemy::setY(float newY) {
    y = newY;
    // CRITIC: Actualizăm sprite-ul imediat pentru ca următoarele verificări de coliziune
    // din același frame (Level::update are mai multe pass-uri) să fie corecte.
    if(sprite) {
        float currentPixelX = sprite->getPosition().x;
        sprite->setPosition({currentPixelX, y * SCALE});
    }
}



void Enemy::changeDirection() {
    // Inversăm viteza (presupunând că ai o variabilă 'speed' sau 'velocity')
    speed = -speed;

    // Opțional: Poți face flip la sprite aici dacă vrei
    // if (speed > 0) sprite->setScale({1.5f, 1.5f});
    // else sprite->setScale({-1.5f, 1.5f});
}

sf::Sprite& Enemy::getSprite() const {
    return *sprite;
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!isAlive || !sprite) return;
    sprite->setPosition({x * 60.f, y * 60.f});
    window.draw(*sprite);
    doDraw(window);
}

void Enemy::doDraw(sf::RenderWindow& window) {
    if (!sprite) return;
    sprite->setPosition({x * 60.f, y * 60.f});
    window.draw(*sprite);
}

// float Enemy::getX() const { return x; }
// float Enemy::getY() const { return y; }
bool Enemy::getIsAlive() const { return isAlive; }
void Enemy::kill() { isAlive = false; }

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite ? sprite->getGlobalBounds() : sf::FloatRect{};
}


//──────────────── GOOMBA ─────────────────

Goomba::Goomba(float startX, float startY, float limStanga, float limDreapta)
    : Enemy(startX, startY, 0.05f),
      movingLeft(true), limStanga(limStanga), limDreapta(limDreapta)
{
    sf::Image image;
    if (!image.loadFromFile("goomba.png"))
        throw ResourceException("Lipsa goomba.png");

    image.createMaskFromColor(sf::Color::White);
    if (!texture.loadFromImage(image)) {
        throw GameException("Eroare textura goomba");
    }

    sprite = new sf::Sprite(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    sprite->setScale({0.05f, 0.05f});
    sprite->setPosition({x * 60.f, y * 60.f});
}

void Goomba::update(float dt) {
    //Detectare lovire zid

    if (speed < 0) {
        speed = std::abs(speed);
        movingLeft = !movingLeft; // Inversare directie
    }

    // Calc distanta miscare
    float moveAmount = speed * dt * 60.f;


    if (movingLeft)
        x -= moveAmount;
    else
        x += moveAmount;

    // 4. VERIFICARE LIMITE PATRULARE
    if (x <= limStanga) {
        x = limStanga;
        movingLeft = false;
    }
    if (x >= limDreapta) {
        x = limDreapta;
        movingLeft = true;
    }

    //oglindire
    sprite->setScale({movingLeft ? -0.05f : 0.05f, 0.05f});
}

void Goomba::reaction() { std::cout << "Goomba strivit!\n"; }
Enemy* Goomba::clone() const { return new Goomba(*this); }


//──────────────── KOOPA ─────────────────

Koopa::Koopa(float startX, float startY, float limStanga, float limDreapta)
    : Enemy(startX, startY, 0.07f),
      limStanga(limStanga), limDreapta(limDreapta)
{
    sf::Image image;
    if (!image.loadFromFile("koopa.png"))
        throw ResourceException("Lipsa koopa.png");

    if (!texture.loadFromImage(image)) {
        throw GameException("Eroare textura Koopa");
    }
    sprite = new sf::Sprite(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite->setScale({0.05f, 0.05f});

    // Ajustare spawn
    this->x = startX + 0.5f;
    this->y = startY - 0.5f;
    sprite->setPosition({this->x * 60.f, this->y * 60.f});
}

void Koopa::update(float dt) {


// x -= speed,pozitiv = stânga

    // Flip Sprite
    if (speed>0) sprite->setScale({0.05f, 0.05f});
    else sprite->setScale({-0.05f, 0.05f});

    x -= speed * dt * 60.f;


    if (x < limStanga) x = limDreapta;
    if (x > limDreapta + 10.0f) x = limStanga;
}

void Koopa::reaction() { std::cout << "Koopa s-a ascuns!\n"; }
Enemy* Koopa::clone() const { return new Koopa(*this); }


//──────────────── PIRANHA PLANT ─────────────────-

PiranhaPlant::PiranhaPlant(float startX, float startY, float limSus, float limJos)
    : Enemy(startX, startY, 0.03f),
      originalY((startY * 60.f + 80.f) / 57.5f),
      goingUp(true), limSus(limSus), limJos(limJos),
      timer(0.f)
{
    sf::Image image;
    if (!image.loadFromFile("plant.png"))
        throw ResourceException("Lipsa plant.png");

    if (!texture.loadFromImage(image)) {
        throw GameException("Eroare textura plant");
    }

    sprite = new sf::Sprite(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    sprite->setScale({0.1f, 0.1f});

    this->x = startX + 0.5f;

    y = originalY;

    //Setare pozitie
    sprite->setPosition({x * 60.f, y * 60.f});
}

void PiranhaPlant::update(float dt) {
    float moveSpeed = speed * dt * 60.f;

    if (goingUp) {
        y -= moveSpeed;
        if (y <= originalY - limSus) {
            y = originalY - limSus;
            timer += dt;
            if (timer >= 2.f) {
                timer = 0.f;
                goingUp = false;
            }
        }
    }
    else {
        y += moveSpeed;
        if (y >= originalY) {
            y = originalY;
            timer += dt;
            if (timer >= 2.f) {
                timer = 0.f;
                goingUp = true;
            }
        }
    }
}

void PiranhaPlant::reaction() { std::cout << "DAMAGE!\n"; }
Enemy* PiranhaPlant::clone() const { return new PiranhaPlant(*this); }
int Koopa::koopaCastCount = 0;