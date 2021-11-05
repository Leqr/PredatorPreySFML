#include "Plant.hpp"
#include <unordered_map>
#include <Random/Random.hpp>
int Plant::count = 0;

/*********************************************************
********************** private ***************************
*********************************************************/

double Plant::getRadius() const {
    return energyLevel/2;
}

int Plant::getLongevity() const {
    return 1E9;
}

bool Plant::isDead() const {
    return ((energyLevel < getAppConfig().animal_min_energy) or (tooOld()));
}

void Plant::divisionManagement(sf::Time dt){
    if(getAppEnv().getEntitiesNearby(this).size() < getAppConfig().plant_growth_max_entities_nearby){
        //Division of the plant
        if((energyLevel >= getAppConfig().plant_max_energy) and (divisionNumber < 1)){
            ++divisionNumber;
            Vec2d addPosition = {uniform(100.0,-100.0), uniform(100.0,-100.0)};
            addSamePlant(position + addPosition);
        }
    }
}

void Plant::drawRandomTarget(sf::RenderTarget& targetWindow) {
}

void Plant::drawDebugInfo(sf::RenderTarget& targetWindow){
}

void Plant::drawCursor(sf::RenderTarget& targetWindow){
    auto cursorSprite = buildSprite(position, 20, getAppTexture(getAppConfig().entity_texture_tracked));
    targetWindow.draw(cursorSprite);
}

bool Plant::eatable(LivingEntity const* entity) const {
    return entity->eatableBy(this);
}

bool Plant::eatableBy(Plant const* plant) const {
    return false;
}

bool Plant::eatableBy(Wolf const* wolf) const {
    return false;
}

bool Plant::eatableBy(Sheep const* sheep) const {
    return true;
}

bool Plant::matable(LivingEntity const* other) const {
    return other->canMate(this);
}

bool Plant::canMate(Wolf const* mate) const {
    return false;
}

bool Plant::canMate(Sheep const* mate) const {
    return false;
}

bool Plant::canMate(Plant const* mate) const {
    return false;
}

void Plant::meet(LivingEntity* mate) {
    mate->isApproachedBy(this);
}

void Plant::isApproachedBy(Wolf* mate) {
    std::cerr << "Error: Wolf and Plant trying to mate..." << std::endl;
}

void Plant::isApproachedBy(Sheep* mate) {
    std::cerr << "Error: Sheep and Plant trying to mate" << std::endl;
}

void Plant::isApproachedBy(Plant* mate) {
    std::cerr << "Error: Plant trying to mate" << std::endl;
}

void Plant::infect(Virus* v) {
}

void Plant::infectOther(LivingEntity* entity) {
}

void Plant::infectedBy(Wolf* wolf) {
}

void Plant::infectedBy(Sheep* sheep) {
}

void Plant::infectedBy(Plant* plant) {
}

bool Plant::isImmuneSystemeVirusNullptr() const {
    return true;
}

void Plant::doSpecificActionTo(LivingEntity* entity) {
    entity->undergoSpecificActionOf(this);
}

void Plant::undergoSpecificActionOf(Animal* animal) {
}

void Plant::undergoSpecificActionOf(Plant* plant) {
}

void Plant::setMateTarget(LivingEntity* mate) {
}

int Plant::getFlock() const {
    return -1;
}

bool Plant::isChief() const {
    return false;
}

void Plant::setChief(LivingEntity* Chief) {
}

bool Plant::isTargetNearby(Vec2d targetPosition) const {
    if(distance(targetPosition, position) <= getAppConfig().plant_growth_surrounding_radius){
        return true;
    } else {
        return false;
    }
}

std::unordered_map<std::string, double> Plant::fetchDataForEntity() const {
    std::unordered_map<std::string, double> newData = {{s::ENERGY, energyLevel}};
    return newData;
}

void Plant::focusOnStatEntity() const {
    getApp().focusOnStat(s::PLANT_INDIVIDUAL);
}

/*********************************************************
********************** public ****************************
*********************************************************/

int Plant::getCount(){
    return count;
}

void Plant::update(sf::Time dt){
    //If the plant is small enough it will grow every time dt.
    if(energyLevel < getAppConfig().plant_max_energy){
        energyLevel += getAppConfig().plant_growth_factor * dt.asSeconds();
    }
    if(getAppConfig().plant_reproduction){
        divisionManagement(dt);
    }
}

void Plant::drawOn(sf::RenderTarget& targetWindow){
    //Create the sprite
    sf::Sprite plantSprite = buildSprite(position, energyLevel, getAppTexture(getTexture()));
    //Check if the object is is the window, otherwise modify it's position.
    setPosition();
    //Draw the plant on the window
    targetWindow.draw(plantSprite);
}

Plant::Plant(Vec2d position, double energyLevel)
:LivingEntity(position, energyLevel)
{
    count++;
}

Plant::~Plant()
{
    count--;
}
