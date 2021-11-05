#include "StandardPlant.hpp"
#include <Random/Random.hpp>

/*********************************************************
 ********************** private ***************************
 *********************************************************/


std::string StandardPlant::getTexture() const {
    return getAppConfig().standard_plant_texture;
}

/*********************************************************
 ********************** public ****************************
 *********************************************************/


void StandardPlant::specificAction(Animal* animal) {
}

void StandardPlant::addSamePlant(Vec2d position, double energyLevel) {
    getAppEnv().addEntity(new StandardPlant(position, energyLevel));
}

StandardPlant::StandardPlant(Vec2d position, double energyLevel)
:Plant(position, energyLevel)
{}
