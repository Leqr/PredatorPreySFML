#include "Obstacle.hpp"

bool Obstacle::isColliding (Obstacle* other) const {
    if (sqrt((pow(abs(other->getCenter().x - getCenter().x),2) + pow(abs(other->getCenter().y - getCenter().y),2))) <= (other->getRadius() + this->getRadius())) return true;
    else return false;
}

bool Obstacle::isPointInside (const Vec2d& point) const {
    if (pow((point.x - getCenter().x),2) + pow((point.y - getCenter().y),2) <= pow(getRadius(),2)) return true;
    else return false;
}

bool Obstacle::operator|(Obstacle* body1) const {
    if (Obstacle::isColliding(body1)) return true;
    else return false;
}

bool Obstacle::operator> (const Vec2d& point) const {
    if (Obstacle::isPointInside(point)) return true;
    else return false;
}
