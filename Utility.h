#pragma once
#include <string>
#include <iostream>

using namespace std;


namespace {

	string myRound(double d, int decimalPlace) {

		decimalPlace = pow(10, decimalPlace);

		d = round(d * decimalPlace) / decimalPlace;

		string d_str = to_string(d);

		d_str = d_str.substr(0, d_str.find_last_not_of('0') + 1);

		if (d_str.find('.') == d_str.size() - 1) {

			d_str = d_str.substr(0, d_str.size() - 1);

		}

		return d_str;

	};

	string displayFood(string name, double cal, double protein, double carbs, double fat, string proportion, bool toConsole = false) {

		if (proportion == "null" || proportion == "n" || proportion == "") {

			if (toConsole) {

				cout << name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";

			}

			return name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";

		}


		if (proportion[0] == 'x') {

			proportion = "x" + myRound(stod(proportion.substr(1)), 1);

		}

		else if (proportion.find("g") != -1) {

			proportion = myRound(stod(proportion.substr(0, proportion.find("g"))), 1) + "g";

		}

		else {

			proportion = myRound(stod(proportion.substr(0, proportion.find("ml"))), 1) + "ml";

		}

		if (toConsole) {

			cout << name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

		}

		return name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

	};


};



