#pragma once
#include "Building.h"
#include "Character.h"
#include <vector>
#include <string>
#include "json.hpp"

class Character;

class Residential : public Building {
private:
	int capacity;

public:
	Residential(int capacity, const std::string& name);

	void printData(std::ostream& out) const override;
	const std::string& getType() const override;
	bool isResidential() const override;

	int getFreePlaces() const;
	int getCapacity() const;
	nlohmann::json toJson() const override;
};

