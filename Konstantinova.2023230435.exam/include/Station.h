#pragma once
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

class Building;

class Station {
private:
	static int idCounter;
	std::string stationId;
	std::string name;
	std::vector<Building*> buildings;
	std::vector<std::pair<Station*, int>> edges;

public:
	Station(const std::string& name);

	void saveStations(std::ostream& out) const;
	void setName(const std::string& name);
	void addBuilding(Building* building);
	void removeBuilding(Building* building);
	void addEdge(Station* end, int distance);
	void removeEdge(Station* end);
	void removeAllBuildings();

	void printBuildings(std::ostream& out) const;
	void printEdges(std::ostream& out) const;
	auto findEdge(Station*) const;

	const std::string& getName() const;
	const std::vector<std::pair<Station*, int>>& getEdges() const;
	const std::string& getId() const;

	bool isEmpty() const;
	

	Station& operator +=(Building* building);
	Station& operator -=(Building* building);
};

