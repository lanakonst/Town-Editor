#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <filesystem>
#include "../include/Town.h"
#include "../include/Character.h"
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

Town::Town(string name) : name(name) {}
void Town::setName(string newName) { 
	string oldFile = "saves/" + this->name + ".json";
	string newFile = "saves/" + newName + ".json";

	if (fs::exists(newFile)) {
		cerr << "A town with the name " << newName << " already exists. Please choose a different name." << endl;
		return;
	}

	if (fs::exists(oldFile)) {
		try {
			fs::rename(oldFile, newFile);
		}
		catch (const fs::filesystem_error& e) {
			cerr << "Could not rename save file: " << e.what() << endl;
			return;
		}
	}

	this->name = newName;
}

json Town::toJson() {
	json jsonTown;
	jsonTown["name"] = this->name;
	jsonTown["stations"] = json::array();
	jsonTown["buildings"] = json::array();
	jsonTown["characters"] = json::array();
	for (const auto& station : this->stations) {
		jsonTown["stations"].push_back(station->toJson());
	}

	for (const auto& building : this->buildings) {
		jsonTown["buildings"].push_back(building->toJson());
	}

	for (const auto& character : this->characters) {
		jsonTown["characters"].push_back(character->toJson());
	}
	return jsonTown;
}

void Town::saveTown() {
	std::filesystem::create_directory("saves");

	string filename = "saves/" + this->name + ".json";
	ofstream outFile(filename);

	if (!outFile) {
		cerr << "Error opening file for writing!" << endl;
		return;
	}
	outFile << toJson() << '\n';
	outFile.close();
	cout << "Town saved to " << filename << endl;
}


Town Town::loadTown(const string& filename) {
	ifstream inFile(filename);
	if (!inFile) {
		cerr << "There is no such town" << endl;
		return Town("");
	}

	json townJson = json::parse(inFile);

	Town town(townJson["name"]);

	town.loadStations(townJson["stations"]);
	town.loadBuildings(townJson["buildings"]);
	town.loadCharacters(townJson["characters"]);

	return town;

}
void Town::loadStations(const json& json) {
	for (const auto& stationInfo : json) {
		this->addStation(make_unique<Station>(stationInfo["name"], stationInfo["id"]));
	}

	for (const auto& stationInfo : json) {
		string startName = stationInfo["name"];
		
		for (const auto& edge : stationInfo["edges"]) {
			auto endStationIt = findStationById(edge["stationId"]);
				if (endStationIt != stations.end()) {
					string endName = endStationIt->get()->getName();
					this->connectStations(startName, endName, edge["dist"]);
				}
		}
		
	}
}

void Town::loadBuildings(const json& json) {
	for (const auto& buildingInfo : json) {
		string type = buildingInfo["type"];
		unique_ptr<Building> building;
		if (type == "Residential") {
			building = make_unique<Residential>(buildingInfo["capacity"], buildingInfo["name"], buildingInfo["id"]);
		}
		else if (type == "Facility") {
			building = make_unique<Facility>(type, buildingInfo["name"], buildingInfo["id"]);
		}
		else {
			cerr << "Unknown building type: " << type << endl;
			continue;
		}

		auto stationIt = findStationById(buildingInfo["stationId"]);
		if (stationIt != stations.end()) {
			addBuilding(move(building), stationIt->get()->getName());
		}
	}
}

void Town::loadCharacters(const json& json) {
	for (const auto& characterInfo : json) {
		unique_ptr<Character> character = make_unique<Character>(
			characterInfo["name"],
			characterInfo["lastName"],
			characterInfo["occupation"],
			characterInfo["age"],
			characterInfo["salary"],
			characterInfo["charId"]
		);
	if(characterInfo.contains("homeId") && !characterInfo["homeId"].is_null()) {
		auto buildingIt = this->findBuildingById(characterInfo["homeId"]);
		if (buildingIt != this->buildings.end()) {
			Residential* home = dynamic_cast<Residential*>(buildingIt->get());
			character->setHome(home);
			*home += character.get();
		}
	}

	if (characterInfo.contains("workplaceId") && !characterInfo["workplaceId"].is_null()) {
		auto buildingIt = this->findBuildingById(characterInfo["workplaceId"]);
		if (buildingIt != this->buildings.end()) {
			Facility* work = dynamic_cast<Facility*>(buildingIt->get());
			character->setWorkplace(work);
			*work += character.get();
		}
	}

		this->addCharacter(move(character));
	}
}

const vector<unique_ptr<Station>>& Town::getStations() const {
	return this->stations;
}

const vector<unique_ptr<Character>>& Town::getCharacters() const {
	return this->characters;
}

const vector<unique_ptr<Building>>& Town::getBuildings() const {
	return this->buildings;
}

vector<unique_ptr<Character>>::const_iterator Town::findCharacterById(const string& charId) const {
	auto it = find_if(this->characters.begin(), this->characters.end(), [&](const unique_ptr<Character>& character) {return character->getCharId() == charId; });
	return it;
}

vector<unique_ptr<Building>>::const_iterator Town::findBuildingById(const string& buildingId) const {
	auto it = find_if(this->buildings.begin(), this->buildings.end(), [&](const unique_ptr<Building>& building) {return building->getId() == buildingId; });
	return it;
}

vector<unique_ptr<Station>>::const_iterator Town::findStationByName(const string& stationName) const {
	auto it = find_if(this->stations.begin(), this->stations.end(), [&](const unique_ptr<Station>& station) {return station->getName() == stationName; });
	return it;
}

vector<unique_ptr<Station>>::const_iterator Town::findStationById(const string& stationId) const {
	auto it = find_if(this->stations.begin(), this->stations.end(), [&](const unique_ptr<Station>& station) {return station->getId() == stationId; });
	return it;
}


void Town::printCharacters(ostream& out) const {
	if (this->characters.empty()) {
		out << "Town is empty" << endl;
		return;
	}
	for_each(this->characters.begin(), this->characters.end(), [&](const unique_ptr<Character>& character) {character->printData(out); });
}

void Town::printHomeless(std::ostream& out) const {
	vector<Character*> homeless = this->findHomeless();
	if (homeless.empty()) {
		out << "Everyone has a home" << endl;
		return;
	}
	for_each(homeless.begin(), homeless.end(), [&](Character* character) {character->printData(out); });
}

void Town::printBuildings(ostream& out) const {
	if (this->buildings.empty()) {
		out << "There are no buildings" << endl;
		return;
	}
	for_each(this->buildings.begin(), this->buildings.end(), [&](const unique_ptr<Building>& building) {building->printData(out); });
}
void Town::printData(ostream& out) const {
	out << this->name << ":" << endl;
	out << "Number of sitizens: " << this->characters.size() << endl;
	out << "Number of stations: " << this->stations.size() << endl;
	out << "Number of buildings: " << this->buildings.size() << endl;
}
void Town::printMap(ostream& out) const {
	if (this->stations.empty()) {
		out << "There are no stations yet" << endl;
		return;
	}
	for_each(this->stations.begin(), this->stations.end(), [&](const unique_ptr<Station>& station) {station->printEdges(out); });
}

void Town::printBuildingCharacters(ostream& out, const string& buildingId) const {
	auto buildingIt = this->findBuildingById(buildingId);
	if (buildingIt == this->buildings.end()) {
		cout << "no buildings under id " << buildingId << endl;
		return;
	}
	Building* building = buildingIt->get();
	building->printData(out);
}

void Town::addBuilding(unique_ptr<Building> building, const string& stationName) {
	auto stationIt = this->findStationByName(stationName);
	if (stationIt == this->stations.end()) {
		cout << "No station under this name" << endl;
		return;
	}
	Station* station = stationIt->get();
	Building* newBuilding = building.get();
	this->buildings.push_back(move(building)); //to pass ownership of the smart pointer to Town
	newBuilding->setStation(station);
	station->addBuilding(newBuilding);
}

void Town::addCharacter(unique_ptr<Character> character) {
	this->characters.push_back(move(character));
}
void Town::addStation(unique_ptr<Station> station) {
	if (this->findStationByName(station->getName()) == this->stations.end()) {
		this->stations.push_back(move(station));
	}
	else {
		cerr << "Station with this name already exists" << endl; //cerr -- error
	}
}

void Town::removeStation(const string& stationName) {
	auto stationToRemoveIT = this->findStationByName(stationName);
	if (stationToRemoveIT == this->stations.end()) {
		cerr << "No stations under this id" << endl;
		return;
	}
	Station* stationToRemove = stationToRemoveIT->get();
	if (!stationToRemove->isEmpty()) {
		cout << "Please move all building to another stations before deleting" << endl;
		return;
	}
	this->stations.erase(stationToRemoveIT);
	cout << "Station is removed" << endl;
}

void Town::removeBuilding(const string& buildingId) {
	auto buildingToRemoveIT = this->findBuildingById(buildingId);
	if (buildingToRemoveIT == this->buildings.end()) {
		cerr << "No buildings under this id" << endl;
		return;
	}
	Building* buildingToRemove = buildingToRemoveIT->get();
	bool isResidential = dynamic_cast<Residential*>(buildingToRemove) != nullptr;
	for (Character* character : buildingToRemove->getCharacters()) {
		if (isResidential) {
			character->setHome(nullptr);
		}
		else {
			character->setWorkplace(nullptr);
		}
	}
	buildingToRemove->removeAllCharacters();
	*buildingToRemove->getStation() -= buildingToRemove;
	this->buildings.erase(buildingToRemoveIT);
	cout << "Building is removed" << endl;
}


void Town::removeCharacter(const string& characterId) {
	auto charToRemoveIT = this->findCharacterById(characterId);
	if (charToRemoveIT == this->characters.end()) {
		cerr << "No characters under this id" << endl; //error
	}
	else {
		Character* charToRemove = charToRemoveIT->get();
		if (!charToRemove->checkHomeless()) {
			Residential* home = charToRemove->getHome();
			*home -= charToRemove;
		}
		if (!charToRemove->checkUndemployed()) {
			Facility* workplace = charToRemove->getWorkplace();
			*workplace -= charToRemove;
		}
		this->characters.erase(charToRemoveIT);
		cout << "Character is removed" << endl;
	}
}

void Town::moveCharacter(Character* character, Residential* building) {
	if (building->getFreePlaces() == 0) {
		cout << "No available places in this building" << endl;
		return;
	}
	Residential* prevHome = character->getHome();
	if (prevHome != nullptr) {
		*prevHome -= character;
	}
	character->setHome(building);
	*building += character;
	cout << "Character is successfully moved" << endl;
}

void Town::hireCharacter(Character* character, Facility* building, int salary) {
	Facility* prevWork = character->getWorkplace();
	if (prevWork != nullptr) {
		*prevWork -= character;
	}
	character->setWorkplace(building);
	character->setSalary(salary);
	*building += character;
	cout << "character is succesfully hired" << endl;
}

const string& Town::getName() const { return this->name; }

void Town::connectStations(const string& startName, const string& endName, int dist) {
	auto startIt = this->findStationByName(startName);
	if (startIt == this->stations.end()) {
		cout << "No stations under name " << startName << endl;
		return;
	}
	auto endIt = this->findStationByName(endName);
	if (endIt == this->stations.end()) {
		cout << "No stations under name " << endName << endl;
		return;
	}
	Station* start = startIt->get();
	Station* end = endIt->get();

	start->addEdge(end, dist);
	end->addEdge(start, dist);
}

void Town::moveBuilding(const std::string& buildingId, const std::string& stationName) {
	auto stationIt = this->findStationByName(stationName);
	if (stationIt == this->stations.end()) {
		cout << "stations under name " << stationName << endl;
		return;
	}
	auto buildingIt = this->findBuildingById(buildingId);
	if (buildingIt == this->buildings.end()) {
		cout << "no buildings under id " << buildingId << endl;
		return;
	}
	Station* station = stationIt->get();
	Building* building = buildingIt->get();
	*(building->getStation()) -= building;
	*station += building;
	building->setStation(station);
	cout << "Buiding " << buildingId << " was moved to " << stationName << endl;
}

vector<Station*> Town::findRoute(Station* start, Station* finish) const { return {}; }

vector<Residential*> Town::findFreeHouses() const {
	vector<Residential*> freeBuildings;
	for (const auto& it : this->buildings) {
		Building* building = it.get();
		auto residential = dynamic_cast<Residential*>(building);
		if (residential != nullptr) {
			if (residential->getFreePlaces() > 0) {
				freeBuildings.push_back(residential);
			}
		}
	}
	return freeBuildings;
}

vector<Character*> Town::findHomeless() const {
	vector<Character*> homeless;
	for (const auto& it : this->characters) {
		Character* character = it.get();
		if (character->checkHomeless()) {
			homeless.push_back(character);
		}
	}
	return homeless;
}
