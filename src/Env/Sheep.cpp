#include "Sheep.hpp"
#include "Application.hpp"
int Sheep::count = 0;

/*********************************************************
********************** private ****************************
*********************************************************/

int Sheep::getLongevity() const {
    return getAppConfig().sheep_longevity.asSeconds();
}

double Sheep::getMass() const {
    return getAppConfig().sheep_mass;
}

double Sheep::getRadius() const {
    return (getAppConfig().sheep_adult_size / 2.0);
}

double Sheep::getCriticalPoint() const {
    return getAppConfig().sheep_energy_initial/5;
}

double Sheep::energyLossFactor() const {
    return getAppConfig().sheep_energy_loss_factor;
}

bool Sheep::tooOld() const {
    return (age > getAppConfig().sheep_longevity);
}

double Sheep::targetEnergyLeftIfEats(LivingEntity const* target) const {
    return target->getEnergyLevel() - getAppConfig().sheep_energy_bite;
}

void Sheep::giveBirth(LivingEntity* mate) {
    Genome* mateGenomeptr(&mateGenome);
    getAppEnv().addEntity(new Sheep(position, flock, 0, genome, mateGenomeptr, true));
    mateTarget = nullptr;
}

double Sheep::getStandardMaxSpeed() const {
    return getAppConfig().sheep_max_speed;
}

double Sheep::getViewRange() const {
    return getAppConfig().sheep_view_range;
}

double Sheep::getViewDistance() const {
    return getAppConfig().sheep_view_distance;
}

double Sheep::getRandomWalkRadius() const {
    return getAppConfig().sheep_random_walk_radius;
}

double Sheep::getRandomWalkDistance() const {
    return getAppConfig().sheep_random_walk_distance;
}

double Sheep::getRandomWalkJitter() const {
    return getAppConfig().sheep_random_walk_jitter;
}

Vec2d Sheep::getDebugStatePosition() const {
    return {getAppConfig().sheep_debug_state_position_y, getAppConfig().sheep_debug_state_position_x};
}

Vec2d Sheep::getDebugSexPosition() const {
    return {getAppConfig().sheep_debug_sex_position_y, getAppConfig().sheep_debug_sex_position_x};
}

Vec2d Sheep::getDebugEnergyPosition() const{
    return {getAppConfig().sheep_debug_energy_position_y, getAppConfig().sheep_debug_energy_position_x};
}

std::string Sheep::getTexture() const {
    if (genome->getColorPhenotype() == ColorAllele::BLACK) {
        return getAppConfig().sheep_texture_black;
    } else {
        return getAppConfig().sheep_texture_white;
    }
}

bool Sheep::eatable(LivingEntity const* entity) const {
    return entity->eatableBy(this);
}

bool Sheep::eatableBy(Plant const* plant) const {
    return false;
}

bool Sheep::eatableBy(Wolf  const* wolf) const {
    return true;
}

bool Sheep::eatableBy(Sheep const* sheep) const {
    return false;
}

bool Sheep::matable(LivingEntity const* other) const {
    return other->canMate(this);
}

bool Sheep::canMate(Wolf const* wolf) const {
    return false;
}

bool Sheep::canMate(Sheep const* mate) const {
    if (!growing) {
        if (isFemale() and !mate->isFemale() and mate->getEnergyLevel() >= getAppConfig().sheep_energy_min_mating_male and
        !(mate->getState() == MATING and !(mate->getMateTarget() == this))) {
            return true;
        }
        if (!isFemale() and mate->isFemale() and mate->getEnergyLevel() >= getAppConfig().sheep_energy_min_mating_female and
        !(mate->   isPregnant() or mate->getState() == GIVINGBIRTH) and !(mate->getState() == MATING and !(mate->getMateTarget() == this))) {
            return true;
        }
    }
    return false;
}

bool Sheep::canMate(Plant const* plant) const {
    return false;
}

void Sheep::meet(LivingEntity* mate) {
    mate->isApproachedBy(this);
}

void Sheep::isApproachedBy(Sheep* mate) {
    if (matable(mate) and mate->matable(this)) {
        forceToMate();
        int lambNumber (uniform(getAppConfig().sheep_reproduction_min_children, getAppConfig().sheep_reproduction_max_children));
        if (isFemale()) {
            pregnantOf = lambNumber;
            energyLevel -= lambNumber * getAppConfig().sheep_energy_loss_female_per_child;
            pregnancyTimeLeft = sf::seconds(getAppConfig().sheep_reproduction_gestation_time + getAppConfig().animal_mating_time);
            mate->setEnergyLevel(getEnergyLevel() - getAppConfig().sheep_energy_loss_mating_male);
        } else {
            mate->setPregnancy(lambNumber);
            mate->setEnergyLevel(getEnergyLevel() - lambNumber * getAppConfig().sheep_energy_loss_female_per_child);
            mate->setPregnancyTime (sf::seconds(getAppConfig().sheep_reproduction_gestation_time + getAppConfig().animal_mating_time));
            energyLevel -= getAppConfig().sheep_energy_loss_mating_male;
        }
    }
}

void Sheep::isApproachedBy(Plant* mate) {
    std::cerr << "Error: Sheep and Plant trying to mate" << std::endl;
}

void Sheep::isApproachedBy(Wolf* mate) {
    std::cerr << "Error: Sheep and Wolf trying to mate" << std::endl;
}

void Sheep::infectOther(LivingEntity* entity) {
    entity->infectedBy(this);
}

void Sheep::infectedBy(Wolf* wolf) {
}

void Sheep::infectedBy(Sheep* sheep) {
    if (getPosition() != sheep->getPosition() and (sheep->getImmuneSystem())->getVirus() != nullptr
    and ((sheep->getImmuneSystem())->getVirus())->getInfectiveAgentQty() > getAppConfig().virus_min_quantity_for_infection) {
        if (bernoulli(getAppConfig().virus_infection_probability) and getImmuneSystem()->getVirus() == nullptr) {
            immuneSystem.setVirus(new Virus(*(sheep->getImmuneSystem())->getVirus()));
        }
    }
}

void Sheep::infectedBy(Plant* plant) {

}

bool Sheep::flockingAnimal() const {
    return true;
}

bool Sheep::isChief() const {
    return getAppEnv().getChief(flock) == this;
}

/*********************************************************
********************** public ****************************
*********************************************************/

int Sheep::getCount(){
    return count;
}

Sheep::Sheep (const Vec2d& position, int flock, double speed, Genome* motherGenome, Genome* fatherGenome, bool growing)
: Animal (position, getAppConfig().sheep_energy_initial, flock, speed, motherGenome, fatherGenome, growing)
{
    count++;
}

Sheep::~Sheep()
{
    count--;
}
