#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
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
namespace fs = std::filesystem;


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

vector<string> getSavedTowns() {
	vector<string> towns;

	if (!fs::exists("saves")) {
		return towns;
	}

	for (const auto& entry : fs::directory_iterator("saves")) {
		if (entry.path().extension() == ".json") {
			towns.push_back(entry.path().stem().string());
		}
	}

	return towns;
}

void loadTown() {
	vector<string> savedTowns = getSavedTowns();
	cout << endl;
	for (int i = 0; i < savedTowns.size(); ++i) {
		cout << i << ". " << savedTowns[i] << endl;
	}

	int townId;
	cout << "Enter town id to load or -1 to exit: ";
	cin >> townId;
	if (townId == -1) { return; }
	if (townId < 0 || townId >= savedTowns.size()) {
		throw TownException("Invalid town id");
	}

	string fName = "saves/" + savedTowns[townId] + ".json";
	Town town = Town::loadTown(fName);

	runMainMenu(&town);
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
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					getline(cin, townName);
					string newTownFile = "saves/" + townName + ".json";
					if (fs::exists(newTownFile)) {
						throw TownException("Town with this name already exists");
					}
					else {
						auto town = make_unique<Town>(townName);
						runMainMenu(town.get()); //check for nullptr
					}
					break;
				} case 2: {
					loadTown();
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