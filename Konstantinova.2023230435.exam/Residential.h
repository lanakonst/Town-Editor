#pragma once
#include "Building.h"
#include <vector>
#include <string>

class Character;

class Residential : public Building {
private:
	int capacity;

public:
	Residential(int capacity, const std::string& buildingId, const std::string& name);

	void printData(std::ostream& out) const override;
	const std::string& getType() const override;

	int getFreePlaces() const;
	int getCapacity() const;
};

