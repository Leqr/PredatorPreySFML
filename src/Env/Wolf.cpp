#include "Wolf.hpp"
#include "Application.hpp"
int Wolf::count = 0;

/*********************************************************
********************** private ****************************
*********************************************************/

int Wolf::getLongevity() const {
    return getAppConfig().wolf_longevity.asSeconds();
}

double Wolf::getMass() const {
    return getAppConfig().wolf_mass;
}

double Wolf::getRadius() const {
    return (getAppConfig().wolf_adult_size / 2.0);
}

double Wolf::getCriticalPoint() const {
    return getAppConfig().wolf_energy_initial/5;
}

double Wolf::energyLossFactor() const {
    return getAppConfig().wolf_energy_loss_factor;
}

bool Wolf::tooOld() const {
    return (age > getAppConfig().wolf_longevity);
}

double Wolf::targetEnergyLeftIfEats(LivingEntity const* target) const {
    return 0;
}

void Wolf::giveBirth(LivingEntity* mate) {
    Genome* mateGenomeptr(&mateGenome);
    getAppEnv().addEntity(new Wolf(position, flock, 0, genome, mateGenomeptr, true));
    mateTarget = nullptr;
}

double Wolf::getStandardMaxSpeed() const {
    return getAppConfig().wolf_max_speed;
}

double Wolf::getViewRange() const {
    return getAppConfig().wolf_view_range;
}

double Wolf::getViewDistance() const {
    return getAppConfig().wolf_view_distance;
}

double Wolf::getRandomWalkRadius() const {
    return getAppConfig().wolf_random_walk_radius;
}

double Wolf::getRandomWalkJitter() const {
    return getAppConfig().wolf_random_walk_jitter;
}

double Wolf::getRandomWalkDistance() const {
    return getAppConfig().wolf_random_walk_distance;
}

Vec2d Wolf::getDebugStatePosition() const {
    return {getAppConfig().wolf_debug_state_position_y, getAppConfig().wolf_debug_state_position_x};
}

Vec2d Wolf::getDebugSexPosition() const {
    return {getAppConfig().wolf_debug_sex_position_y, getAppConfig().wolf_debug_sex_position_x};
}

Vec2d Wolf::getDebugEnergyPosition() const{
    return {getAppConfig().wolf_debug_energy_position_y, getAppConfig().wolf_debug_energy_position_x};
}

std::string Wolf::getTexture() const {
    return getAppConfig().wolf_texture;
}

bool Wolf::eatable(LivingEntity const* entity) const {
    return entity->eatableBy(this);
}

bool Wolf::eatableBy(Plant const* plant) const {
    return false;
}

bool Wolf::eatableBy(Sheep const* sheep) const {
    return false;
}

bool Wolf::eatableBy(Wolf const* wolf) const {
    return false;
}

bool Wolf::matable(LivingEntity const* other) const {
    return other->canMate(this);
}

bool Wolf::canMate(Wolf const* mate) const {
    if (!growing) {
        if (isFemale() and !mate->isFemale() and mate->getEnergyLevel() >= getAppConfig().wolf_energy_min_mating_male and
        !(mate->getState() == MATING and !(mate->getMateTarget() == this))) {
            return true;
        }
        if (!isFemale() and mate->isFemale() and mate->getEnergyLevel() >= getAppConfig().wolf_energy_min_mating_female and
        !(mate->isPregnant() or mate->getState() == GIVINGBIRTH) and !(mate->getState() == MATING and !(mate->getMateTarget() == this))) {
            return true;
        }
    }
    return false;
}

bool Wolf::canMate(Sheep const* mate) const {
    return false;
}

bool Wolf::canMate(Plant const* mate) const {
    return false;
}

void Wolf::meet(LivingEntity* mate)  {
    mate->isApproachedBy(this);
}

void Wolf::isApproachedBy(Wolf* mate)  {
    if (matable(mate) and mate->matable(this)) {
        forceToMate();
        int cubNumber (uniform(getAppConfig().wolf_reproduction_min_children, getAppConfig().wolf_reproduction_max_children));
        if (isFemale()) {
            pregnantOf = cubNumber;
            energyLevel -= cubNumber * getAppConfig().wolf_energy_loss_female_per_child;
            pregnancyTimeLeft = sf::seconds(getAppConfig().sheep_reproduction_gestation_time + getAppConfig().animal_mating_time);
            mate->setEnergyLevel(getEnergyLevel() - getAppConfig().wolf_energy_loss_mating_male);
        } else {
            mate->setPregnancy(cubNumber);
            mate->setEnergyLevel(getEnergyLevel() - cubNumber * getAppConfig().wolf_energy_loss_female_per_child);
            mate->setPregnancyTime (sf::seconds(getAppConfig().wolf_reproduction_gestation_time + getAppConfig().animal_mating_time));
            energyLevel-= getAppConfig().wolf_energy_loss_mating_male;
        }
    }
}

void Wolf::isApproachedBy(Sheep* mate)  {
    std::cerr << "Error: Sheep and Wolf trying to mate" << std::endl;
}

void Wolf::isApproachedBy(Plant* mate)  {
    std::cerr << "Error: Plant and Wolf trying to mate" << std::endl;
}

void Wolf::infectOther(LivingEntity* entity) {
    entity->infectedBy(this);
}

void Wolf::infectedBy(Wolf* wolf) {
    if (getPosition() != wolf->getPosition() and (wolf->getImmuneSystem())->getVirus() != nullptr and
    ((wolf->getImmuneSystem())->getVirus())->getInfectiveAgentQty() > getAppConfig().virus_min_quantity_for_infection) {
        if (bernoulli(getAppConfig().virus_infection_probability) and getImmuneSystem()->getVirus() == nullptr) {
            immuneSystem.setVirus(new Virus(*(wolf->getImmuneSystem())->getVirus()));
        }
    }
}

void Wolf::infectedBy(Sheep* sheep) {
}

void Wolf::infectedBy(Plant* plant) {
}

bool Wolf::flockingAnimal() const {
    return false;
}

bool Wolf::isChief() const {
    return false;
}

/*********************************************************
********************** public ****************************
*********************************************************/

int Wolf::getCount(){
    return count;
}

Wolf::Wolf (const Vec2d& position, int flock, double speed, Genome* motherGenome, Genome* fatherGenome, bool growing)
: Animal (position, getAppConfig().wolf_energy_initial, -1, speed, motherGenome, fatherGenome, growing)
{
    count++;
}

Wolf::~Wolf()
{
    count--;
}
