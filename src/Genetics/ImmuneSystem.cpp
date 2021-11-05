#include "Virus.hpp"
#include <Random/Random.hpp>
#include "Application.hpp"
#include "Env/Animal.hpp"

void ImmuneSystem::update(sf::Time dt) {
    initScores();
    if (virus != nullptr) {
        virus->update(dt);
        health -= getAppConfig().immune_health_penalty * virus->getInfectiveAgentQty() * dt.asSeconds();
        activationLevel *= (1 + dt.asSeconds() * (0.5 * (1 - (activationLevel * activationLevel) / 16)));
        for (size_t i(0); i < adaptativeImmuneProfile.size(); ++i) {
            adaptativeImmuneProfile[i] += virus->getVirulenceProfile(i) * dt.asSeconds() * 0.1;
        }
        virus->setInfectiveAgentQty(virus->getInfectiveAgentQty() - getScore() * dt.asSeconds());
        if (virus->isDead()) {
            delete virus;
            virus = nullptr;
        }
    } else {
        if (activationLevel > getAppConfig().immune_adaptive_baseline) {
            activationLevel *= 0.995;
            if (activationLevel < getAppConfig().immune_adaptive_baseline) {
                activationLevel = getAppConfig().immune_adaptive_baseline;
            }
        }
        if (health < getAppConfig().immune_health_max) {
            health += getAppConfig().immune_health_recovery * dt.asSeconds();
            if (health > getAppConfig().immune_health_max) {
                health = getAppConfig().immune_health_max;
            }
        }
    }
}

double ImmuneSystem::getHealth() const {
    return health;
}

void ImmuneSystem::setHealth (double h) {
    health = h;
}

double ImmuneSystem::getScore() const {
    return score;
}

double ImmuneSystem::getAdaptScore() const {
    return adaptScore;
}

void ImmuneSystem::initScores() {
    if(virus == nullptr) {
        score = 0;
        adaptScore = 0;
    } else {
        double tempScore(0);
        for(size_t i(0); i < adaptativeImmuneProfile.size(); ++i) {
            tempScore += adaptativeImmuneProfile[i] * virus->getVirulenceProfile(i);
        }
        tempScore *= getAppConfig().immune_defense_effectiveness;
        adaptScore = tempScore;
        for(size_t i(0); i < adaptativeImmuneProfile.size(); ++i) {
            tempScore += getAppConfig().immune_defense_effectiveness * ((host->getGenome())->getImmuneGenes(i) * virus->getVirulenceProfile(i)
            + uniform (-getAppConfig().immune_defense_random_variability, getAppConfig().immune_defense_random_variability));
        }
        tempScore *= tempScore;
        tempScore *= activationLevel;
        score = tempScore;
    }
}

void ImmuneSystem::setVirus(Virus* v) {
    virus = v;
}

Virus* ImmuneSystem::getVirus() const {
    return virus;
}

double ImmuneSystem::getActivationLevel() const {
    return activationLevel;
}

ImmuneSystem::ImmuneSystem(Animal* host)
:health(getAppConfig().immune_health_max), activationLevel(getAppConfig().immune_adaptive_baseline), host(host), virus(nullptr)
{
    for(auto& x:adaptativeImmuneProfile) {
        x = 0;
    }
}

ImmuneSystem::~ImmuneSystem(){
    delete virus;
    virus = nullptr;
}
