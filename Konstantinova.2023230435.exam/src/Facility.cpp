#include "../include/Facility.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Facility::Facility(const string& type, const string& name) : Building(name), type(type) {
	this->buildingId = "FC" + to_string(++this->idCounter);
}

void Facility::printData(ostream& out) const {
	out << "id: " + this->buildingId + ", name: " + this->getName() + ", type: " + this->getType() << endl;
	out << "Workers: " << endl;
	for (Character* worker : this->characters) {
		out << worker->getName() << " " << worker->getLastName() << ", occupation: " << worker->getOccupation() << ", salary: " << worker->getSalary() << endl;
	}
}
const string& Facility::getType() const {
	return this->type;
}

bool Facility::isResidential() const { return false; }
