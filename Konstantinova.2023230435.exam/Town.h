#pragma once
#include <string>
#include <vector>
#include "Character.h"
#include "Station.h"
#include "Building.h"
#include "Facility.h"
#include "Residential.h"
#include <memory>
using Edge = std::pair<Station*, int>;

class Town {
private: 
	std::string name;
	std::vector<std::unique_ptr<Character>> characters;
	std::vector<std::unique_ptr<Building>> buildings;
	std::vector<std::unique_ptr<Station>> stations;

public:
	Town(std::string name);
	~Town() = default;
	void setName(std::string name);
	void saveTown();

	void printCharacters(std::ostream& out) const; //const because these methods don't modify the object
	void printBuildings(std::ostream& out) const;
	void printData(std::ostream& out) const;
	void printMap(std::ostream& out) const;

	void addBuilding(std::unique_ptr<Building> building, Station* station);
	void addCharacter(std::unique_ptr<Character> character);
	void addStation(std::unique_ptr<Station> station);

	void removeBuilding(const std::string& buildingId);
	void removeCharacter(const std::string& characterId);
	void removeStation(const std::string& stationId);

	void moveCharacter(Character* character, Residential* building);
	void hireCharacter(Character* character, Facility* building, int salary);
	void connectStations(Station* start, Station* end, int dist);
	
	const std::string& getName() const;

	auto findCharacterById(const std::string& charId) const;
	auto findBuildingById(const std::string& buildingId) const;
	auto findStationById(const std::string& stationId) const;
	auto findStationByName(const std::string& stationName) const;
	std::vector<Station*> findRoute(Station* start, Station* finish) const;
	std::vector<Residential*> findFreeHouses() const;
	std::vector<Character*> findHomeless() const;
};
