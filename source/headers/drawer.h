#pragma once
#include <SFML/Graphics.hpp>

class Drawer {
private:
    sf::Vector2u windowSize;
    std::string title;

public:
    sf::RenderWindow* window;

    Drawer(unsigned int width, unsigned int height, const std::string& title_ = "");
    ~Drawer();
    
    void eventHandler();
    void clearScreen(const sf::Color& color);
    void drawCircle(const sf::Vector2f& center, unsigned int radius, const sf::Color& color);
    void drawText(const std::string& text, unsigned int size, const sf::Vector2f& pos, const sf::Color& color);
};
