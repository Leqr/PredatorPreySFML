#include "VirtuousPlant.hpp"
#include <Random/Random.hpp>

/*********************************************************
 ********************** private ***************************
 *********************************************************/


std::string VirtuousPlant::getTexture() const {
    return getAppConfig().virtuous_plant_texture;
}

/*********************************************************
 ********************** public ****************************
 *********************************************************/


void VirtuousPlant::specificAction(Animal* animal) {
    if (animal->getImmuneSystem()->getHealth() < getAppConfig().immune_health_max) {
        animal->getImmuneSystem()->setHealth(getAppConfig().immune_health_max);
    } else {
        if ((animal->getImmuneSystem())->getHealth() >= getAppConfig().immune_health_max) {
            animal->getImmuneSystem()->setHealth(animal->getImmuneSystem()->getHealth() * uniform(getAppConfig().virtuous_min_efficiency_factor, getAppConfig().virtuous_max_efficiency_factor));
        }
    }
}

void VirtuousPlant::addSamePlant(Vec2d position, double energyLevel) {
    getAppEnv().addEntity(new VirtuousPlant(position, energyLevel));
}

VirtuousPlant::VirtuousPlant(Vec2d position, double energyLevel)
:Plant(position, energyLevel)
{}
