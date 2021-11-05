#ifndef WOLF_HPP
#define WOLF_HPP
#include "Animal.hpp"

class Wolf : public Animal {
private:
    static int count;

    int getLongevity() const override;
    double getMass() const override;
    double getRadius() const override;
    double getCriticalPoint() const override;
    double energyLossFactor() const override;
    bool tooOld() const override;
    double targetEnergyLeftIfEats(LivingEntity const* target) const override;
    void giveBirth(LivingEntity* mate) override;

    /******** Dynamics ********/
    double getStandardMaxSpeed() const override;
    double getViewRange() const override;
    double getViewDistance() const override;
    double getRandomWalkRadius() const override;
    double getRandomWalkJitter() const override;
    double getRandomWalkDistance() const override;

    /********** Draw *********/
    Vec2d getDebugStatePosition() const override;
    Vec2d getDebugSexPosition() const override;
    Vec2d getDebugEnergyPosition() const override;
    std::string getTexture() const override;

    /********* Double dispatch & related *********/
    bool eatable(LivingEntity const* entity) const override;
    bool eatableBy(Wolf  const* wolf) const override;
    bool eatableBy(Sheep const* sheep) const override;
    bool eatableBy(Plant const* Plant) const override;
    bool matable(LivingEntity const* other) const override;
    bool canMate(Wolf const* wolf) const override;
    bool canMate(Sheep const* sheep) const override;
    bool canMate(Plant const* plant) const override;
    void meet(LivingEntity* mate) override;
    void isApproachedBy(Wolf* mate) override;
    void isApproachedBy(Sheep* mate) override;
    void isApproachedBy(Plant* mate) override;
    void infectOther(LivingEntity* entity) override;
    void infectedBy(Wolf* wolf) override;
    void infectedBy(Sheep* sheep) override;
    void infectedBy(Plant* plant) override;

    /********* Flock *********/
    bool flockingAnimal() const override;
    bool isChief() const;

public:
    static int getCount();

    Wolf (const Vec2d& position, int flock = -1, double speed = 0, Genome* motherGenome = nullptr, Genome* fatherGenome = nullptr, bool growing = false);
    ~Wolf() override;
};

#endif
