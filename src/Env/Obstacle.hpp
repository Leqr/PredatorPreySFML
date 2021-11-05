#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP
#include <Utility/Vec2d.hpp>

class Obstacle {
public:
    virtual Vec2d getCenter() const = 0;
    virtual double getRadius() const = 0;
    bool isColliding (Obstacle* other) const;
    bool isPointInside (const Vec2d& point) const;
    bool operator|(Obstacle* body1) const;
    bool operator> (const Vec2d& point) const;
};

#endif
