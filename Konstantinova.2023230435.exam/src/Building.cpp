#include "../include/Building.h"
#include "../include/Station.h"
#include "../include/json.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int Building::idCounter = 0;


Building::Building(const string& name) : name(name) {}  

const string& Building::getId() const { return this->buildingId; }
const string& Building::getName() const { return this->name; }
Station* Building::getStation() const { return this->station; }
void Building::setStation(Station* station) { this->station = station; }
void Building::removeAllCharacters() {
    this->characters.clear();
}
const std::vector<Character*>& Building::getCharacters() const { return this->characters; }

Building& Building::operator +=(Character* character) {
    auto it = find(this->characters.begin(), this->characters.end(), character);
    if (it == this->characters.end()) {
        this->characters.push_back(character);
    }
    else {
        std::cout << "This character already lives/works here";
    }
    
    return *this;
}
Building& Building::operator -=(Character* character) {
    auto it = find(this->characters.begin(), this->characters.end(), character);
    if (it != this->characters.end()) {
        this->characters.erase(it);
    }
    return *this;
}
