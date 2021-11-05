#include "Animal.hpp"
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Constants.hpp>
#include <Random/Random.hpp>
#include <cmath>
#include <unordered_map>

int Animal::infectionCount = 0;

/*********************************************************
********************** private ****************************
*********************************************************/

double Animal::getSize() const {
    if(growing == false){
        return getRadius()*2;
    } else {
        if(age.asSeconds() < 20 ){
            if (age.asSeconds() < 15){
                if (age.asSeconds() < 10){
                    if (age.asSeconds() < 5){
                        return getRadius();
                    }
                    return getRadius() * 1.25;
                }
                return getRadius() * 1.5;
            }
            return getRadius() * 1.75;
        }
    }
}

bool Animal::isFemale() const {
    return (genome->getSex() == FEMALE);
}

void Animal::convertToGlobalCoord (Vec2d& local) const {
    sf::Transform matTransform;                         // create a transformation matrix
    matTransform.translate(position);                   // first, translate
    matTransform.rotate(getRotation() / DEG_TO_RAD);    // then rotate
    local = matTransform.transformPoint(local);         // now transform the point
}

void Animal::move(Vec2d force, sf::Time dt){
    Vec2d newSpeed;
    newSpeed = getSpeedVector() + ((force) / getMass())*dt.asSeconds();
    movingDirection = newSpeed.normalised();
    if (newSpeed.length() < getMaxSpeed()){
        speed = newSpeed.length();
    } else {
        newSpeed = newSpeed.normalised() * getMaxSpeed();
        speed = newSpeed.length();
    }
    position += newSpeed*dt.asSeconds();
}

Vec2d Animal::attractionForce() const {
    if(!isEqual((targetPosition - position).lengthSquared(), 0)){
        Vec2d toTarget = targetPosition - position;
        Vec2d speedTarget = (toTarget/(toTarget.length())*std::min(toTarget.length()/getAppConfig().animal_deceleration, getMaxSpeed()));
        Vec2d currentSpeed = getSpeedVector();
        return speedTarget - currentSpeed;
    } else {
        //if statement needed to insure that toTarget is never (0,0)
        return {0,0};
    }
}

Vec2d Animal::randomWalkForce() {
    //Create a random point in front of the animal
    Vec2d random_vec (uniform(-1.0,1.0), uniform(-1.0, 1.0));
    currentTarget += (random_vec * getRandomWalkJitter());
    currentTarget = currentTarget.normalised();
    currentTarget *= getRandomWalkRadius();
    targetPosition = currentTarget + Vec2d(getRandomWalkDistance(), 0);
    convertToGlobalCoord (targetPosition);
    Vec2d force = targetPosition - position;
    return force;
}

Vec2d Animal::collidingAttractionForce() const {
    //Create a force to slow down the animal when it's arriving on his prey.
    Vec2d fakeTargetPositionLocal = {-1,0};
    convertToGlobalCoord(fakeTargetPositionLocal);
    Vec2d toTarget = fakeTargetPositionLocal - position;
    Vec2d speedTarget = (toTarget/(toTarget.length())*(toTarget.length()/getAppConfig().animal_deceleration));
    Vec2d currentSpeed = getSpeedVector();
    return speedTarget - currentSpeed;
}

Vec2d Animal::escapeForce() const {
    Vec2d force;
    for(auto& predatorPosition : predatorPositions) {
        Vec2d toTarget = predatorPosition - position;
        force = force + ((getAppConfig().animal_escape_coeff1*toTarget)/pow(toTarget.length(), getAppConfig().animal_escape_coeff2));
    }
    return -force;
}

double Animal::getMaxSpeed() const {
    //Change the value of MAXSPEED according to the current state
    double maxSpeed;
    if(state == FOODINSIGHT){
        maxSpeed = getStandardMaxSpeed();
    }
    else if(state == MATEINSIGHT){
        maxSpeed = getStandardMaxSpeed() * 2;
    }
    else if(state == RUNNINGAWAY){
        maxSpeed = getStandardMaxSpeed() * 4;
    }
    else maxSpeed = getStandardMaxSpeed();
    if (energyLevel >= getCriticalPoint()) {
        return maxSpeed;
    } else {
        return maxSpeed/2;
    }
}

Vec2d Animal::getSpeedVector() const {
    return speed * movingDirection;
}

double Animal::getRotation() const {
    if(isEqual(speed, 0)){
        return previousMovingDirectionAngle;
    } else {
        return movingDirection.angle();
    }
}

bool Animal::isFull() const {
    return !((energyLevel < getAppConfig().animal_satiety_min) or ((state == FEEDING or state == FOODINSIGHT) and energyLevel < getAppConfig().animal_satiety_max));
}

double Animal::energyAfterMeal(LivingEntity const* target) const {
    return energyLevel + getAppConfig().animal_meal_retention * target->getEnergyLevel();
}

void Animal::doSpecificActionTo(LivingEntity* entity) {
}

void Animal::undergoSpecificActionOf(Animal* animal) {
}

void Animal::undergoSpecificActionOf(Plant* plant) {
    plant->specificAction(this);
}

LivingEntity* Animal::getMateTarget() const {
    return mateTarget;
}

void Animal::setMateTarget(LivingEntity* mate) {
    mateTarget = mate;
}

sf::Time Animal::getMatingTimeLeft() const {
    return matingTimeLeft;
}

void Animal::forceToMate() {
    forcedToMate = true;
}

bool Animal::isPregnant() const {
    return (pregnantOf > 0);
}

void Animal::setPregnancy(int childnumber) {
    pregnantOf = childnumber;
}

void Animal::setPregnancyTime(sf::Time time) {
    pregnancyTimeLeft = time;
}

int Animal::getFlock() const {
    if (flockingAnimal()) {
        return flock;
    } else {
        return -1;
    }
}

void Animal::setChief(LivingEntity* Chief) {
    myChief = Chief;
}

ImmuneSystem* Animal::getImmuneSystem(){
    return &immuneSystem;
}

bool Animal::isImmuneSystemeVirusNullptr() const {
    if(immuneSystem.getVirus() == nullptr){
        return true;
    } else {
        return false;
    }
}

void Animal::infect(Virus* v) {
    immuneSystem.setVirus(v);
}

void Animal::drawVision (sf::RenderTarget& targetWindow) const {
    //Draw the vision arc
    sf::Color color = sf::Color::Black;
    color.a = 16;
    Arc arc((getRotation()-getViewRange()/2)/DEG_TO_RAD, (getRotation()+getViewRange()/2)/DEG_TO_RAD, getViewDistance(), color, getViewDistance());
    arc.setOrigin(getViewDistance(), getViewDistance());
    arc.setPosition(position);
    targetWindow.draw(arc);
}

void Animal::drawRandomTarget(sf::RenderTarget& targetWindow){
    //Draw the RandomWalk point
    targetWindow.draw(buildCircle(targetPosition, 5, sf::Color(0,0,255)));
    //Draw the randomWalk Circle
    sf::CircleShape circle(getRandomWalkRadius());
    Vec2d circleCenter(getRandomWalkDistance(), 0);
    convertToGlobalCoord(circleCenter);
    circle.setPosition(circleCenter);
    circle.setOrigin(getRandomWalkRadius(),getRandomWalkRadius());
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Magenta);
    circle.setOutlineThickness(1);
    targetWindow.draw(circle);
}

void Animal::drawDebugInfo(sf::RenderTarget& targetWindow){
    //Draw the state of the animal on the vision arc
    Vec2d positionState = getDebugStatePosition();
    convertToGlobalCoord(positionState);
    auto textState = buildText(stateToString(state), positionState, getAppFont(), getAppConfig().default_debug_text_size, sf::Color::Black);
    textState.setRotation((getRotation()/DEG_TO_RAD)+90);
    targetWindow.draw(textState);
    //Draw the energy of the animal on the vision arc
    Vec2d positionEnergy = getDebugEnergyPosition();
    convertToGlobalCoord(positionEnergy);
    auto textEnergy = buildText(to_nice_string(energyLevel), positionEnergy, getAppFont(), getAppConfig().default_debug_text_size, sf::Color::Black);
    textEnergy.setRotation((getRotation()/DEG_TO_RAD)+90);
    targetWindow.draw(textEnergy);
    //Draw the sex of the animal on the vision arc
    Vec2d positionSex = getDebugSexPosition();
    convertToGlobalCoord(positionSex);
    auto textSex = buildText(genome->getStringSex(), positionSex, getAppFont(), getAppConfig().default_debug_text_size, sf::Color::Black);
    textSex.setRotation((getRotation()/DEG_TO_RAD)+90);
    targetWindow.draw(textSex);
    //Draw the vision arc
    sf::Color color = sf::Color::Black;
    color.a = 16;
    Arc arc((getRotation()-getViewRange()/2)/DEG_TO_RAD, (getRotation()+getViewRange()/2)/DEG_TO_RAD, getViewDistance(), color, getViewDistance());
    arc.setOrigin(getViewDistance(), getViewDistance());
    arc.setPosition(position);
    targetWindow.draw(arc);
    //Draw the obstacle circle
    targetWindow.draw(buildCircle(position, getRadius(), sf::Color(0,60,0,30)));
    //Draw the pregnancy typeTest
    if(this->isPregnant()){
        sf::CircleShape circle = buildCircle(position, getRadius(), sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::Magenta);
        circle.setOutlineThickness(3);
        targetWindow.draw(circle);
    }
    //Flock Number
    if(this->flockingAnimal()){
        Vec2d flockTextPosition = {90,100};
        convertToGlobalCoord(flockTextPosition);
        auto textFlock = buildText("Flock " + to_nice_string(getFlock()), flockTextPosition, getAppFont(), getAppConfig().default_debug_text_size, sf::Color::Black);
        textFlock.setRotation(getRotation()/DEG_TO_RAD+90);
        targetWindow.draw(textFlock);
    }
}

void Animal::drawHeart(sf::RenderTarget& targetWindow){
    auto heartSprite = buildSprite(position, 20, getAppTexture(getAppConfig().heart_texture));
    targetWindow.draw(heartSprite);
}

void Animal::drawCrown(sf::RenderTarget& targetWindow){
    Vec2d crownPosition = {30,0};
    convertToGlobalCoord(crownPosition);
    auto crownSprite = buildSprite(crownPosition, 20, getAppTexture(getAppConfig().animal_texture_leader));
    crownSprite.setRotation(getRotation()/DEG_TO_RAD+90);
    targetWindow.draw(crownSprite);
}

void Animal::drawCursor(sf::RenderTarget& targetWindow){
    Vec2d cursorPosition = {-20,-20};
    convertToGlobalCoord(cursorPosition);
    auto cursorSprite = buildSprite(cursorPosition, 20, getAppTexture(getAppConfig().entity_texture_tracked));
    targetWindow.draw(cursorSprite);
}

void Animal::drawInfected(sf::RenderTarget& targetWindow){
    Vec2d virusIconPosition = {30,-30};
    convertToGlobalCoord(virusIconPosition);
    auto virusIconSprite = buildSprite(virusIconPosition, 20, getAppTexture(getAppConfig().virus_texture_infected));
    virusIconSprite.setRotation(getRotation()/DEG_TO_RAD+90);
    targetWindow.draw(virusIconSprite);
}

void Animal::updateState(sf::Time dt) {
    //Analyze the environment to update the animal's state.
    bool isNotWandering(false);
    if (idleTimeLeft > sf::seconds(0)) {
        state = IDLE;
        isNotWandering = true;
    }
    if (!isFull()) {
        Vec2d foodPosition(getAppConfig().window_simulation_width,getAppConfig().window_simulation_height); //initialized to the biggest positive distance
        for(auto& entity : getAppEnv().getEntitiesNearby(this)) {
            //FOODINSIGHT
            if(eatable(entity)){
                if (distance(entity->getPosition(), position) < distance (foodPosition, position)) {
                    //keeps the position of the nearest food in sight
                    foodPosition = entity->getPosition();
                    state = FOODINSIGHT;
                    isNotWandering = true;                        //Is stated as FEEDING if FOODINSIGHT and is colliding.
                    if(this->isColliding(entity)){
                        //FEEDING
                        state = FEEDING;
                        foodTarget = entity;
                    }
                }
            }
        }
        if((state == FOODINSIGHT) or (state == FEEDING)) targetPosition = foodPosition;
    }
    if (feedingTimeLeft > sf::seconds(0)) {
        state = FEEDING;
        isNotWandering = true;
    }
    Vec2d matePosition(getAppConfig().simulation_world_size,getAppConfig().simulation_world_size); //initialized to the biggest positive distance
    for(auto& entity : getAppEnv().getEntitiesNearby(this)) {
        //MATEINSIGHT
        if(matable(entity) and entity->matable(this) and (!growing)){
            if (distance(entity->getPosition(), position) < distance (matePosition, position)) {
                //keeps the position of the nearest mate in sight
                matePosition = entity->getPosition();
                state = MATEINSIGHT;
                isNotWandering = true;
                //Is stated as MATING if MATEINSIGHT and is colliding.
                if(this->isColliding(entity)){
                    state = MATING;
                    mateTarget = entity;
                    mateGenome = *mateTarget->getGenome();
                    entity->setMateTarget(this);
                }
            }
        }
    }
    if ((matingTimeLeft > sf::seconds(0) or forcedToMate) and !growing) {
        state = MATING;
        isNotWandering = true;
    }
    if ((state == MATEINSIGHT) or (state == MATING)) {
        targetPosition = matePosition;
    }
    if (inLabor) {
        state = GIVINGBIRTH;
        isNotWandering = true;
        inLabor = false;
    }
    if (deliveryTimeLeft > sf::seconds(0)) {
        state = GIVINGBIRTH;
        isNotWandering = true;
    }
    if (!isNotWandering) {
        if (bernoulli(getAppConfig().animal_idle_probability) and canIdle) {
            //IDLE
            state = IDLE;
            idleTimeLeft = sf::seconds(uniform(getAppConfig().animal_idle_time_min, getAppConfig().animal_idle_time_max));
        } else {
            state = WANDERING;
        }
    }
    //RUNNINGAWAY
    for(auto& entity : getAppEnv().getEntitiesNearby(this)) {
        predatorPositions.empty();
        if(entity->eatable(this)){
            state = RUNNINGAWAY;
            predatorPositions.push_back(entity->getPosition());
        }
    }
}

State Animal::getState() const {
    return state;
}

std::unordered_map<std::string, double> Animal::fetchDataForEntity() const {
    if(immuneSystem.getVirus() != nullptr){
        std::unordered_map<std::string, double> newData = {{s::ENERGY, energyLevel},{s::HEALTH, immuneSystem.getHealth()},
        {s::VIRUS, immuneSystem.getVirus()->getInfectiveAgentQty()},{s::SCORE, immuneSystem.getScore()},
        {s::IMUNAC, immuneSystem.getActivationLevel()},{s::ADASCORE,immuneSystem.getAdaptScore()}};
        return newData;
    } else {
        std::unordered_map<std::string, double> newData = {{s::ENERGY, energyLevel},{s::HEALTH, immuneSystem.getHealth()},
        {s::VIRUS, 0.0},{s::SCORE, immuneSystem.getScore()},
        {s::IMUNAC, immuneSystem.getActivationLevel()},{s::ADASCORE,immuneSystem.getAdaptScore()}};
        return newData;
    }
}

void Animal::focusOnStatEntity() const {
    getApp().focusOnStat(s::ANIMAL_INDIVIDUAL);
}

/*********************************************************
********************** public ****************************
*********************************************************/

int Animal::getInfectionCount() {
    return infectionCount;
}

int Animal::incrementInfectionCount() {
    infectionCount++;
}

int Animal::decrementInfectionCount() {
    infectionCount--;
}

void Animal::update(sf::Time dt) {
    updateState(dt);
    immuneSystem.update(dt);
    age += dt;                                                      //Age increment
    if(age.asSeconds() > 20){
        growing = false;
    }
    Vec2d newSpeed;
    switch (state) {
    case FOODINSIGHT:
        //if food is in sight
        move(attractionForce(), dt);
        break;
    case WANDERING:
        //if no target is in sight
        canIdle = false;
        if((flockingAnimal()) and (!isChief())){
            Vec2d avoidanceForce(0,0);
            for(auto entity : getAppEnv().getEntitiesNearby(this)){
                if((flock == entity->getFlock()) and (!isEqual((position - entity->getPosition()).length(),0))){
                    if((position - entity->getPosition()).length() <= getAppConfig().animal_avoidance_threshold){
                        avoidanceForce += (getAppConfig().animal_avoidance_coeff *(position - entity->getPosition()))
                        /(position - entity->getPosition()).lengthSquared();
                    }
                }
            }
            targetPosition = myChief->getPosition();
            move((attractionForce()+randomWalkForce()+avoidanceForce)/3, dt);      //Take an average between the three forces
        } else {
            move(randomWalkForce(), dt);
        }
        break;
    case FEEDING:
        //if the entity is close enough to the target (isColliding is true)
        //Here we can't use the move() function because we need to be sure that the collidingAttractionForce() won't make the animal go backwards.
        if(!isEqual(((targetPosition - position).lengthSquared()),0)){
            newSpeed = getSpeedVector() + (collidingAttractionForce()/ getMass())*dt.asSeconds();
            if((newSpeed.dot(getSpeedVector()) < 0) or (isEqual(speed, 0))){        //To insure that the collidingAttractionForce won't make the direction change.
                speed = 0;
                movingDirection = {0,0};
            } else {
                movingDirection = newSpeed.normalised();
                speed = newSpeed.length();
                position += newSpeed*dt.asSeconds();
            }
        } else {
            speed = 0;
            movingDirection = {0,0};
        }
        if (feedingTimeLeft <= sf::seconds(0)) {
            energyLevel = energyAfterMeal(foodTarget);
            foodTarget->setEnergyLevel(targetEnergyLeftIfEats(foodTarget));
            if (energyAfterMeal(foodTarget) >= getAppConfig().animal_satiety_max) {
                feedingTimeLeft = sf::seconds(getAppConfig().animal_feed_time);         // set pause time to max on the last bite
                foodTarget->doSpecificActionTo(this);
            }
        }
        // set pause time to max
        break;
    case MATEINSIGHT:
        move(attractionForce(), dt);
        break;
    case MATING:
        if (matingTimeLeft <= sf::seconds(0)) {
            matingTimeLeft = sf::seconds(getAppConfig().animal_mating_time);
            if (!forcedToMate) {
                meet(mateTarget);
            }
        }
        forcedToMate = false;
        speed = 0;
        movingDirection = {0,0};
        break;
    case RUNNINGAWAY:
        //if a predator is in sight of the animal
        move(escapeForce(), dt);
        break;
    case GIVINGBIRTH:
        if (deliveryTimeLeft <= sf::seconds(0)){
            do {
                giveBirth(mateTarget);
                --pregnantOf;
            } while (pregnantOf > 0);

            deliveryTimeLeft = sf::seconds(getAppConfig().animal_delivery_time);
        }
        speed = 0;
        movingDirection = {0,0};
        break;
    case IDLE:
        speed = 0;
        movingDirection = {0,0};
    }
    if(!isEqual(speed, 0)){
        previousMovingDirectionAngle = movingDirection.angle();             //So the sprite always keep the same direction even if speed == 0
    }
    if (state != WANDERING) canIdle = true;
    //Create a toric environment
    setPosition();
    //Empties a little animal energy at each step (+ speed, -energy)
    energyLevel -= getAppConfig().animal_base_energy_consumption + speed * energyLossFactor() * dt.asSeconds();
    deliveryTimeLeft -= dt;
    matingTimeLeft -= dt;
    feedingTimeLeft -= dt;
    idleTimeLeft -= dt;
    if (pregnancyTimeLeft > sf::seconds(0)) {
        pregnancyTimeLeft -= dt;
        if (pregnancyTimeLeft <= sf::seconds(0)) {
            inLabor = true;
        }
    }
}

void Animal::drawOn(sf::RenderTarget& targetWindow) {
    auto animalSprite = buildSprite(position, getSize(), getAppTexture(getTexture()));
    //Needed to keep the direction of the sprite to the same value even if speed = 0;
    animalSprite.setRotation(getRotation()/DEG_TO_RAD);
    targetWindow.draw(animalSprite);
    if(isDebugOn()){
        //Draw the state and the vision arc
        drawDebugInfo(targetWindow);
        if(state == WANDERING){
            //Draw randomTarget only if nothing is in sight and animal.
            drawRandomTarget(targetWindow);
        }
        if((state == MATING) or (state == MATEINSIGHT)){
            drawHeart(targetWindow);
        }
    }
    if(isChief()){
        drawCrown(targetWindow);
    }
    if((immuneSystem.getVirus() != nullptr) and (immuneSystem.getVirus()->getInfectiveAgentQty() >= getAppConfig().virus_min_quantity_for_infection)) {
        drawInfected(targetWindow);
    }
}

void Animal::setRotation(double angle) {
    movingDirection = Vec2d(cos(angle), sin(angle));
}

void Animal::setTargetPosition(const Vec2d& position) {
    targetPosition = position;
}

bool Animal::isTargetNearby(Vec2d targetPosition) const{
    //Return true if a target is in sight
    Vec2d d = targetPosition - position;
    if ((isEqual(d.lengthSquared(), 0)) or
    ((d.lengthSquared() <= getViewDistance()*getViewDistance()) and
    (movingDirection.dot(d.normalised()) >= cos((getViewRange() + 0.01)/2.0))))
    {
        return true;
    }
    else return false;
}

std::string Animal::stateToString(State stateToConvert) const {
    switch (stateToConvert) {
    case FOODINSIGHT:
        return "FoodInSight";
        break;

    case FEEDING:
        return "Feeding";
        break;

    case RUNNINGAWAY:
        return "RunningAway";
        break;

    case MATEINSIGHT:
        return "MateInSight";
        break;

    case MATING:
        return "Mating";
        break;

    case GIVINGBIRTH:
        return "GivingBirth";
        break;

    case WANDERING:
        return "Wandering";
        break;

    case IDLE:
        return "Idle";
        break;
    }
}

void Animal::setImmuneGenes(std::array<double, 10> const& other) {
    genome->setImmuneGenes(other);
}

bool Animal::isDead() const {
    return ((energyLevel < getAppConfig().animal_min_energy) or (tooOld()) or immuneSystem.getHealth() < 0);
}

Animal::Animal (const Vec2d& position, double energy, int flock, double speed, Genome* motherGenome, Genome* fatherGenome, bool growing, sf::Time age)
:LivingEntity (position, energy, motherGenome, fatherGenome), movingDirection(1,0), speed(speed), targetPosition(0,0), previousMovingDirectionAngle(uniform(-3,3)),
pregnantOf(0), pregnancyTimeLeft(sf::seconds(0)), deliveryTimeLeft(sf::Time::Zero), matingTimeLeft(sf::Time::Zero),
feedingTimeLeft(sf::Time::Zero), inLabor(false), age(age), growing(growing), forcedToMate(false), flock(flock), myChief(this), idleTimeLeft(sf::Time::Zero),
canIdle(true), immuneSystem(this)
{}

Animal::~Animal()
{}
