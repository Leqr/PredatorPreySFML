#include "Environment.hpp"
#include "Animal.hpp"
#include "Wolf.hpp"
#include "Sheep.hpp"
#include "VirtuousPlant.hpp"
#include "StandardPlant.hpp"
#include "ToxicPlant.hpp"
#include "Application.hpp"
#include <cmath>
#include <unordered_map>
#include <Utility/Utility.hpp>

void Environment::addEntity (LivingEntity* entity) {
    Fauna.push_back(entity);
    if (entity->getFlock() >= 0) {
        if (flock.find(entity->getFlock()) == flock.end()) {
            flock[entity->getFlock()] = entity;
        }
        entity->setChief(flock[entity->getFlock()]);
    }
}

void Environment::addRandomPlant(Vec2d position) {
    if (bernoulli(getAppConfig().virtuous_plant_probability)) {
        addEntity(new VirtuousPlant(position));
    } else {
        if (bernoulli(getAppConfig().toxic_plant_probability)) {
            addEntity(new ToxicPlant(position));
        } else {
            addEntity(new StandardPlant(position));
        }
    }
}

void Environment::update(sf::Time dt) {
    for(auto& entity: Fauna) {
            entity->update(dt);
            entity->addAge(dt);
            if (entity->isDead()) {
                if(entity == entityTracked){
                    stopTrackingAnyEntity();
                    //Focus on the general stats if the entityTracked is dead
                }
                if (entity->isChief()) {
                    resetChief(entity->getFlock());
                }
                delete entity;
                entity = nullptr;
            }
    }
    Fauna.erase(std::remove(Fauna.begin(), Fauna.end(), nullptr), Fauna.end());
}

void Environment::drawOn(sf::RenderTarget& targetWindow) {
    for (auto entity: Fauna) {
        entity->drawOn(targetWindow);
    }
    if(entityTracked != nullptr){
        entityTracked->drawCursor(targetWindow);
    }
}

void Environment::reset() {
    stopTrackingAnyEntity();        //Otherwise the program crash
    for(auto& entity: Fauna){
        delete entity;
        entity = nullptr;
    }
    Fauna.clear();
    flock.clear();
}

std::vector<LivingEntity*> Environment::getEntitiesNearby(LivingEntity* entity) const {
    // Return targets that are in the animal's sight.
    std::vector<LivingEntity*> entitiesInSight;
    for(auto& x : Fauna) {
        if((x != nullptr) and (distance(x->getPosition(), entity->getPosition()) <= getAppConfig().virus_infection_range)){
            x->infectOther(entity);
        }
        if(x && (entity->isTargetNearby(x->getPosition()))and(x != entity)){
            entitiesInSight.push_back(x);
        }
    }
    return entitiesInSight;
}

std::unordered_map<std::string, double> Environment::fetchData(std::string title) {
    if(title == s::ANIMAL_INDIVIDUAL){
        if(entityTracked != nullptr){
            return entityTracked->fetchDataForEntity();
        } else {
            std::unordered_map<std::string, double> newData = {{s::ENERGY, 0},{s::HEALTH, 0},
            {s::VIRUS, 0},{s::SCORE, 0},
            {s::IMUNAC, 0},{s::ADASCORE,0}};
            return newData;
        }
    }
    if(title == s::PLANT_INDIVIDUAL){
        if(entityTracked != nullptr){
            return entityTracked->fetchDataForEntity();
        } else {
            std::unordered_map<std::string, double> newData = {{s::ENERGY, 0}};
            return newData;
        }
    }
    if(title == s::GENERAL){
        std::unordered_map<std::string, double> newData = {{s::WOLVES, Wolf::getCount()}, {s::SHEEP, Sheep::getCount()},
        {s::PLANT, Plant::getCount()}, {s::INFECTED, Animal::getInfectionCount()}};
        return newData;
    }
}

void Environment::trackEntity(Vec2d cursorPos){
    if(Fauna.size() != 0){
        for(auto& entity : Fauna){
            if(entityTracked == nullptr){
                entityTracked = entity;
            } else {
                if(distance(entity->getPosition(), cursorPos) < distance(entityTracked->getPosition(), cursorPos)){
                    entityTracked = entity;
                }
            }
        }
        entityTracked->focusOnStatEntity();     //Call focus on stat according to the type of the entity in a polymorphic way.
    }
}

void Environment::stopTrackingAnyEntity(){
    getApp().focusOnStat(s::GENERAL);
    entityTracked = nullptr;
}

LivingEntity* Environment::getChief(int flockNb) const {
    return flock.at(flockNb);
}

void Environment::resetChief(int flockNb) {
    double goodChief(-1E9);
    LivingEntity* Chief;
    for (auto entity: Fauna) {
        if (entity->getFlock() == flockNb) {
            if ((entity->getEnergyLevel() - (abs(entity->getLongevity()/2 - entity->getAge().asSeconds()))/2) > goodChief) {
                goodChief = entity->getEnergyLevel() - (abs(entity->getLongevity()/2 - entity->getAge().asSeconds()))/2;
                Chief = entity;
            }       //is more likely to be Chief if high energy and close to half of its life
        }
    }
    flock[flockNb] = Chief;
    for (auto& entity: Fauna) {
        if (entity->getFlock() == flockNb) {
            entity->setChief(Chief);
        }
    }
}

void Environment::infectEntity(const Vec2d& cursorPos, Virus* v) {
    LivingEntity* toInfect = nullptr;
    if(Fauna.size() != 0){
        for(auto& entity: Fauna) {
            if(toInfect == nullptr) {
                toInfect = entity;
            }
            if(distance(entity->getPosition(), cursorPos) < distance(toInfect->getPosition(), cursorPos)) {
                toInfect = entity;
            }
        }
        for(auto& entity: Fauna) {
            if(entity == toInfect){
                if(entity->isImmuneSystemeVirusNullptr()){
                    if(v == nullptr){
                        v = new Virus;
                    }
                    entity->infect(v);
                }
            }
        }
    }
}

Environment::Environment()
:entityTracked(nullptr)
{}
