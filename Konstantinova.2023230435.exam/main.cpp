#include <iostream>
#include <vector>
#include <string>
#include "Town.h"
#include "Residential.h"
#include "Character.h"

using namespace std;

int main() {
	cout << "Welcome!" << endl;
	int i = 1;
	while (i != 0) {
		cout << "1.Create new town" << endl;
		cout << "2. Load town" << endl;
		cout << "0. Exit" << endl;
	}
	/*
	Town* t = new Town("t1");
	Residential* r1 = new Residential("12344", 7);
	Character* c1 = new Character("k", "kom", "123", "medic", r1, r1, 22, 12);
	*r1 += *c1;

	int i = 1;
	Town* t = new Town("TownTon");
	while (i != 0) {
		cout << "1.Add building" << endl;
		cout << "2.Add character" << endl;
		cout << "3.Info about the city" << endl;
		cout << "4.Remove character" << endl;
		cout << "5.Remove building" << endl;
		cout << "0. Exit" << endl;
		cin >> i;

		switch (i) 
		{
		case 1: {
			string id;
			int capacity;
			cout << "Enter id and capacity" << endl;
			cin >> id >> capacity;
			t->addBuilding(make_unique<Residential>(id, capacity));
			break;
		} case 2: {
			string name, lastName, charId, occupation;
			int age, salary;
			cout << "Enter name, lastName, id, occupatoin, age, salary" << endl;
			cin >> name >> lastName >> charId >> occupation >> age >> salary;
			t->addCharacter(make_unique<Character>(name, lastName, charId, occupation, age, salary));
			break;
		} case 3:
			t->printData(cout);
			break;
		case 4: {
			string id;
			cout << "Choose character: " << endl;
			cin >> id;
			t->removeCharacter(id);
			break;
		}
		case 5: {
			string id;
			cout << "Choose building: " << endl;
			cin >> id;
			t->removeBuilding(id);
			break;
		}
		default:
			break;
		}
	}

	system("pause");
	return 0;
	*/
}
