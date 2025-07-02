#pragma once
#include "Building.h"
#include <vector>
#include <string>

class Character;

class Facility : public Building {
private:
	std::string type;

public:
	Facility(const std::string& type, const std::string& buildingId, const std::string& name);

	void printData(std::ostream& out) const override;
	const std::string& getType() const override;

};


