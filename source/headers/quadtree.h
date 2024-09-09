#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "particle.h"

class Point {
public:
    double x;
    double y;
    Particle* data;

    Point(double x_, double y_, Particle* data_) : x(x_), y(y_), data(data_) {};
};

class Rectangle {
public:
    int x;
    int y;
    int w;
    int h;

    Rectangle(int x_, int y_, int w_, int h_) :  x(x_), y(y_), w(w_), h(h_) {};
    bool contains(const Point& point);
    bool intersects(const Rectangle& range);
};

class Circle {
public:
    int x;
    int y;
    double r;
    double rSquared;

    Circle(int x_, int y_, double r_) :  x(x_), y(y_), r(r_), rSquared(r_*r_) {};
    bool contains(const Point& point);
    bool intersects(const Rectangle& range);
};

class QuadTree {
public:
    Rectangle boundary;
    unsigned int capacity;
    std::vector<Point*> points;
    bool divided;

    QuadTree* northeast;
    QuadTree* northwest;
    QuadTree* southeast;
    QuadTree* southwest;

    QuadTree(const Rectangle& boundary_, unsigned int capacity_) : boundary(boundary_), capacity(capacity_), divided(false) {};
    void subdivide();
    bool insert(Point* point);
    void query(Circle range, std::vector<Point*>& found);
};
