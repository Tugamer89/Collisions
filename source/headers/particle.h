#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include "drawer.h"

class Particle {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    int mass;
    int r;
    unsigned int id;
    sf::Color color;

    Particle(const sf::Vector2f& coords, unsigned int mass, unsigned int id, const sf::Color& clr, const unsigned int& seed);
    void update();
    void collide(Particle* other);
    void edges(unsigned int width, unsigned int height);
};
