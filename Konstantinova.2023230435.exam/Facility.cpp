#include "Facility.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Facility::Facility(const string& type, const string& buildingId, const string& name) :Building(buildingId, name), type(type) {}

void Facility::printData(ostream& out) const {
	out << "name: " + this->getName() + ", type: " + this->getType() << endl;
}
const string& Facility::getType() const {
	return this->type;
}