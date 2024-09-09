#include "headers/particle.h"

using namespace std;

Particle::Particle(const sf::Vector2f& coords, unsigned int mass_, unsigned int id_, const sf::Color& clr) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 2 * M_PI);
    double angle = dis(gen);

    position = coords;
    velocity = sf::Vector2f(cos(angle), sin(angle));
    acceleration = sf::Vector2f(0, 0);
    mass = mass_;
    r = sqrt(mass) * 2;
    id = id_;
    color = clr;
}

void Particle::update() {
    velocity += acceleration;
    position += velocity;
    acceleration.x *= 0;
    acceleration.y *= 0;

    if (position.x != position.x || position.y != position.y)
        exit(-1);
}

void Particle::collide(Particle* other) {
    sf::Vector2f impactVect = other->position - position;
    float d = sqrt(impactVect.x * impactVect.x + impactVect.y * impactVect.y);

    if (d >= r + other->r)
        return;

    float overlap = d - (r + other->r);
    float magFactor = overlap * 0.5 / d;
    sf::Vector2f dir = impactVect;
    dir *= magFactor;

    position += {dir.x, dir.y};
    other->position -= {dir.x, dir.y};

    float magFactor2 = 1/d;
    d = r + other->r;
    magFactor2 *= d;
    impactVect *= magFactor2;

    int mSum = mass + other->mass;
    sf::Vector2f vDiff = other->velocity - velocity;

    double num = vDiff.x * impactVect.x + vDiff.y * impactVect.y;
    double den = mSum * d * d;

    sf::Vector2f deltaVA = impactVect;
    deltaVA *= 2 * float(other->mass * num / den);
    velocity += {deltaVA.x, deltaVA.y};

    sf::Vector2f deltaVB = impactVect;
    deltaVB *= -2 * float(mass * num / den);
    other->velocity += {deltaVB.x, deltaVB.y};
}

void Particle::edges(unsigned int width, unsigned int height) {
    if (position.x > width - r) {
        position.x = width - r;
        velocity.x *= -1;
    } else if (position.x < r) {
        position.x = r;
        velocity.x *= -1;
    }

    if (position.y > height - r) {
        position.y = height - r;
        velocity.y *= -1;
    } else if (position.y < r) {
        position.y = r;
        velocity.y *= -1;
    }
}
