#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#include <Utility/Vec2d.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include "Config.hpp"
#include <Random/Random.hpp>
#include "LivingEntity.hpp"
#include "Genetics/ImmuneSystem.hpp"
#include "Plant.hpp"

enum State {
    FOODINSIGHT,
    FEEDING,
    RUNNINGAWAY,
    MATEINSIGHT,
    MATING,
    GIVINGBIRTH,
    WANDERING,
    IDLE,
};
class Animal : public LivingEntity {
protected:
    sf::Time age;
    double getSize() const;                                                    //Return a size in function of the age of the animal.
    virtual double getMass() const = 0;
    bool isFemale() const;

    /************* Dynamics ***************/
    void convertToGlobalCoord (Vec2d& local) const;
    void move(Vec2d force, sf::Time dt);
    /*Force*/
    Vec2d attractionForce() const;
    Vec2d randomWalkForce();
    Vec2d collidingAttractionForce() const;                                     //Slowing the animal down.
    Vec2d escapeForce() const;                                                  //If a predator is in sight of a prey.
    /*Speed*/
    double speed;
    double getMaxSpeed() const;                                                 //Change the value of returned value of getStandardMaxSpeed in function of the state
    virtual double getStandardMaxSpeed() const = 0;                             //Get the max speed in function of the type of the animal.
    Vec2d getSpeedVector() const;
    /*Direction*/
    Vec2d movingDirection;
    double previousMovingDirectionAngle;
    double getRotation() const;
    /*Target*/
    Vec2d targetPosition;
    Vec2d currentTarget;
    std::vector<Vec2d> predatorPositions;
    /*Sight*/
    virtual double getViewRange() const = 0;
    virtual double getViewDistance() const = 0;
    /*Random Walk*/
    virtual double getRandomWalkRadius() const = 0;
    virtual double getRandomWalkDistance() const = 0;
    virtual double getRandomWalkJitter() const = 0;

    /************ Idle *****************/
    sf::Time idleTimeLeft;
    bool canIdle;

    /*********** Feeding *************/
    LivingEntity* foodTarget;
    sf::Time feedingTimeLeft;
    bool isFull() const;
    double energyAfterMeal(LivingEntity const* target) const;
    virtual double targetEnergyLeftIfEats(LivingEntity const* target) const = 0;
    void doSpecificActionTo(LivingEntity* entity);
    void undergoSpecificActionOf(Animal* animal);
    void undergoSpecificActionOf(Plant* plant);

    /************ Mating, Pregancy and growing ***********/
    /*Growing*/
    bool growing;
    /*Mating*/
    LivingEntity* mateTarget;
    LivingEntity* getMateTarget() const;
    void setMateTarget(LivingEntity* mate);
    sf::Time matingTimeLeft;
    sf::Time getMatingTimeLeft() const;
    bool forcedToMate;
    void forceToMate();
    Genome mateGenome;
    /*Pregnancy*/
    int pregnantOf;
    bool isPregnant() const;
    void setPregnancy(int childnumber);
    void setPregnancyTime(sf::Time time);
    sf::Time pregnancyTimeLeft;
    sf::Time deliveryTimeLeft;
    bool inLabor;
    virtual void giveBirth(LivingEntity* mate) = 0;

    /*************** Flock and Chief ***************/
    int flock;
    int getFlock() const override;
    LivingEntity* myChief;
    void setChief(LivingEntity* Chief) override;
    virtual bool isChief() const = 0;
    virtual bool flockingAnimal() const = 0;

    /************ Immune System and infection ************/
    static int infectionCount;
    ImmuneSystem immuneSystem;

    /****** Energy Management ******/
    virtual double getCriticalPoint() const = 0;
    virtual double energyLossFactor() const = 0;

    /*********** Drawing  *************/
    virtual std::string getTexture() const = 0;
    void drawVision (sf::RenderTarget& targetWindow) const;
    void drawRandomTarget(sf::RenderTarget& targetWindow) override;
    void drawDebugInfo(sf::RenderTarget& targetWindow) override;                //Draw info in debug mode
    void drawHeart(sf::RenderTarget& targetWindow);
    void drawCrown(sf::RenderTarget& targetWindow);
    void drawCursor(sf::RenderTarget& targetWindow) override;
    void drawInfected(sf::RenderTarget& targetWindow);
    virtual Vec2d getDebugStatePosition() const = 0;                            //Return the position needed to draw the state.
    virtual Vec2d getDebugSexPosition() const = 0;                              //Return the position needed to draw the sex.
    virtual Vec2d getDebugEnergyPosition() const = 0;                           //Return the position needed to draw the energy

    /*********** State Management *************/
    void updateState(sf::Time dt);
    State state;
    State getState() const;

    /************** Stats ******************/
    std::unordered_map<std::string, double> fetchDataForEntity() const override;
    void focusOnStatEntity() const override;

public:

    /************** Count ******************/
    static int getInfectionCount();
    static int incrementInfectionCount();
    static int decrementInfectionCount();

    /************ Immune System and infection ************/
    ImmuneSystem* getImmuneSystem();
    bool isImmuneSystemeVirusNullptr() const override;
    void infect(Virus* v) override;

    /*********** OnRun *************/
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& targetWindow);

    void setRotation(double angle);
    void setTargetPosition(const Vec2d& position);
    bool isTargetNearby(Vec2d targetPosition) const;
    std::string stateToString(State stateToConvert) const;
    void setImmuneGenes(std::array<double, 10> const& other);
    bool isDead() const override;
    Animal (const Vec2d& position, double energy, int flock, double speed, Genome* motherGenome, Genome* fatherGenome,
        bool growing, sf::Time age = sf::seconds(0));
    virtual ~Animal();
};

#endif
