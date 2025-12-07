

#ifndef PROIECT_POO_PLAYER_H
#define PROIECT_POO_PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>

const float SCALE = 60.0f;

class Player {
private:
    int state;
    float x, y;
    float px, py;
    float jump;
    float gravity;
    float speed;
    float prevX;
    float prevY;
    bool forcedJump;

    sf::Texture texIdle;
    sf::Texture texRun1;
    sf::Texture texRun2;
    sf::Texture bigIdle;
    sf::Texture bigRun1;
    sf::Texture bigRun2;
    sf::Texture fireIdle;
    sf::Texture fireRun1;
    sf::Texture fireRun2;
    sf::Texture fireAtk;
    sf::Texture* pCurrentIdle;
    sf::Texture* pCurrentRun1;
    sf::Texture* pCurrentRun2;

    sf::FloatRect oldBounds;
    sf::Sprite sprite;
    bool onGround;
    bool facingRight;

    float animTimer;
    int currentFrame;
public:
    void updateAnimation(float dt);
    Player();
    Player(float in_x, float in_y, int in_state, float in_jump, float in_gravity, float in_speed);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player()=default;

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    friend std::istream& operator>>(std::istream& is, Player& player);

    [[nodiscard]] int getState() const;
    [[nodiscard]] float getJump() const;
    [[nodiscard]] float getX() const;
    [[nodiscard]] float getY() const;
    sf::Sprite& getSprite();
    sf::FloatRect getGlobalBounds() const;


    void takedmg();
    void setState(int s);
    void setX(float newX);
    void setY(float newY);
    void setOnGround(bool g);


    // void update(float dt);
    void updateX(float dt);
    void updateY(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getOldBounds() const;

    float getPrevX() const;
    float getPrevY() const;

    void setJump(float newJump) { jump = newJump; }
    void bounce();

};


#endif //PROIECT_POO_PLAYER_H

