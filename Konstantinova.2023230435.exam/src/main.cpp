#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../include/Town.h"
#include "../include/Residential.h"
#include "../include/Character.h"
#include "../include/menu/TownMenu.h"
#include "../include/menu/CharacterMenu.h"
#include "../include/menu/BuildingMenu.h"
#include "../include/TownException.h"
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;

void loadTown() {
	string townName;
	cout << "Enter town name to load: ";
	cin >> townName;
	string fName = townName + ".txt";
}

void runMainMenu(Town* town) {
	int opt = 1;
	while (opt != 0) {
		cout << "\n" << town->getName() << endl;
		cout << "1. Town menu" << endl;
		cout << "2. Buildings menu" << endl;
		cout << "3. Character menu" << endl;
		cout << "4. Save town" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter option: ";
		cin >> opt;
		if (cin.fail()) {
			throw TownException("Invalid option");
		}
		if (int(opt) == opt) {
			switch (opt) {
			case 1: {
				runTownMenu(town);
				break;
			} case 2: {
				runBuildingMenu(town);
				break;
			} case 3: {
				runCharacterMenu(town);
				break;
			} case 4: {
				town->saveTown();
				break;
			} case 0: {
				break;
			} default: {
				cout << "invalid option" << endl;
				break;
			}
			}
		}
		else {
			cout << "invalid option" << endl;
		}
	}
}


int main() {
	cout << "Welcome!" << endl;
	int opt = 1;
	while (opt != 0) {
		try {
			cout << "\n" << endl;
			cout << "1. Create new town" << endl;
			cout << "2. Load town" << endl;
			cout << "0. Exit" << endl;
			cout << "Enter option: ";
			cin >> opt;
			if (cin.fail()) {
				cin.clear();
				cin.ignore();
				throw TownException("Invalid option");
			}
			if (int(opt) == opt) {
				switch (opt) {
				case 1: {
					string townName;
					cout << "Name your town: ";
					cin >> townName;
					auto town = make_unique<Town>(townName);
					runMainMenu(town.get()); //check for nullptr
					break;
				} case 2: {
					cout << "town loaded" << endl;
					break;
				} case 0: {
					break;
				} default: {
					cout << "invalid option" << endl;
					break;
				}
				}
			}
			else {
				cout << "invalid option" << endl;
			}
		}
		catch (const TownException& e) {
			cout << "Error: " << e.what() << endl;
			opt = 1;
		}
	}

	system("pause");
	return 0;
}