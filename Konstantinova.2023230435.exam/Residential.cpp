#include "Residential.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Residential::Residential(int capacity, const string& buildingId, const string& name) :Building(buildingId, name), capacity(capacity) {}

void Residential::printData(ostream& out) const {
	out << "name: " + this->getName() + ", type: " + this->getType() + ", free rooms: " + to_string(this->getFreePlaces()) + "/" + to_string(this->getCapacity()) << endl;
}
const string& Residential::getType() const {
	static const string type = "Residential"; //static -- so it won't be destroyed outside if the function
	return type;
}

int Residential::getFreePlaces() const {
	return this->getCapacity() - this->characters.size();
}

int Residential::getCapacity() const {
	return this->capacity;
}