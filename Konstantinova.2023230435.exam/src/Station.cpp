#include "../include/Station.h"
#include "../include/Building.h"
#include "../include/json.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;
using json = nlohmann::json;

int Station::idCounter = 0;

Station::Station(const string& name) : name(name) {
    this->stationId = "ST" + to_string(++this->idCounter);
}

/*
void Station::saveStations(std::ostream& out) const {
    out << this->name;
    for (const pair<Station*, int>& edge : this->edges) {
        out << "    <-- " << to_string(edge.second) << " --> " << edge.first->name << endl;
    }
}*/

json Station::toJson() const {
    json jsonStation;
    jsonStation["id"] = this->stationId;
    jsonStation["name"] = this->name;

    jsonStation["buildings"] = json::array();
    jsonStation["edges"] = json::array();


    for (const auto& building : this->buildings) {
        jsonStation["buildings"].push_back(building->getId());
    }

    for (const pair<Station*, int>& edge : this->edges) {
        jsonStation["edges"].push_back({ 
            {"stationId", edge.first->getId()}, 
            {"dist", edge.second } 
            });
    }
    return jsonStation;
}

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
    return find_if(this->edges.begin(), this->edges.end(), [&](const pair<Station*, int>& edge) { //lambda function [&] - captures surrounding veriables (end) by reference
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
    out << this->name << endl;
    for_each(this->edges.begin(), this->edges.end(), [&](const pair<Station*, int> edge) {
        out << "    <-- " + to_string(edge.second) + " --> " + edge.first->name << endl;
        });
    out << endl;
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
