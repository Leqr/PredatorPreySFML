/*
 * prjsv 2015, 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#ifndef INFOSV_DRAWABLE_HPP
#define INFOSV_DRAWABLE_HPP

#include <SFML/Graphics.hpp>

/*!
 * @class Drawable
 *
 * @brief Represents an entity that can be represented graphically
 */
class Drawable
{
public:
    virtual void drawOn(sf::RenderTarget& target) const = 0;

    virtual ~Drawable() = default;
};

#endif // INFOSV_DRAWABLE_HPP
