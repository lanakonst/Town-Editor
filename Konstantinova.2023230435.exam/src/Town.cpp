#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include "../include/Town.h"
#include "../include/Character.h"

using namespace std;

Town::Town(string name) : name(name) {}
void Town::setName(string name) { this->name = name; }
void Town::saveTown() {
	string filename = this->name + ".txt";
	ofstream outFile(filename);

	if (!outFile) {
		cerr << "Error opening file for writing!" << endl;
		return;
	}
	outFile << this->name << '\n';

	outFile << "Stations: {\n";
	for (const auto& station : this->stations) {
		station->saveStations(outFile);
	}
	outFile << "}\n";
	outFile.close();

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

/*

Town::Town(string name) {
		this->name = name;
	}

void Town::setName(string name) { this->name = name; }

	
	void printData();
	void printCharacters();
	void printBuildings();

void Town::addBuilding(unique_ptr<Building> building) {
		
		buildings.push_back(move(building));
	}

void Town::addCharacter(unique_ptr<Character> character) {
		
		this->characters.push_back(move(character));
	}

void Town::removeBuilding(string buildingId) {
		if (nOfbuildings == 0) {
			cout << "There are no buildings to remove" << endl;
			return;
		}
		auto it = find_if(buildings.begin(), buildings.end(), [buildingId](const unique_ptr<Building>& building) {
			return building->getId() == buildingId;
			});
		if (it == buildings.end()) {
			cout << "There is no such building" << endl;
		}
		else {
			const unique_ptr<Building>& buildingToremove = *it;
			buildingToremove->removeAllCharacters();
			buildings.erase(it);
			nOfbuildings -= 1;
			cout << "Building number " << buildingId << " is successfully removed" << endl;
		}
	}

void Town::removeCharacter(string characterId) {
		if (nOfCharacters == 0) {
			cout << "There are no characters to remove" << endl;
			return;
		}
		auto it = find_if(characters.begin(), characters.end(), [characterId](const unique_ptr<Character>& character) {
			return character->getCharId() == characterId;
			});
		if (it == characters.end()) {
			cout << "There is no such character" << endl;
		}
		else {
			Character* charToRemove = it->get();
			Building* building = charToRemove->getBuilding();
			*building -= *charToRemove;
			characters.erase(it);
			nOfCharacters -= 1;
			cout << "Character number " << characterId << " is successfully removed" << endl;
		}
	}

void Town::printData(ostream& out) const{
	out << this->name << " Town " << endl;
	out << "Number of buildings: " << this->nOfbuildings;
	out << "Number of characters: " << this->nOfCharacters;
}


void Town::printCharacters(ostream& out) const{
	for (const auto& character : this->characters) character->printData(out);
}
void Town::printBuildings(ostream& out) const {
	for (const auto& building : this->buildings) building->printData(out);
}
	
	void printHomeless();
	vector<Building*> findFreeHouses();

	void saveTown();





class Town {
private:
	string name;
	int nOfCharacters = 0, nOfbuildings = 0;
	vector<unique_ptr<Character>> characters;
	vector<unique_ptr<Building>> buildings;
public:
	Town(string name) {
		this->name = name;
	};
	void setName(string name) { this->name = name; };

	/*
	void printData();
	void printCharacters();
	void printBuildings();

	void addBuilding(unique_ptr<Building> building) {
		nOfbuildings += 1;
		buildings.push_back(building);
	}

	void addCharacter(unique_ptr<Character> character) {
		this->nOfCharacters += 1;
		this->characters.push_back(character);
	}

	void removeBuilding(string buildingId) {
		if (nOfbuildings == 0) {
			cout << "There are no buildings to remove" << endl;
			return;
		}
		auto it = find_if(buildings.begin(), buildings.end(), [buildingId](Building* building) {
			return building->getId() == buildingId;
			});
		if (it == buildings.end()) {
			cout << "There is no such building" << endl;
		}
		else {
			unique_ptr<Building>& buildingToremove = *it;
			buildingToremove->removeAll();
			buildings.erase(it);
			nOfbuildings -= 1;
			cout << "Building number " << buildingId << " is successfully removed" << endl;
		}
	}
	void removeCharacter(string characterId) {
		if (nOfCharacters == 0) {
			cout << "There are no characters to remove" << endl;
			return;
		}
		auto it = find_if(characters.begin(), characters.end(), [characterId](Character* character) {
			return character->getCharId() == characterId;
			});
		if (it == characters.end()) {
			cout << "There is no such character" << endl;
		}
		else {
			Character * charToRemove = it->get();
			Building* building = charToRemove->getBuilding();
			*building -= *charToRemove;
			characters.erase(it);
			nOfCharacters -= 1;
			cout << "Character number " << characterId << " is successfully removed" << endl;
		}
	}

	
	void printHomeless();
	vector<Building*> findFreeHouses();

	void saveTown();


};
*/