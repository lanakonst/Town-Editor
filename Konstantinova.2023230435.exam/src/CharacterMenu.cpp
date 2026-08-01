#include "../include/menu/CharacterMenu.h"
#include <string>
#include <iostream>
#include <vector>
#include "../include/TownException.h"

using namespace std;

void runCharacterMenu(Town* town) {
	int opt = 1;
	while (opt != 0) {
		try {
			cout << "\n" << endl;
			cout << "1. Print characters" << endl;
			cout << "2. Add character" << endl;
			cout << "3. Choose character" << endl;
			cout << "0. Exit" << endl;
			cout << "Enter option: ";
			cin >> opt;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(20, '\n'); //ignore up to 20 chars in the buffer until \n
				throw TownException("Invalid option");
			}
			string name, lastName, occupation, characterId;
			int age, salary;
			switch (opt) {
			case 1: {
				int optPrint = 1;
				cout << "1. Print all" << endl;
				cout << "2. Print homeless" << endl;
				cin >> optPrint;
				switch (optPrint) {
				case 1:
					town->printCharacters(cout);
					break;
				case 2:
					town->printHomeless(cout);
					break;
				default:
					cout << "Invalid option" << endl;
					break;
				}
				break;
			}
			case 2: {
				cout << "Enter first name: ";
				cin.ignore();
				getline(cin, name);
				cout << "Enter last name: ";
				getline(cin, lastName);
				cout << "Enter age: ";
				cin >> age;
				cin.ignore();
				cout << "Enter occupation: ";
				getline(cin, occupation);
				cout << "Enter salary: ";
				cin >> salary;
				town->addCharacter(make_unique<Character>(name, lastName, occupation, age, salary));
				break;
			}
			case 3: {
				cout << "Enter character id: ";
				cin >> characterId;
				auto charIt = town->findCharacterById(characterId);
				if (charIt != town->getCharacters().end()) {
					runChooseCharacterMenu(town, charIt->get());
				}
				else {
					cout << "No character under this id" << endl;
				}
				break;
			}
			case 0: {
				break;
			}
			default: {
				cout << "Invalid option" << endl;
				break;
			}
			}
		}catch(const TownException& e){
			cout << "Error: " << e.what() << endl;
			opt = 1;
		}
	}
}

void runChooseCharacterMenu(Town* town, Character* character) {
	int opt = 1;
	while (opt != 0) {
		cout << "\n" << endl;
		cout << character->getName() << " " << character->getLastName() << endl;
		cout << "1. Print data" << endl;
		cout << "2. Move in a house" << endl;
		cout << "3. Move out" << endl;
		cout << "4. Hire" << endl;
		cout << "5. Fire" << endl;
		cout << "6. Change age" << endl;
		cout << "7. Change occupation" << endl;
		cout << "8. Change salary" << endl;
		cout << "9. Delete character" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter option: ";
		cin >> opt;

		int age, salary;
		string buildingId, occupation;

		switch (opt) {
		case 1: {
			character->printData(cout);
			break;
		}
		case 2: {
			cout << "Enter residence id: ";
			cin >> buildingId;
			auto buildingIt = town->findBuildingById(buildingId);
			if (buildingIt == town->getBuildings().end()) {
				cout << "No buildings under this id" << endl;
			}
			else {
				if (buildingIt->get()->isResidential()) {
					Residential* building = dynamic_cast<Residential*>(buildingIt->get());
					town->moveCharacter(character, building);
				}
				else {
					cout << "This building is not residential" << endl;
				}
			}
			break;
		}
		case 3: {
			character->moveOut();
			break;
		}
		case 4: {
			cout << "Enter facility id: ";
			cin >> buildingId;
			auto buildingIt = town->findBuildingById(buildingId);
			if (buildingIt == town->getBuildings().end()) {
				cout << "No building under this id" << endl;
			}
			else {
				if (!buildingIt->get()->isResidential()) {
					Facility* building = dynamic_cast<Facility*>(buildingIt->get());
					cout << "Enter salary: ";
					cin >> salary;
					town->hireCharacter(character, building, salary);
				}
				else {
					cout << "This building is not a facility" << endl;
				}
			}
			break;
		}
		case 5: {
			character->resign();
			break;
		}
		case 6: {
			cout << "Enter age: ";
			cin >> age;
			character->setAge(age);
			break;
		}
		case 7: {
			cout << "Enter occupation: ";
			cin.ignore();
			getline(cin, occupation);
			character->setOccupation(occupation);
			break;
		}
		case 8: {
			cout << "Enter salary: ";
			cin >> salary;
			character->setSalary(salary);
			break;
		}
		case 9: {
			town->removeCharacter(character->getCharId());
			return;
		}
		case 0: {
			break;
		}
		default: {
			cout << "Invalid option" << endl;
			break;
		}
		}
	}
}