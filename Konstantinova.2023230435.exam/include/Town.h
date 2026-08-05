#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Character.h"
#include "Station.h"
#include "Building.h"
#include "Facility.h"
#include "Residential.h"
#include "json.hpp"
#include <memory>

class Town {
private: 
	std::string name;
	std::vector<std::unique_ptr<Character>> characters;
	std::vector<std::unique_ptr<Building>> buildings;
	std::vector<std::unique_ptr<Station>> stations;

public:
	Town(std::string name);
	~Town() = default;

	Town(const Town&) = delete;
	Town& operator=(const Town&) = delete;

	Town(Town&&) = default;
	Town& operator=(Town&&) = default;

	void setName(std::string name);
	void saveTown();
	
	nlohmann::json toJson();

	static Town loadTown(const std::string& filename); //static because it doesn't require an instance of Town to be called (belongs to the class and not the instance)
	void loadStations(const nlohmann::json& json);
	void loadBuildings(const nlohmann::json& json);
	void loadCharacters(const nlohmann::json& json);

	const std::vector<std::unique_ptr<Station>>& getStations() const;
	const std::vector<std::unique_ptr<Character>>& getCharacters() const;
	const std::vector<std::unique_ptr<Building>>& getBuildings() const;

	void printCharacters(std::ostream& out) const; //const because these methods don't modify the object
	void printHomeless(std::ostream& out) const;
	void printBuildings(std::ostream& out) const;
	void printData(std::ostream& out) const;
	void printMap(std::ostream& out) const;
	void printBuildingCharacters(std::ostream& out, const std::string& buildingId) const;

	void addBuilding(std::unique_ptr<Building> building, const std::string& stationName);
	void addCharacter(std::unique_ptr<Character> character);
	void addStation(std::unique_ptr<Station> station);
	
	void removeBuilding(const std::string& buildingId);
	void removeCharacter(const std::string& characterId);
	void removeStation(const std::string& stationId);

	void moveCharacter(Character* character, Residential* building);
	void hireCharacter(Character* character, Facility* building, int salary);
	void connectStations(const std::string& startName, const std::string& endName, int dist);
	void moveBuilding(const std::string& buildingId, const std::string& stationName);
	
	const std::string& getName() const;

	std::vector<std::unique_ptr<Character>>::const_iterator findCharacterById(const std::string& charId) const;
	std::vector<std::unique_ptr<Building>>::const_iterator findBuildingById(const std::string& buildingId) const;
	std::vector<std::unique_ptr<Station>>::const_iterator findStationById(const std::string& stationId) const;
	std::vector<std::unique_ptr<Station>>::const_iterator findStationByName(const std::string& stationName) const;
	std::vector<Station*> findRoute(Station* start, Station* finish) const;
	std::vector<Residential*> findFreeHouses() const;
	std::vector<Character*> findHomeless() const;
};
