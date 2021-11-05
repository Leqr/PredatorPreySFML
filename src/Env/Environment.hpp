#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <Utility/Vec2d.hpp>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "LivingEntity.hpp"
#include <algorithm>
#include <unordered_map>

typedef Vec2d Target;
class Animal;
class Environment {
private:
    std::list <LivingEntity*> Fauna;
    std::unordered_map<int, LivingEntity*> flock;
    LivingEntity* entityTracked;

public:
    void addEntity (LivingEntity* entity);
    void addRandomPlant(Vec2d position);
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& targetWindow);
    void reset();
    std::vector <LivingEntity*> getEntitiesNearby(LivingEntity* entity) const ;             // Return targets that are in the animal's sight.

    /********** Data ***********/
    std::unordered_map<std::string, double> fetchData(std::string title);                   //Prepare data for the stats
    void trackEntity(Vec2d cursorPos);
    void stopTrackingAnyEntity();

    /********** Flock **********/
    LivingEntity* getChief(int flockNb) const;
    void resetChief(int flockNb);

    /******** Infection **********/
    void infectEntity(const Vec2d& cursorPos, Virus* v = nullptr);    //Call virus default constructor to randomly create a virus.

    /* An Environment can't be copied */
    Environment(const Environment&) = delete;
    Environment();
};

#endif
