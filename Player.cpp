#include "Player.h"
#include "Exceptions.h"
#include <iostream>
#include <cmath>

Player::Player()
    : state(0), x(0), y(0), px(0), py(0),
      jump(0), gravity(0), speed(0),
      prevX(0), prevY(0),
      forcedJump(false),
      texIdle(), texRun1(), texRun2(),
      bigIdle(), bigRun1(), bigRun2(),
      fireIdle(), fireRun1(), fireRun2(), fireAtk(),
      pCurrentIdle(nullptr), pCurrentRun1(nullptr), pCurrentRun2(nullptr),
      oldBounds(),
      sprite(nullptr),
      onGround(false),
      facingRight(true),
      animTimer(0),
      currentFrame(0)
{}

Player::Player(float in_x, float in_y, int in_state, float in_jump,
               float in_gravity, float in_speed)
    : Player()
{
    x = in_x;
    y = in_y;
    px = x * SCALE;
    py = y * SCALE;

    jump = in_jump;
    gravity = in_gravity;
    speed = in_speed;

    if (!texIdle.loadFromFile("mario_idle.png") ||
        !texRun1.loadFromFile("mario_run1.png") ||
        !texRun2.loadFromFile("mario_run2.png") ||
        !bigIdle.loadFromFile("big_idle.PNG") ||
        !bigRun1.loadFromFile("big_run1.PNG") ||
        !bigRun2.loadFromFile("big_run2.PNG") ||
        !fireIdle.loadFromFile("fire_idle.PNG") ||
        !fireRun1.loadFromFile("fire_run1.PNG") ||
        !fireRun2.loadFromFile("fire_run2.PNG") ||
        !fireAtk.loadFromFile("fire_atk.png"))
    {
        throw ResourceException("Eroare png");
    }

    setState(in_state);

    sprite = new sf::Sprite(*pCurrentIdle);
    sprite->setScale({3.5f, 3.5f});
    sprite->setPosition({px, py});
}

Player::Player(const Player& other)
    : Player()
{
    *this = other;
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {

        state = other.state;
        x = other.x; y = other.y;
        px = other.px; py = other.py;
        jump = other.jump;
        gravity = other.gravity;
        speed = other.speed;
        prevX = other.prevX;
        prevY = other.prevY;
        forcedJump = other.forcedJump;

        texIdle = other.texIdle;
        texRun1 = other.texRun1;
        texRun2 = other.texRun2;
        bigIdle = other.bigIdle;
        bigRun1 = other.bigRun1;
        bigRun2 = other.bigRun2;
        fireIdle = other.fireIdle;
        fireRun1 = other.fireRun1;
        fireRun2 = other.fireRun2;
        fireAtk = other.fireAtk;

        setState(state);

        delete sprite;
        sprite = new sf::Sprite(*pCurrentIdle);
        sprite->setScale({1.5f, 1.5f});
        sprite->setPosition({px, py});

        onGround = other.onGround;
        facingRight = other.facingRight;
        animTimer = other.animTimer;
        currentFrame = other.currentFrame;
    }
    return *this;
}

Player::~Player() {
    delete sprite;
    sprite = nullptr;
}

void Player::updateX(float dt) {
    oldBounds = getGlobalBounds();
    prevX = px;
    prevY = py;

    float moveSpeed = speed * dt * 60.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        px -= moveSpeed;
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        px += moveSpeed;
        facingRight = true;
    }

    x = px / SCALE;
    sprite->setPosition({px, py});
}

void Player::updateY(float dt) {
    bool isJumping =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

    if (isJumping && onGround) {
        jump = -700.0f;
        forcedJump = false;
    }

    if (!forcedJump && jump < 0 && !isJumping && jump < -100.0f)
        jump = -100.0f;

    if (jump > 0)
        forcedJump = false;

    onGround = false;
    jump += 1100.0f * dt;
    if (jump > 600.0f)
        jump = 600.0f;

    py += jump * dt;
    y = py / SCALE;

    sprite->setPosition({px, py});
}


void Player::updateAnimation(float dt) {
    sf::Texture* currentTexture = pCurrentIdle;
    bool isMoving = std::abs(px - prevX) > 0.1f;

    if (!onGround)
        currentTexture = pCurrentRun2;
    else if (isMoving) {
        animTimer += dt;
        if (animTimer > 0.1f) {
            animTimer = 0.f;
            currentFrame = 1 - currentFrame;
        }
        currentTexture = (currentFrame == 0) ? pCurrentRun1 : pCurrentRun2;
    }

    // Setare textura
    sprite->setTexture(*currentTexture);

    // Resetare TextureRect pentru a lua toata imaginea
    auto size = currentTexture->getSize();
    sprite->setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(size.x), static_cast<int>(size.y)}));

    //schimib originea intre animatii (efect de topait cand merge mario)
    sprite->setOrigin({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});

    //oglindire
    sprite->setScale({facingRight ? 3.5f : -3.5f, 3.5f});
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}

sf::Sprite& Player::getSprite() {
    return *sprite;
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite->getGlobalBounds();
}

void Player::takedmg() {
    if (state > 0) {
        int newState = state - 1;
        state = -1;
        setState(newState);
    } else {
        std::cout << "YOU DIED\n";
    }
}

void Player::bounce() {
    jump = -600.f;
    forcedJump = true;
    onGround = false;
}

void Player::setState(int s) {
    bool grow = (state == 0 && s > 0);
    state = s;

    switch (state) {
        case 0: pCurrentIdle = &texIdle; pCurrentRun1 = &texRun1; pCurrentRun2 = &texRun2; break;
        case 1: pCurrentIdle = &bigIdle; pCurrentRun1 = &bigRun1; pCurrentRun2 = &bigRun2; break;
        case 2: pCurrentIdle = &fireIdle; pCurrentRun1 = &fireRun1; pCurrentRun2 = &fireRun2; break;
        default:
            pCurrentIdle = &texIdle; pCurrentRun1 = &texRun1; pCurrentRun2 = &texRun2; break;
    }

    if (sprite) sprite->setTexture(*pCurrentIdle);

    if (grow) {
        py -= 50.f;
        sprite->setPosition({px, py});
    }
}

// float Player::getPrevX() const { return prevX; }
// float Player::getPrevY() const { return prevY; }
// sf::FloatRect Player::getOldBounds() const { return oldBounds; }

void Player::setX(float newX) {
    x = newX;
    px = x * SCALE;
    sprite->setPosition({px, py});
}

void Player::setY(float newY) {
    y = newY;
    py = newY * SCALE;
    sprite->setPosition({px, py});
}

int Player::getState() const {
    return state;
}

float Player::getJump() const {
    return jump;
}

void Player::setOnGround(bool g) {
    onGround = g;
}

