#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include "Obstacle.hpp"
#include "Genetics/Genome.hpp"
#include "Genetics/Virus.hpp"

class Wolf;
class Sheep;
class Plant;
class Animal;
class LivingEntity : public Obstacle {
protected:
    Vec2d position;
    double energyLevel;
    sf::Time age;
    Genome* genome;

public:
    Vec2d getCenter() const override;
    Vec2d getPosition() const;
    double getEnergyLevel() const;
    Genome* getGenome() const;
    void setPosition();
    void setEnergyLevel(double energy);
    sf::Time getAge() const;
    void addAge(sf::Time dt);
    virtual bool isDead() const = 0;                                    //dead if energy is too low or if too old or health too low
    virtual bool tooOld() const;                                        //specific for wolves and sheep, infinite otherwise
    virtual int getLongevity() const = 0;
    virtual bool isTargetNearby(Vec2d targetPosition) const = 0;

    /*********** OnRun *************/
    virtual void update(sf::Time dt) = 0;

    /******* Draw *******/
    virtual void drawOn(sf::RenderTarget& targetWindow) = 0;
    virtual void drawRandomTarget(sf::RenderTarget& targetWindow) = 0;
    virtual void drawDebugInfo(sf::RenderTarget& targetWindow) = 0;
    virtual void drawCursor(sf::RenderTarget& targetWindow) = 0;

    /******* Double Dispatch & related *********/
    virtual bool eatable(LivingEntity const* entity) const = 0;
    virtual bool eatableBy(Wolf  const* wolf) const = 0;
    virtual bool eatableBy(Sheep const* sheep) const = 0;
    virtual bool eatableBy(Plant const* plant) const = 0;
    virtual bool matable(LivingEntity const* other) const = 0;
    virtual bool canMate(Wolf const* wolf) const = 0;
    virtual bool canMate(Sheep const* sheep) const = 0;
    virtual bool canMate(Plant const* plant) const = 0;
    virtual void setMateTarget(LivingEntity* mate) = 0;
    virtual void meet(LivingEntity* mate) = 0;
    virtual void infect(Virus* v) = 0;
    virtual void infectOther(LivingEntity* entity) = 0;
    virtual void infectedBy(Wolf* wolf) = 0;
    virtual void infectedBy(Sheep* sheep) = 0;
    virtual void infectedBy(Plant* plant) = 0;
    virtual bool isImmuneSystemeVirusNullptr() const = 0;
    virtual void isApproachedBy(Wolf* mate) = 0;
    virtual void isApproachedBy(Sheep* mate) = 0;
    virtual void isApproachedBy(Plant* mate) = 0;
    virtual void doSpecificActionTo(LivingEntity* entity) = 0;
    virtual void undergoSpecificActionOf(Animal* animal) = 0;
    virtual void undergoSpecificActionOf(Plant* plant) = 0;

    /*************** Flock and Chief ***************/
    virtual int getFlock() const = 0;
    virtual bool isChief() const = 0;
    virtual void setChief(LivingEntity* Chief) = 0;

    /************** Stats ******************/
    virtual std::unordered_map<std::string, double> fetchDataForEntity() const = 0;
    virtual void focusOnStatEntity() const = 0;

    /************ Constructor, Destructor and operator *************/
    bool operator==(const LivingEntity& livingentity);              //Make a comparison of two entities checking their position, age and energyLevel

    LivingEntity(const Vec2d& position, double energy, Genome* motherGenome = nullptr, Genome* fatherGenome = nullptr);
    virtual ~LivingEntity();
};

#endif
