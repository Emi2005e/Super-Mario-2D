//
// Created by Emi on 12/3/2025.
//

#ifndef PROIECT_POO_POWERUP_H
#define PROIECT_POO_POWERUP_H
#include "Enums.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Platform.h"

class Powerup {
private:
    float x, y;      // Pozitia logica (blocuri)
    float px, py;    // Pozitia vizuala (pixeli)
    float velocityY; // Gravitatie
    float speed;     // Viteza de miscare stanga-dreapta

    bool active;     // Daca mai exista in joc
    bool spawning;   // Daca e in animatia de iesire din bloc
    float spawnTargetY; // Pana la ce inaltime trebuie sa urce cand iese

    PowerUpType type;
    sf::Sprite sprite;
    sf::Texture texture;
public:
    Powerup(float startX, float startY, PowerUpType type);
    Powerup(const Powerup& other);
    ~Powerup() = default;
    friend std::ostream& operator<<(std::ostream& os, const Powerup& powerup);
    friend std::istream& operator>>(std::istream& is, Powerup& powerup);
    Powerup& operator=(const Powerup& other);
    void update(float dt, const std::vector<Platform>& platforms);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
    bool isActive() const { return active; }
    void kill() { active = false; } // Cand o ia Mario, dispare
    PowerUpType getType() const { return type; }
};


#endif //PROIECT_POO_POWERUP_H