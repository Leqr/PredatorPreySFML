#include "ToxicPlant.hpp"
#include <Random/Random.hpp>

/*********************************************************
 ********************** private ***************************
 *********************************************************/


std::string ToxicPlant::getTexture() const {
    return getAppConfig().toxic_plant_texture;
}

/*********************************************************
 ********************** public ****************************
 *********************************************************/


void ToxicPlant::specificAction(Animal* animal) {
    animal->infect(new Virus);
}

void ToxicPlant::addSamePlant(Vec2d position, double energyLevel) {
    getAppEnv().addEntity(new ToxicPlant(position, energyLevel));
}

ToxicPlant::ToxicPlant(Vec2d position, double energyLevel)
:Plant(position, energyLevel)
{}
