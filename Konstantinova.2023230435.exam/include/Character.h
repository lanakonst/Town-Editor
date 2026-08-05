#pragma once
#include "Building.h"
#include "Residential.h"
#include "Facility.h"
#include <string>
#include "json.hpp"

class Building;
class Facility;
class Residential;

class Character {
private:
	static int idCounter;
	std::string charId, name, lastName, occupation;
	Residential* home = nullptr;
	Facility* workplace = nullptr;
	int age;
	int salary = 0;

public:
	Character(const std::string& name, const std::string& lastName, const std::string& occupation, int age, int salary, const std::string& id = "");
	void setName(const std::string& name);
	void setLastName(const std::string& lastName);
	void setHome(Residential* home);
	void setOccupation(const std::string& occupation);
	void setWorkplace(Facility* workplace);
	void setAge(int age);
	void setSalary(int salary);

	void moveOut();
	void resign();

	void printData(std::ostream& out) const;

	bool checkHomeless() const;
	bool checkUndemployed() const;

	const std::string& getName() const;
	const std::string& getLastName() const;
	const std::string& getCharId() const;
	const std::string& getOccupation() const;
	Facility* getWorkplace() const;
	Residential* getHome() const;
	int getAge() const;
	int getSalary() const;

	nlohmann::json toJson() const;

};

