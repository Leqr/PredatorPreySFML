#include "Cacti.hpp"
#include <Random/Random.hpp>

/*********************************************************
 ********************** private ***************************
 *********************************************************/


std::string Cacti::getTexture() const {
    return getAppConfig().cacti_texture;
}

/*********************************************************
 ********************** public ****************************
 *********************************************************/


void Cacti::specificAction(Animal* animal) {
    animal->setEnergyLevel(animal->getEnergyLevel() - getAppConfig().cacti_dammage);
}

void Cacti::addSamePlant(Vec2d position, double energyLevel) {
    getAppEnv().addEntity(new Cacti(position, energyLevel));
}

Cacti::Cacti(Vec2d position, double energyLevel)
:Plant(position, energyLevel)
{}
