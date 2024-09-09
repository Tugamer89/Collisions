#include "headers/quadtree.h"

using namespace std;

bool Rectangle::contains(const Point& point) {
    return (
        point.x >= x - w &&
        point.x <= x + w &&
        point.y >= y - h &&
        point.y <= y + h
    );
}

bool Rectangle::intersects(const Rectangle& range) {
    return (
        range.x + range.w >= x - w &&
        range.x - range.w <= x + w &&
        range.y + range.h >= y - h &&
        range.y - range.h <= y + h
    );
}

bool Circle::contains(const Point& point) {
    double d = pow(point.x - x, 2) + pow(point.y - y, 2);
    return d <= rSquared;
}

bool Circle::intersects(const Rectangle& range) {
    int xDist = abs(range.x - x);
    int yDist = abs(range.x - x);

    double edges = pow(xDist - range.w, 2) + pow(yDist - range.h, 2);

    if (xDist > r + range.w || yDist > r + range.h)
        return false;

    if (xDist <= range.w || yDist <= range.h)
        return true;
    
    return edges <= rSquared;
}

void QuadTree::subdivide() {
    int x = boundary.x;
    int y = boundary.y;
    int w = boundary.w;
    int h = boundary.h;

    Rectangle ne(x + w, y - h, w, h);
    northeast = new QuadTree(ne, capacity);
    Rectangle nw(x - w, y - h, w, h);
    northwest = new QuadTree(nw, capacity);
    Rectangle se(x + w, y + h, w, h);
    southeast = new QuadTree(se, capacity);
    Rectangle sw(x - w, y + h, w, h);
    southwest = new QuadTree(sw, capacity);

    divided = true;
}

bool QuadTree::insert(Point* point) {
    if (!boundary.contains(*point))
        return false;
    
    if (points.size() < capacity) {
        points.push_back(point);
        return true;
    }

    if (!divided)
        this->subdivide();

    if (northeast->insert(point) ||
        northwest->insert(point) ||
        southeast->insert(point) ||
        southwest->insert(point))
        return true;

    return false;
}

void QuadTree::query(Circle range, vector<Point*>& found) {
    if (!range.intersects(boundary))
        return;
    
    for (size_t i = 0; i < points.size(); ++i)
        if (range.contains(*points[i]))
            found.push_back(points[i]);

    if (!divided)
        return;

    northeast->query(range, found);
    northwest->query(range, found);
    southeast->query(range, found);
    southwest->query(range, found);
}
