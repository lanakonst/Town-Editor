#include "Station.h"
#include "Building.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

Station::Station(const string& stationId, const string& name) : stationId(stationId), name(name) {}

void Station::setName(const string& name) { this->name = name; }
void Station::addBuilding(Building* building) { this->buildings.push_back(building); }
void Station::removeBuilding(Building* building) {
    auto it = find(this->buildings.begin(), this->buildings.end(), building);
    if (it != this->buildings.end()) {
        this->buildings.erase(it);
    }
}

bool Station::isEmpty() const {
    return this->buildings.size() == 0;
}

auto Station::findEdge(Station* end) const {
    return find_if(this->edges.begin(), this->edges.end(), [&](pair<Station*, int>& edge) { //lambd function [&] - captures surrounding veriables (end) by reference
        return edge.first == end;
        });
}

void Station::addEdge(Station* end, int distance) {
    auto it = this->findEdge(end);
    if (it == this->edges.end()) {
        this->edges.push_back({ end, distance });
    }
    
}
void Station::removeEdge(Station* end) {
    auto it = this->findEdge(end);
    if (it != this->edges.end()) {
        this->edges.erase(it);
    }
}
void Station::removeAllBuildings() {
    this->buildings.clear();
}

void Station::printBuildings(std::ostream& out) const {
    for_each(this->buildings.begin(), this->buildings.end(), [&](Building* building) {
        building->printData(out);
     });
}
void Station::printEdges(std::ostream& out) const {
    out << this->name;
    for_each(this->edges.begin(), this->edges.end(), [&](pair<Station*, int> edge) {
        out << "    <-" + to_string(edge.second) + "-> " + edge.first->name << endl;
        });
}

const string& Station::getName() const { return this->name; }
const vector<pair<Station*, int>>& Station::getEdges() const { return this->edges; }
const string& Station::getId() const { return this->stationId; }

Station& Station::operator +=(Building* building) {
    this->addBuilding(building);
    return *this;
}

Station& Station::operator -=(Building* building) {
    this->removeBuilding(building);
    return *this;
}
