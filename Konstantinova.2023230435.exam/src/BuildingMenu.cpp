#include "../include/menu/BuildingMenu.h"
#include <iostream>
#include <string>
#include "../include/TownException.h"

using namespace std;

void runBuildingMenu(Town* town) {
	int opt = 1;

	while (opt != 0) {
		try {
			cout << "\n" << endl;
			cout << "1. Print buildings" << endl;
			cout << "2. Add building" << endl;
			cout << "3. Remove building" << endl;
			cout << "4. Move building to another station" << endl;
			cout << "5. Print characters in the building" << endl;
			cout << "0. Exit" << endl;
			cout << "Enter option: ";
			cin >> opt;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(20, '\n');
				throw TownException("Invalid option");
			}
				switch (opt) {
				case 1: {
					town->printBuildings(cout);
					break;
				} case 2: {
					if (town->getStations().size() == 0) {
						cout << "Add at least one station before adding any buildings" << endl;
						break;
					}
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					string station;
					cout << "Enter station name: ";
					getline(cin, station, '\n');
					string name, type;
					cout << "Enter building name: ";

					getline(cin, name, '\n');
					cout << "Choose building category:" << endl;
					cout << "1. Residential" << endl;
					cout << "2. Facility" << endl;
					cout << "Enter option: ";
					int buildingOpt;
					cin >> buildingOpt;
					if (cin.fail()) {
						cin.clear();
						cin.ignore(20, '\n');
						throw TownException("Invalid option");
						buildingOpt = 1;
					}
					switch (buildingOpt) {
					case 1: {
						int capacity;
						cout << "Enter capacity: ";
						cin >> capacity;
						town->addBuilding(make_unique<Residential>(capacity, name), station);
						break;
					} case 2: {
						string type;
						cout << "Enter facility type: ";
						cin >> type;
						town->addBuilding(make_unique<Facility>(type, name), station);
						break;
					} default: {
						cout << "Invalid option" << endl;
						break;
					}
					}
					break;
				} case 3: {
					string buildingId;
					cout << "Enter building id: ";
					cin >> buildingId;
					town->removeBuilding(buildingId);
					break;
				} case 4: {
					string buildingId;
					cout << "Enter building id: ";
					cin >> buildingId;
					string stationName;
					cout << "Enter new station: ";
					cin >> stationName;
					town->moveBuilding(buildingId, stationName);
					break;
				} case 5: {
					string buildingId;
					cout << "Enter building id: ";
					cin >> buildingId;
					town->printBuildingCharacters(cout, buildingId);
					break;
				} case 0: {
					break;
				} default: {
					cout << "Invalid option" << endl;
					break;
				}
				}
		} catch (const TownException& e) {
			cout << "Error: " << e.what() << endl;
			opt = 1;
		}
	}
	
}
