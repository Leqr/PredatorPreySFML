#ifndef CACTI_HPP
#define CACTI_HPP
#include <Utility/Utility.hpp>
#include <iostream>
#include <Utility/Constants.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include "Plant.hpp"
#include "LivingEntity.hpp"
#include "Application.hpp"
#include "Animal.hpp"


class Cacti : public Plant {
private:
    std::string getTexture() const;
    
public:
    void specificAction(Animal* animal);
    void addSamePlant(Vec2d position, double energyLevel = getAppConfig().plant_initial_energy);
    Cacti(Vec2d position, double energyLevel  = getAppConfig().plant_initial_energy);
};

#endif
