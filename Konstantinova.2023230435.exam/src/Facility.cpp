#include "../include/Facility.h"
#include "../include/json.hpp"
#include "../include/Station.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using json = nlohmann::json;

Facility::Facility(const string& type, const string& name, const string& id) : Building(name), type(type) {
	if (!id.empty()) {
		this->buildingId = id;
		++this->idCounter;
	}
	else {
		this->buildingId = "FC" + to_string(++this->idCounter);
	}
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

json Facility::toJson() const {
	json jsonFacility;
	jsonFacility["id"] = this->buildingId;
	jsonFacility["name"] = this->name;
	jsonFacility["type"] = this->getType();
	jsonFacility["workers"] = json::array();
	for (const auto& worker : this->characters) {
		jsonFacility["workers"].push_back(worker->getCharId());
	}
	if (this->station != nullptr) {
		jsonFacility["stationId"] = this->station->getId();
	}
	else {
		jsonFacility["stationId"] = nullptr;
	}
	return jsonFacility;
}
