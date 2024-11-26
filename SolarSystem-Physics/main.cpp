#include <SFML/Graphics.hpp>
#include "PhysicsHandler.h"
#include <vector>
#include <deque>
#include <cmath>
#include <filesystem>

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Simple Solar System", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./blackspace.PNG")) {  //DON'T FORGET PUT THE CORRECT PATH FOR THE IMAGE
        std::cerr << "Error: Failed to load background image from 'assets/background.jpg'.\n";
        return -1; 
    }
    

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

  
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    if (textureSize.x == 0 || textureSize.y == 0) {
        std::cerr << "Error: Background texture has invalid dimensions (width or height is 0).\n";
        return -1;
    }

    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    window.setFramerateLimit(60);

    sf::CircleShape star(50.f);
    star.setFillColor(sf::Color::Yellow);
    star.setOrigin(50.f, 50.f);
    star.setPosition(400.f, 300.f);

    std::vector<sf::CircleShape> planets;
    std::vector<std::deque<sf::CircleShape>> planetTrails;

    PhysicsHandler physics;
    physics.addStar(400.f, 300.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                sf::CircleShape planet(10.f);
                planet.setFillColor(sf::Color(224, 167, 94, 255));
                planet.setOrigin(10.f, 10.f);
                planet.setPosition(mousePos);

                planets.push_back(planet);
                physics.addPlanet(mousePos.x, mousePos.y);
                planetTrails.emplace_back();
            }
        }

        physics.update();

        for (size_t i = 0; i < planets.size(); ++i) {
            auto position = physics.getPlanetPosition(i);
            planets[i].setPosition(position.x, position.y);

            sf::CircleShape trailPoint(1.f);
            trailPoint.setFillColor(sf::Color(245, 231, 178, 255));
            trailPoint.setOrigin(3.f, 3.f);
            trailPoint.setPosition(position);

            planetTrails[i].push_back(trailPoint);

            if (planetTrails[i].size() > 130) {
                planetTrails[i].pop_front();
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(star);
        for (size_t i = 0; i < planets.size(); ++i) {
            for (const auto& trailPoint : planetTrails[i]) {
                window.draw(trailPoint);
            }
            window.draw(planets[i]);
        }
        window.display();
    }

    return 0;
}



