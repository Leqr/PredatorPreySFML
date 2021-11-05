#include "Stats.hpp"
#include <vector>
#include <Application.hpp>

void Stats::setActive(int i){
    activeIdentifier = i;
}

void Stats::reset(){
    for(auto& p : graphs){
        p.second->reset();
    }
}

void Stats::addGraph(int id, std::string label, std::vector<std::string> titles, double min, double max, Vec2d windowSize){
    activeIdentifier = id;
    graphs[id].reset();
    graphs[id] = std::unique_ptr<Graph>(new Graph(titles, windowSize, min, max));
    labels[id] = label;
}

void Stats::update(sf::Time dt){
    graphs[activeIdentifier]->updateData(dt, getAppEnv().fetchData(labels[activeIdentifier]));
}

void Stats::drawOn(sf::RenderTarget& targetWindow){
    graphs[activeIdentifier]->drawOn(targetWindow);
}

void Stats::focusOn(std::string graphTitle){
    graphs[activeIdentifier]->reset();
    for(auto& x : labels){
        if(x.second == graphTitle){
            activeIdentifier = x.first;
        }
    }
}

void Stats::changeStatsScale() {
    graphs[activeIdentifier]->changeGraphScale();
}
