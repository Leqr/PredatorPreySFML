#ifndef TOXICPLANT_HPP
#define TOXICPLANT_HPP
#include <Utility/Utility.hpp>
#include <iostream>
#include <Utility/Constants.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include "Plant.hpp"
#include "LivingEntity.hpp"
#include "Application.hpp"
#include "Animal.hpp"


class ToxicPlant : public Plant {
private:
    std::string getTexture() const;
    
public:
    void specificAction(Animal* animal);
    void addSamePlant(Vec2d position, double energyLevel = getAppConfig().plant_initial_energy);
    ToxicPlant(Vec2d position, double energyLevel  = getAppConfig().plant_initial_energy);
};

#endif
