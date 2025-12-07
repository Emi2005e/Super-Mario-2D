#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>
#include<SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"
#include "Platform.h"
#include "Level.h"
// #include "imgui.h"
// #include "imgui-SFML.h"

int main() {
try {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Mario OOP");
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0, 0, 1280, 720));

    Player player(2, 5, 0, 0.0f, 0.0047f, 6.f);

    Level level;

    level.loadFromFile("level1.txt");

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        if (dt > 0.05f) dt = 0.05f;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        level.update(player, dt);

        float playerX = player.getSprite().getPosition().x;

        float mapWidthPixels = 2400.0f;
        float windowHalfWidth = 1280.0f / 2.0f; // adica 640.0f

        float leftLimit = windowHalfWidth;
        float rightLimit = mapWidthPixels - windowHalfWidth; // 2400 - 640 = 1760

        float cameraX = std::max(leftLimit, std::min(playerX, rightLimit));

        float cameraY = 720.0f / 2.0f;

        view.setCenter(std::round(cameraX), std::round(cameraY));
        window.setView(view);

        // -------------------------------------------------------------

        window.clear();
        level.draw(window);
        player.draw(window);
        window.display();
    }
}catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
}

    return 0;
}