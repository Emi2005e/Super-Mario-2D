#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"
#include "Platform.h"
#include "Level.h"

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({1280u, 720u}), "Mario OOP");
        window.setFramerateLimit(60);

        // View (camera)
        sf::View view;
        view.setSize({1280.f, 720.f});
        view.setCenter({1280.f / 2.f, 720.f / 2.f});

        Player player(2.f, 5.f, 0, 0.0f, 0.0047f, 6.f);
        Level level;
        level.loadFromFile("level1.txt");

        sf::Clock clock;

        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            if (dt > 0.05f) dt = 0.05f;

            while (const auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
            }

            level.update(player, dt);

            float playerX = player.getSprite().getPosition().x;

            float mapWidthPixels = 2400.0f;
            float windowHalfWidth = 1280.0f / 2.0f;
            float leftLimit  = windowHalfWidth;
            float rightLimit = mapWidthPixels - windowHalfWidth;

            float cameraX = std::max(leftLimit, std::min(playerX, rightLimit));
            float cameraY = 720.0f / 2.0f;

            view.setCenter({std::round(cameraX), std::round(cameraY)});
            window.setView(view);

            window.clear();
            level.draw(window);
            player.draw(window);
            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
