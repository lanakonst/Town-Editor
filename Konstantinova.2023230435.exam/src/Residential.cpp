#include "../include/Residential.h"
#include "../include/Station.h"
#include "../include/json.hpp"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using json = nlohmann::json;

Residential::Residential(int capacity, const string& name) :Building(name), capacity(capacity) {
	this->buildingId = "RS" + to_string(++this->idCounter);
}

void Residential::printData(ostream& out) const {
	out << "id: " + this->buildingId + ", name: " + this->getName() + ", type: " + this->getType() + ", free rooms: " + to_string(this->getFreePlaces()) + "/" + to_string(this->getCapacity()) << endl;
	out << "Residents: " << endl;
	for (Character* resident : this->characters) {
		out << resident->getName() << " " << resident->getLastName() << endl;
	}
}
const string& Residential::getType() const {
	static const string type = "Residential"; //static -- so it won't be destroyed outside of the function
	return type;
}

int Residential::getFreePlaces() const {
	return this->getCapacity() - this->characters.size();
}

int Residential::getCapacity() const {
	return this->capacity;
}

bool Residential::isResidential() const { return true; }

json Residential::toJson() const {
	json jsonResidential;
	jsonResidential["id"] = this->buildingId;
	jsonResidential["name"] = this->name;
	jsonResidential["type"] = this->getType();
	jsonResidential["capacity"] = this->capacity;
	jsonResidential["residents"] = json::array();
	for (const auto& resident : this->characters) {
		jsonResidential["residents"].push_back(resident->getCharId());
	}
	if (this->station != nullptr) {
		jsonResidential["stationId"] = this->station->getId();
	}
	else {
		jsonResidential["stationId"] = nullptr;
	}
	return jsonResidential;
}
