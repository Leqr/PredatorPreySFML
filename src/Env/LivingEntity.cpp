#include "LivingEntity.hpp"
#include "Application.hpp"

/*********************************************************
********************** private ****************************
*********************************************************/

bool LivingEntity::tooOld() const {
    return (age > sf::seconds(1E9));
}

/*********************************************************
********************** public ****************************
*********************************************************/

Vec2d LivingEntity::getCenter() const {
    return getPosition();
}

Vec2d LivingEntity::getPosition() const {
    return position;
}

double LivingEntity::getEnergyLevel() const {
    return energyLevel;
}

Genome* LivingEntity::getGenome() const {
    return genome;
}

void LivingEntity::setPosition() {
    if(position.x > getAppConfig().simulation_world_size){
        position.x = 0;
    }
    else if(position.x < 0){
        position.x = getAppConfig().simulation_world_size;
    }
    else if (position.y > getAppConfig().simulation_world_size){
        position.y = 0;
    }
    else if (position.y < 0) {
        position.y = getAppConfig().simulation_world_size;
    }
}

void LivingEntity::setEnergyLevel(double energy) {
    energyLevel = energy;
}

sf::Time LivingEntity::getAge() const {
    return age;
}

void LivingEntity::addAge(sf::Time dt) {
    age += dt;
}

bool LivingEntity::operator==(const LivingEntity& livingentity){
    if((position == livingentity.getPosition())and(energyLevel == livingentity.getEnergyLevel())and(age == livingentity.getAge())){
        return true;
    } else {
        return false;
    }
}

LivingEntity::LivingEntity(const Vec2d& position, double energy, Genome* motherGenome, Genome* fatherGenome)
: position(position), energyLevel(energy), age(sf::Time::Zero), genome(new Genome(motherGenome, fatherGenome))
{}

LivingEntity::~LivingEntity(){}
