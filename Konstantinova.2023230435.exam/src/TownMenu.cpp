#include "../include/menu/TownMenu.h"
#include <iostream>
#include <string>
#include "../include/TownException.h"

using namespace std;

void runTownMenu(Town* town) {
	int opt = 1;
	while (opt != 0) {
		try {
			cout << "\n" << endl;
			cout << "1. Change name" << endl;
			cout << "2. Print data" << endl;
			cout << "3. Print map" << endl;
			cout << "4. Add station" << endl;
			cout << "5. Remove station" << endl;
			cout << "6. Connect stations" << endl;
			cout << "7. Find route" << endl;
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
				string newName;
				cout << "New name: ";
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				getline(cin, newName);
				town->setName(newName);
				break;
			} case 2: {
				town->printData(cout);
				break;
			} case 3: {
				town->printMap(cout);
				break;
			} case 4: {
				string stationName;
				cout << "Enter satation name: ";
				cin.ignore();
				getline(cin, stationName);
				town->addStation(make_unique<Station>(stationName));
				break;
			} case 5: {
				string stationName;
				cout << "Enter satation name: " << endl;
				cin.ignore();
				getline(cin, stationName);
				town->removeStation(stationName);
				break;
			}case 6: {
				string start, end;
				int dist;
				cout << "Enter first station name: ";
				cin.ignore();
				getline(cin, start);
				cout << "Enter second station name: ";
				getline(cin, end);
				cout << "Enter distance: ";
				cin >> dist;
				town->connectStations(start, end, dist);
				break;
			} case 7: {
				string start, end;
				cout << "Enter first station name: ";
				cin >> start;
				cout << "Enter second station name: ";
				cin >> end;
				cout << "Counting" << endl;
				break;
			} case 0: {
				break;
			} default: {
				cout << "Invalid option" << endl;
			}
			}
		}
		catch (const TownException& e) {
			cout << "Error: " << e.what() << endl;
			opt = 1;
		}
	}
}
