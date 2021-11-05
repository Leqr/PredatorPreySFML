#include "Genome.hpp"
#include <Random/Random.hpp>

std::string Genome::getStringSex() const{
  switch (sex) {
    case MALE:
    return "Male";
    break;
    case FEMALE:
    return "Female";
    break;
  }
}

Sex Genome::getSex() const{
    return sex;
}

ColorAllele Genome::getFirstColorAllele() const {
    return geneticMaterial[0];
}

ColorAllele Genome::getSecondColorAllele() const {
    return geneticMaterial[1];
}

ColorAllele Genome::getColorPhenotype() const {
    if(geneticMaterial[0] == geneticMaterial[1]) {
        return geneticMaterial[0];
    } else return WHITE;
}


double Genome::getImmuneGenes(size_t idx) const {
    return immuneProfile[idx];
}

std::ostream& operator<<(std::ostream& out, const Genome& genome) {
    out << "Genome (";
    if (genome.getSex() == FEMALE) {
        out << "femelle";
    } else {
        out << "male";
    }
    out << ") : " << std::endl << "Color phenotype = ";
    if (genome.getColorPhenotype() == WHITE) {
        out << "white";
    } else {
        out << "black";
    }
    out << std::endl << "First color allele = ";
    if (genome.getFirstColorAllele() == WHITE) {
        out << "white";
    } else {
        out << "black";
    }
    out << std::endl << "Second color allele = ";
    if (genome.getSecondColorAllele() == WHITE) {
        out << "white";
    } else {
       out << "black";
    }
    out << std::endl << "Immune profile =";
    for (size_t i(0); i < 10; ++i) {
        out << " " << genome.getImmuneGenes(i) ;
    }
    return out;
}

void Genome::setImmuneGenes(std::array<double, 10> const& otherProfile) {
    for (size_t i(0); i < immuneProfile.size(); ++i) {
        immuneProfile[i] = otherProfile[i];
    }
}

ColorAllele Genome::getGeneticMaterial(size_t idx) const {
    return geneticMaterial[idx];
}

size_t Genome::getProfileSize() const {
    return immuneProfile.size();
}

Genome::Genome(Genome* motherGenome, Genome* fatherGenome)  {
    if (bernoulli(0.5)) sex = FEMALE;
    else sex = MALE;
    if ((motherGenome == nullptr) or (fatherGenome == nullptr)) {
        for (int i(0); i < 2; ++i) {
            if (bernoulli(0.3)) geneticMaterial[i] = BLACK;
            else geneticMaterial[i] = WHITE;
        } for (int i(0); i < 10; ++i) {
            immuneProfile[i] = uniform(-5.00, 5.00);
        }

    } else {
        if (bernoulli(0.5)) {
            geneticMaterial[0] = motherGenome->geneticMaterial[0];
        } else {
            geneticMaterial[0] = motherGenome->geneticMaterial[1];
        }
        if (bernoulli(0.5)) {
            geneticMaterial[1] = fatherGenome->geneticMaterial[0];
        } else {
            geneticMaterial[1] = fatherGenome->geneticMaterial[1];
        }
        for (size_t i(0); i < 10; ++i) {
            if (bernoulli(0.5)) {
                immuneProfile[i] = motherGenome->immuneProfile[i];
            } else {
                immuneProfile[i] = fatherGenome->immuneProfile[i];
            } immuneProfile[i] += uniform(-0.3, 0.3);
        }
    }
}

Genome::Genome(const Genome& genome)
:sex(genome.sex), geneticMaterial(genome.geneticMaterial), immuneProfile(genome.immuneProfile)
{}
