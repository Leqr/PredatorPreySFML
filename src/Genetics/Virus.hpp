#ifndef VIRUS_HPP
#define VIRUS_HPP
#include <array>
#include <iostream>
#include "Config.hpp"

class Virus {
private:
    std::array<double, 10> virulenceProfile;
    double infectiveAgentQty;
    double growthRate;

public:
    double getInfectiveAgentQty() const;
    double getVirulenceProfile(size_t i) const;
    void setInfectiveAgentQty(double other);
    bool isDead() const;
    void update(sf::Time dt);
    Virus();
    Virus(Virus const& virus);
    Virus(std::array<double, 10> const& otherVirPro);
    ~Virus();
};













#endif
