#ifndef PROIECT_POO_ENEMY_H
#define PROIECT_POO_ENEMY_H


#include <SFML/Graphics.hpp>

class Enemy {
protected:
    float x, y;
    float speed;
    bool isAlive;
    static int enemyCount;

    sf::Texture texture;
    sf::Sprite* sprite;

public:
    Enemy();
    Enemy(float startX, float startY, float startSpeed);
    virtual ~Enemy(); // definit în .cpp

    virtual void update(float dt) = 0;
    virtual void reaction() = 0;
    virtual Enemy* clone() const = 0;

    void draw(sf::RenderWindow& window);
    virtual bool canBeStomped() const { return true; }
    virtual bool hasGravity() const { return true; }
    void setY(float newY);
    void setX(float newX);
    void changeDirection();
    sf::Sprite& getSprite() const;

    [[nodiscard]] float getX() const;
    [[nodiscard]] float getY() const;
    [[nodiscard]] bool getIsAlive() const;
    void kill();

    sf::FloatRect getGlobalBounds() const;

protected:
    virtual void doDraw(sf::RenderWindow& window);
};


//──────────── GOOMBA ─────────────────

class Goomba : public Enemy {
private:
    bool movingLeft;
    float limStanga, limDreapta;
public:
    Goomba(float startX, float startY, float limStanga, float limDreapta);
    void update(float dt) override;
    void reaction() override;
    Enemy* clone() const override;
};


//──────────── KOOPA ─────────────────

class Koopa : public Enemy {
private:
    float limStanga, limDreapta;
    static int koopaCastCount;
public:
    Koopa(float startX, float startY, float limStanga, float limDreapta);
    void update(float dt) override;
    void reaction() override;
    Enemy* clone() const override;
    static int getKoopaCastCount() { return koopaCastCount; }
    static void incrementCastCount() { koopaCastCount++; }
};


//──────────── PIRANHA ─────────────────

class PiranhaPlant : public Enemy {
private:
    float originalY;
    bool goingUp;
    float limSus, limJos;
    float timer = 0.f;
public:
    PiranhaPlant(float startX, float startY, float limSus, float limJos);
    void update(float dt) override;
    void reaction() override;
    bool canBeStomped() const override { return false; }
    bool hasGravity() const override { return false; }
    Enemy* clone() const override;
};

#endif // PROIECT_POO_ENEMY_H
