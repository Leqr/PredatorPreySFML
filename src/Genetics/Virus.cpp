#include "Virus.hpp"
#include <Random/Random.hpp>
#include "Application.hpp"
#include "../Env/Animal.hpp"

double Virus::getInfectiveAgentQty() const {
    return infectiveAgentQty;
}

double Virus::getVirulenceProfile(size_t i) const {
    return virulenceProfile[i];
}

void Virus::setInfectiveAgentQty(double other) {
    infectiveAgentQty = other;
}

bool Virus::isDead() const {
    return getInfectiveAgentQty() <= getAppConfig().virus_initial_quantity;
}

void Virus::update(sf::Time dt) {
    infectiveAgentQty *= (1 + growthRate * (1 - infectiveAgentQty / getAppConfig().virus_max) * dt.asSeconds());
}

Virus::Virus()
:infectiveAgentQty(getAppConfig().virus_initial_quantity), growthRate(getAppConfig().virus_growth_rate)
{
    Animal::incrementInfectionCount();
    for(auto& x: virulenceProfile) {
        x = 0;
        if(bernoulli(getAppConfig().virus_sparsity)) {
            x = uniform(-5.0, 5.0);
        }
    }
}

Virus::Virus(Virus const& virus)
:virulenceProfile(virus.virulenceProfile), infectiveAgentQty(getAppConfig().virus_initial_quantity), growthRate(virus.growthRate)
{
    Animal::incrementInfectionCount();
}

Virus::Virus(std::array<double, 10> const& otherVirPro)
 : virulenceProfile(otherVirPro), infectiveAgentQty(getAppConfig().virus_initial_quantity), growthRate(getAppConfig().virus_growth_rate)
{
    Animal::incrementInfectionCount();
}

Virus::~Virus(){
    Animal::decrementInfectionCount();
}
