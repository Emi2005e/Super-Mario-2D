
#ifndef PROIECT_POO_ENEMY_H
#define PROIECT_POO_ENEMY_H

#include <iostream>
#include <SFML/Graphics.hpp>


class Enemy {
protected:
    float x,y;
    int direction;
    float speed;
    float gravity;
    bool isAlive;
    static int enemyCount;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Enemy();
    Enemy(float startX, float startY, float startSpeed);
    virtual ~Enemy(){enemyCount--;}

    virtual void update(float dt)=0;
    virtual void reaction()=0;
    virtual Enemy* clone() const =0;
    void draw(sf::RenderWindow& window);
    virtual bool canBeStomped() const { return true; }
    virtual bool hasGravity() const { return true; }
    void setY(float newY) {
        y=newY;
    }
    static int getCount() { return enemyCount; }


    [[nodiscard]] float getX() const;
    [[nodiscard]] float getY() const;
    [[nodiscard]] bool getIsAlive() const;
    void kill();

    sf::FloatRect getGlobalBounds() const;

    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);
    friend std::istream& operator>>(std::istream& is, Enemy& enemy);
protected:
    virtual void doDraw(sf::RenderWindow& window);

};

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

class Koopa : public Enemy {
private:
    float limStanga, limDreapta;
public:
    Koopa(float startX, float startY, float limStanga, float limDreapta);
    void update(float dt) override;
    void reaction() override;
    Enemy* clone() const override;
};

class PiranhaPlant : public Enemy {
private:
    float originalY;
    bool goingUp;
    float limSus, limJos;
    float timer;
public:
    PiranhaPlant(float startX, float startY, float limSus, float limJos);
    void update(float dt) override;
    void reaction() override;
    bool canBeStomped() const override { return false; }
    bool hasGravity() const override { return false; }
    Enemy* clone() const override;
};




#endif //PROIECT_POO_ENEMY_H