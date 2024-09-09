#include "headers/drawer.h"

using namespace std;

Drawer::Drawer(unsigned int width, unsigned int height, const std::string& title_) {
    windowSize = {width, height};
    title = title_;
    window = new sf::RenderWindow(sf::VideoMode(width, height), title);
}

Drawer::~Drawer() {
    delete window;
}

void Drawer::eventHandler() {
    sf::Event event;

    while (window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window->close();
            break;
        
        default:
            break;
        }
    }
}

void Drawer::clearScreen(const sf::Color& color) {
    window->clear(color);
}

void Drawer::drawCircle(const sf::Vector2f& center, unsigned int radius, const sf::Color& color) {
    sf::CircleShape circle(radius);

    circle.setPosition(center.x - radius, center.y - radius);
    circle.setFillColor(color);

    window->draw(circle);
}

void Drawer::drawText(const string& text, unsigned int size, const sf::Vector2f& pos, const sf::Color& color) {
    sf::Text txt;
    sf::Font font;

    font.loadFromFile("OpenSans.ttf");
    txt.setFont(font);

    txt.setString(text);
    txt.setCharacterSize(size);
    txt.setFillColor(color);
    txt.setStyle(sf::Text::Style::Regular);

    float x = (pos.x - txt.getLocalBounds().width / 2);
    float y = (pos.y - txt.getLocalBounds().height / 2);
    txt.setPosition(x, y);

    window->draw(txt);
}
