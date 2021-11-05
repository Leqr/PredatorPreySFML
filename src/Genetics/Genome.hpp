#ifndef GENOME_HPP
#define GENOME_HPP
#include <array>
#include <iostream>
enum Sex {MALE, FEMALE};
enum ColorAllele {WHITE, BLACK};

class Genome {
private:
    Sex sex;
    std::array<ColorAllele, 2> geneticMaterial;
    std::array<double, 10> immuneProfile;

public:
    std::string getStringSex() const;
    Sex getSex() const;
    ColorAllele getFirstColorAllele() const;
    ColorAllele getSecondColorAllele() const;
    ColorAllele getColorPhenotype() const;
    double getImmuneGenes(size_t idx) const;
    void setImmuneGenes(std::array<double, 10> const& otherProfile);
    ColorAllele getGeneticMaterial(size_t idx) const;
    size_t getProfileSize() const;
    Genome(Genome* motherGenome = nullptr, Genome* fatherGenome = nullptr);
    Genome(const Genome& genome);
    };

std::ostream& operator<<(std::ostream& out, const Genome& genome);

#endif
