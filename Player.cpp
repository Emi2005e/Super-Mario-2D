#include "Player.h"
#include "Exceptions.h"

Player::Player() : state(0), x(0.0f), y(0.0f), jump(0.0f), gravity(0.0f), speed(0.0f), onGround(false) {}

Player::Player(float in_x, float in_y, int in_state, float in_jump, float in_gravity, float in_speed) {
    this->x = in_x;
    this->y = in_y;
    this->state = in_state;
    this->jump = in_jump;       // Viteza verticala inițiala
    this->gravity = in_gravity; // Cât de tare trage pământul
    this->speed = in_speed;
    this->onGround=false;
    this->facingRight=true;
    this->forcedJump = false;

    px = x * SCALE;
    py = y * SCALE;

    bool t1 = texIdle.loadFromFile("mario_idle.png");
    bool t2 = texRun1.loadFromFile("mario_run1.png");
    bool t3 = texRun2.loadFromFile("mario_run2.png");
    bool t4 = bigIdle.loadFromFile("big_idle.PNG");
    bool t5 = bigRun1.loadFromFile("big_run1.PNG");
    bool t6 = bigRun2.loadFromFile("big_run2.PNG");
    bool t7 = fireIdle.loadFromFile("fire_idle.PNG");
    bool t8 = fireRun1.loadFromFile("fire_run1.PNG");
    bool t9 = fireRun2.loadFromFile("fire_run2.PNG");
    bool t10 = fireAtk.loadFromFile("fire_atk.png");

    if (!t1 || !t2 || !t3 || !t4 || !t5 || !t6 || !t7 || !t8 || !t9 || !t10) {
            throw ResourceException("Eroare png");

    }



    this->state = -1;
    setState(in_state); // Setez pointerii corecti

    // 3. Setari Sprite initiale
    sprite.setTexture(*pCurrentIdle);
    sprite.setScale(3.5f, 3.5f);
    sprite.setPosition(px, py);

    // Centrare origine
    sf::Vector2u sz = pCurrentIdle->getSize();
    sprite.setOrigin(sz.x / 2.0f, sz.y / 2.0f);
}


Player::Player(const Player& other)
    : x(other.x), y(other.y), state(other.state),
      jump(other.jump), gravity(other.gravity), speed(other.speed),
      texIdle(other.texIdle), texRun1(other.texRun1), texRun2(other.texRun2),
      bigIdle(other.bigIdle), bigRun1(other.bigRun1), bigRun2(other.bigRun2),
      fireIdle(other.fireIdle), fireRun1(other.fireRun1), fireRun2(other.fireRun2), fireAtk(other.fireAtk),
      animTimer(other.animTimer), currentFrame(other.currentFrame),
      sprite(other.sprite)
{

    sprite.setTexture(texIdle);
}


Player& Player::operator=(const Player& other) {
    if (this != &other) {
        this->x = other.x;
        this->y = other.y;
        this->state = other.state;
        this->jump = other.jump;
        this->gravity = other.gravity;
        this->speed = other.speed;
        this->animTimer = other.animTimer;
        this->currentFrame = other.currentFrame;
        this->texIdle = other.texIdle;
        this->texRun1 = other.texRun1;
        this->texRun2 = other.texRun2;
        this->bigIdle = other.bigIdle;
        this->bigRun1 = other.bigRun1;
        this->bigRun2 = other.bigRun2;
        this->fireIdle = other.fireIdle;
        this->fireRun1 = other.fireRun1;
        this->fireRun2 = other.fireRun2;
        this->fireAtk = other.fireAtk;
        this->pCurrentIdle = other.pCurrentIdle;
        this->pCurrentRun1 = other.pCurrentRun1;
        this->pCurrentRun2 = other.pCurrentRun2;

        this->sprite = other.sprite;

        this->sprite.setTexture(this->texIdle);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player Info:\n";
    os << "Pozitie: (" << player.x << ", " << player.y << ")\n";
    os << "State: " << player.state << "\n";
    os << "Jump Velocity: " << player.jump << "\n";
    os << "Gravity: " << player.gravity << "\n";
    os << "Speed: " << player.speed << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, Player& player) {
    std::cout << "Introduceți coordonatele (x y): ";
    is >> player.x >> player.y;

    std::cout << "Player state (0=Mic, 1=Mare): ";
    is >> player.state;

    std::cout << "Jump force: ";
    is >> player.jump;

    std::cout << "Gravity: ";
    is >> player.gravity;

    std::cout << "Speed: ";
    is >> player.speed;

    return is;
}


void Player::updateX(float dt) {
    oldBounds = this->getGlobalBounds();
    prevX = px;
    prevY = py;

    float moveSpeed = speed * dt * 60.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        px -= moveSpeed;
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        px += moveSpeed;
        facingRight = true;
    }

    x = px / SCALE;
    sprite.setPosition(px, py);
}


void Player::updateY(float dt) {
    bool isJumping = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::W);

    // 1. START JUMP
    if (isJumping && onGround) {

        jump = -700.0f;
        forcedJump=false;
    }

    // 2. VARIABLE JUMP HEIGHT
    if (!forcedJump) {
        if (jump < 0 && !isJumping) {

            if (jump < -100.0f) {
                jump = -100.0f;
            }
        }
    }
    if (jump > 0) {
        forcedJump = false;
    }
    onGround = false;

    jump += 1100.0f * dt;

    if (jump > 600.0f) jump = 600.0f;

    py += jump * dt;
    y = py / SCALE;
    sprite.setPosition(px, py);
}

void Player::updateAnimation(float dt) {
    // 1. SELECTARE TEXTURĂ (Alegem poza corecta)
    sf::Texture* currentTexture = pCurrentIdle; // Default
    bool isMoving = std::abs(px - prevX) > 0.1f;

    if (!onGround) {
        currentTexture = pCurrentRun2; // Saritura
    }
    else if (isMoving) {
        animTimer += dt;
        if (animTimer > 0.1f) {
            animTimer = 0.0f;
            currentFrame = (currentFrame == 0) ? 1 : 0;
        }
        currentTexture = (currentFrame == 0) ? pCurrentRun1 : pCurrentRun2;
    }
    else {
        currentTexture = pCurrentIdle;
    }

    if (currentTexture == nullptr) return;

    sprite.setTexture(*currentTexture);

    // Toata imaginea
    sf::Vector2u texSize = currentTexture->getSize();
    sprite.setTextureRect(sf::IntRect(0, 0, texSize.x, texSize.y));

    // Setare punct de control imagine JOS, la mijlocul picioarelor.
    // (x / 2, y) orizontal centru, vertical jos
    sprite.setOrigin(texSize.x / 2.0f, (float)texSize.y);

    //oglindire
    float scale = 3.5f;
    if (facingRight) {
        sprite.setScale(scale, scale);
    } else {
        sprite.setScale(-scale, scale);
    }



    //  unde este PODEAUA FIZICA
    // (Centru + Jumatate din Inaltimea Fizica)
    float hitboxHeight = (state == 0) ? 60.0f : 110.0f;
    float floorY = py + (hitboxHeight / 2.0f);

    //sprite fix pe podea
    sprite.setPosition(px, floorY);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

[[nodiscard]] int Player::getState() const {return state;}
[[nodiscard]] float Player::getJump() const {return jump;}
[[nodiscard]] float Player::getX() const { return x; }
[[nodiscard]] float Player::getY() const { return y; }




sf::Sprite& Player::getSprite() {
    return sprite;
}


sf::FloatRect Player::getGlobalBounds() const {
    float width = 40.0f;

    float height = (state == 0) ? 60.0f : 110.0f;

    float left = px - (width / 2.0f);
    float top = py - (height / 2.0f);

    return sf::FloatRect(left, top, width, height);
}

void Player::takedmg() {
    if (this->state > 0) {
        std::cout << "Ai luat damage\n";
        int newState = this->state - 1;

        this->state = -1;
        setState(newState);
    }
    else {
        std::cout << "YOU DIED\n";
//resetare nivel pt dtvt
    }
}

void Player::bounce() {
    this->jump = -600.0f; // Putere bounce
    this->forcedJump = true;
    this->onGround = false;
}

void Player::setState(int s) {
    if (this->state == s) return;
    bool grow=(this->state == 0 && s>0);
    this->state = s;
    if (state==0) {
        pCurrentIdle=&texIdle;
        pCurrentRun1=&texRun1;
        pCurrentRun2=&texRun2;
    }
    else
    if (state==1) {
        pCurrentIdle=&bigIdle;
        pCurrentRun1=&bigRun1;
        pCurrentRun2=&bigRun2;
    }
    else
    if (state==2) {
        pCurrentIdle=&fireIdle;
        pCurrentRun1=&fireRun1;
        pCurrentRun2=&fireRun2;
    }

    if (grow) {
        py -= 50.0f;
        y = py / SCALE;
        sprite.setPosition(px, py);
    }
}

void Player::setOnGround(bool g) {
    onGround=g;
}

void Player::setX(float newX) {
    x = newX;
    px = x * SCALE;
    sprite.setPosition(px, py);
}

void Player::setY(float newY) {
    y = newY;
    py = y * SCALE;
    sprite.setPosition(px, py);
}

sf::FloatRect Player::getOldBounds() const { return oldBounds; }

float Player::getPrevX() const { return prevX; }
float Player::getPrevY() const { return prevY; }
