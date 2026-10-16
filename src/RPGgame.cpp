#include <SFML/Graphics.hpp>
#include <iostream>

int main(int, char**) {

    sf::RenderWindow window(sf::VideoMode({800, 600}), "RPGgame");
    window.setFramerateLimit(60);

    sf::CircleShape circle(40.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({100.f, 100.f});

    sf::RectangleShape rect ({120.f, 60.f});
    rect.setFillColor(sf::Color(200, 50, 50));
    rect.setPosition({400.f, 300.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(30, 30, 40));
        window.draw(circle);
        window.draw(rect);
        window.display();
    }


    return 0;
}