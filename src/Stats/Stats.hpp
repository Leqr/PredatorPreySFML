#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Stats/Graph.hpp>
#include <SFML/System.hpp>
#include <string>
#include <memory>
#include <unordered_map>

class Stats {
private:
    std::unordered_map<int, std::unique_ptr<Graph>> graphs;
    std::unordered_map<int, std::string> labels;
    int activeIdentifier;

public:
    void setActive(int i);
    void reset();
    void addGraph(int id, std::string label, std::vector<std::string> titles, double min, double max, Vec2d windowSize);
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& targetWindow);
    void focusOn(std::string graphTitle);               //Allow a change of activeIdentifier with a graphTitle
    void changeStatsScale();                            //Switch between two stat scales
};

#endif
