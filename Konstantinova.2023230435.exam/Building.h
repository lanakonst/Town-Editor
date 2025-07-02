#pragma once
#include <string>
#include <vector>
#include <memory>

class Character; //circular dependency
class Station;

class Building {
protected:
    std::string buildingId, name;
    std::vector <Character*> characters;
    Station* station = nullptr;

public:
	Building(const std::string& buildingId, const std::string& name);
    virtual ~Building() = default;

    const std::string& getId() const;
    const std::string& getName() const;
    Station* getStation() const;
    void setStation(Station* station);
    void removeAllCharacters();
    const std::vector<Character*>& getCharacters() const;


    Building& operator +=(Character* character);
    Building& operator -=(Character* character);

    virtual void printData(std::ostream& out) const = 0;

    virtual const std::string& getType() const = 0;
};
