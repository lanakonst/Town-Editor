#include "../include/Character.h"
#include "../include/json.hpp"
#include <iostream>
#include <string>

using namespace std;

int Character::idCounter = 0;
	Character::Character(const string& name, const string& lastName, const string& occupation, int age, int salary, const string& id) :
		name(name), lastName(lastName), occupation(occupation), age(age), salary(salary) {
		if (!id.empty()) {
			this->charId = id;
			++this->idCounter;
		}
		else {
			this->charId = "CH" + to_string(++this->idCounter);
		}
	};
	void Character::setName(const string& name) { this->name = name; }
	void Character::setLastName(const string& lastName) { this->lastName = lastName; }
	void Character::setHome(Residential* home) { this->home = home; }
	void Character::setOccupation(const string & occupation) { this->occupation = occupation; }
	void Character::setWorkplace(Facility * workplace) { this->workplace = workplace; }
	void Character::setAge(int age) { this->age = age; }
	void Character::setSalary(int salary) { this->salary = salary; }

	void Character::moveOut() { this->home = nullptr; }
	void Character::resign() { this->workplace = nullptr; }

	void Character::printData(ostream& out) const {
		string toStr = "id: " + this->charId + ", " + this->name + " " + this->lastName + ", " + to_string(this->age) + " years, ";
		toStr += (this->checkHomeless()) ? " homeless " : (" lives in " + this->home->getName());
		toStr += (this->checkUndemployed()) ? " unemployed" : ", works as " + (this->occupation + " in " + this->workplace->getName() + " for " + to_string(this->salary) + "$");

		out << toStr <<endl;
	}

	bool Character::checkHomeless() const { return this->home == nullptr; }
	bool Character::checkUndemployed() const { return this->workplace == nullptr; }

	const string& Character::getName() const { return this->name; }
	const string& Character::getLastName() const { return this->lastName; }
	const string& Character::getCharId() const { return this->charId; }
	const string& Character::getOccupation() const { return this->occupation; }
	Facility* Character::getWorkplace() const { return this->workplace; }
	Residential* Character::getHome() const { return this->home; }
	int Character::getAge() const { return this->age; }
	int Character::getSalary() const { return this->salary; }

	nlohmann::json Character::toJson() const {
		nlohmann::json jsonCharacter;
		jsonCharacter["charId"] = this->charId;
		jsonCharacter["name"] = this->name;
		jsonCharacter["lastName"] = this->lastName;
		jsonCharacter["occupation"] = this->occupation;
		jsonCharacter["age"] = this->age;
		jsonCharacter["salary"] = this->salary;
		if (this->home != nullptr) {
			jsonCharacter["homeId"] = this->home->getId();
		}
		else {
			jsonCharacter["homeId"] = nullptr;
		}
		if (this->workplace != nullptr) {
			jsonCharacter["workplaceId"] = this->workplace->getId();
		}
		else {
			jsonCharacter["workplaceId"] = nullptr;
		}
		return jsonCharacter;
	}


