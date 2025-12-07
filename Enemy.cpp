#include "Enemy.h"
#include <iostream>
#include "Exceptions.h"
int Enemy::enemyCount = 0;

Enemy::Enemy() : x(0), y(0), speed(0), isAlive(true) {enemyCount++;}

Enemy::Enemy(float startX, float startY, float startSpeed) : x(startX), y(startY), speed(startSpeed), isAlive(true) {}

void Enemy::draw(sf::RenderWindow& window) {
    if (isAlive) {
        sprite.setPosition(x*60.0f, y*60.0f);
        window.draw(sprite);
        doDraw(window);
    }
}

void Enemy::doDraw(sf::RenderWindow& window) {
    sprite.setPosition(x * 60.0f, y * 60.0f);
    window.draw(sprite);
}

float Enemy::getX() const { return x; }
float Enemy::getY() const { return y; }
bool Enemy::getIsAlive() const { return isAlive; }
void Enemy::kill() { isAlive = false; }

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

Goomba::Goomba(float startX, float startY, float limStanga, float limDreapta) : Enemy(startX, startY, 0.05f),
        movingLeft(true), limStanga(limStanga), limDreapta(limDreapta) {
    sf::Image image;
    if (!image.loadFromFile("goomba.png"))
            throw ResourceException("Lipsa goomba.png");


    image.createMaskFromColor(sf::Color::White);
    texture.loadFromImage(image);

    //setez sprite
    sprite.setTexture(texture);

    //centrare img
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite.setScale(0.05f, 0.05f);

    sprite.setPosition(x * 60.0f, y * 60.0f);


}

void Goomba::update(float dt) {
    if (movingLeft)
        x-=speed*dt*60;
    else
        x+=speed*dt*60;
    if (x<limStanga)
            movingLeft = false;
    if (x>limDreapta)
            movingLeft = true;
    if (!movingLeft) {
        sprite.setScale(0.05f, 0.05f);
    }
    else
    {
        sprite.setScale(-0.05f, 0.05f);
    }
}
void Goomba::reaction() { std::cout << "Goomba strivit!\n"; }
Enemy* Goomba::clone() const { return new Goomba(*this); }

Koopa::Koopa(float startX, float startY, float limStanga, float limDreapta) : Enemy(startX, startY, 0.07f),
            limStanga(limStanga), limDreapta(limDreapta) {
    sf::Image image;
    if (!image.loadFromFile("koopa.png"))
        throw ResourceException("Lipsa koopa.png");
    texture.loadFromImage(image);

    sprite.setTexture(texture);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite.setScale(0.08f, 0.08f);

    sprite.setPosition(x * 60.0f, y * 60.0f);
}

void Koopa::update(float dt) {
    x-=speed*dt*60;
    if (x<limStanga)
        x=limDreapta;
}

void Koopa::reaction() { std::cout << "Koopa s-a ascuns!\n"; }
Enemy* Koopa::clone() const { return new Koopa(*this); }

PiranhaPlant::PiranhaPlant(float startX, float startY, float limSus, float limJos) : Enemy(startX, startY, 0.03f),
                            goingUp(true), limSus(limSus), limJos(limJos) {
    this->x = startX + 0.5f;

    //coordonate pixel
    float startPixelY = startY * 60.0f + 80.0f; // +80 ca sa fie adanc in teava
    this->y = startPixelY / 57.5f;

    this->originalY = this->y;
    this->goingUp = true;

    sf::Image image;

    if (!image.loadFromFile("plant.png")) throw ResourceException("Lipsa plant.png");;
    
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setScale(0.1f, 0.1f);
}

void PiranhaPlant::update(float dt) {
    float moveSpeed = speed * dt * 60.0f;

    if (goingUp) {
        y -= moveSpeed;

        // Daca a ajuns sus (limita sus)
        if (y <= originalY - limSus) {
            y = originalY - limSus; //Fixare pozitie

            // Start Pauza Sus
            timer += dt;
            if (timer > 2.0f) { // Sta 2 secunde sus
                timer = 0.0f;
                goingUp = false; // Incepe sa coboare
            }
        }
    }
    else {
        //COBOARA
        y += moveSpeed;

        // Daca a ajuns jos (pozitia originala)
        if (y >= originalY) {
            y = originalY; // Fixare pozitia

            // Start Pauza Jos
            timer += dt;
            if (timer > 2.0f) { // Sta 2 secunde ascunsa
                timer = 0.0f;
                goingUp = true; // Incepe sa urce
            }
        }
    }
}



void PiranhaPlant::reaction() { std::cout << "DAMAGE!\n"; }
Enemy* PiranhaPlant::clone() const { return new PiranhaPlant(*this); }


