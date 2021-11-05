#ifndef PLANT_HPP
#define PLANT_HPP
#include <Utility/Utility.hpp>
#include <iostream>
#include <Utility/Constants.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include "LivingEntity.hpp"
#include "Application.hpp"

class Plant : public LivingEntity {
private:
    static int count;

    int divisionNumber = 0;
    double getRadius() const override;
    int getLongevity() const override;
    bool isDead() const override;
    void divisionManagement(sf::Time dt);          //Make the plant multiply by themselves. Every plant can divide once in it's lifetime to avoid exponential growth rate.

    /********** Draw *********/
    void drawRandomTarget(sf::RenderTarget& targetWindow) override;             //Draw nothing
    void drawDebugInfo(sf::RenderTarget& targetWindow) override;                //Draw nothing
    void drawCursor(sf::RenderTarget& targetWindow) override;

    /********* Double dispatch & related *********/
    bool eatable(LivingEntity const* entity) const override;
    bool eatableBy(Wolf  const* wolf) const override;
    bool eatableBy(Sheep const* sheep) const override;
    bool eatableBy(Plant const* plant) const override;
    bool matable(LivingEntity const* other) const override;
    bool canMate(Wolf const* mate) const override;
    bool canMate(Sheep const* mate) const override;
    bool canMate(Plant const* mate) const override;
    void meet(LivingEntity* mate) override;
    void isApproachedBy(Wolf* mate) override;
    void isApproachedBy(Sheep* mate) override;
    void isApproachedBy(Plant* mate) override;
    void infect(Virus* v) override;
    void infectOther(LivingEntity* entity) override;
    void infectedBy(Wolf* wolf) override;
    void infectedBy(Sheep* sheep) override;
    void infectedBy(Plant* plant) override;
    bool isImmuneSystemeVirusNullptr() const override;
    void doSpecificActionTo(LivingEntity* entity) override;
    virtual void undergoSpecificActionOf(Animal* animal) override;
    virtual void undergoSpecificActionOf(Plant* plant) override;


    /********** Useful for polymorphism ***********/
    void setMateTarget(LivingEntity* mate) override;
    int getFlock() const override;
    bool isChief() const override;
    void setChief(LivingEntity* Chief) override;
    bool isTargetNearby(Vec2d targetPosition) const override;       //Return always false
    virtual void addSamePlant(Vec2d position, double energyLevel = getAppConfig().plant_initial_energy) = 0;
    virtual std::string getTexture() const = 0;

    /********** Stats **********/
    std::unordered_map<std::string, double> fetchDataForEntity() const override;
    void focusOnStatEntity() const override;

public:
    static int getCount();

    /*********** OnRun ***********/
    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& targetWindow) override;

    virtual void specificAction(Animal* animal) = 0;

    Plant(Vec2d position, double energyLevel);
    ~Plant() override;
};

#endif
