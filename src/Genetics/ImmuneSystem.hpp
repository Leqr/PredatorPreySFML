#ifndef IMMUNESYSTEME_HPP
#define IMMUNESYSTEME_HPP
#include <array>
#include <iostream>
#include "Config.hpp"
#include "Env/Animal.hpp"
#include "Virus.hpp"

class Animal;
class ImmuneSystem {
private:
    double health;
    double score;
    double adaptScore;
    std::array<double, 10> adaptativeImmuneProfile;
    double activationLevel;
    Animal* host;
    Virus* virus;

public:
    void update(sf::Time dt);
    double getHealth() const;
    void setHealth (double h);
    double getScore() const;
    double getAdaptScore() const;
    void initScores();
    void setVirus(Virus* v);
    Virus* getVirus() const;
    double getActivationLevel() const;
    ImmuneSystem(Animal* host);
    ~ImmuneSystem();
};

#endif
