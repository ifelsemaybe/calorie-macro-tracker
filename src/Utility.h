#pragma once
#include <string>
#include <iostream>
#include <sqlite3.h>
#include <typeinfo>

#include "Calorie_Macro_Tracker.h"
#include "Food.h"
#include "Ingredient.h"
#include "Meal.h"

using namespace std;

namespace {

	const string ALLDECIMALS = "0123456789.";

	string getTime() {

		time_t t = time(NULL);

        #if defined(_WIN64) || defined(WIN32) 

        struct tm time;

		localtime_s(&time, &t); // for windows use

		return to_string(time.tm_mon + 1) + "/" + to_string(time.tm_mday) + "/" + to_string(1900 + time.tm_year);

        #elif __linux__

        tm* time = localtime(&t);

        return to_string(time->tm_mon + 1) + "/" + to_string(time->tm_mday) + "/" + to_string(1900 + time->tm_year);

        #endif

	}

	void checkForBrand(string& name, string& brand) {

		if (name.find(" -b ") != -1) {

			
			brand = name.substr(name.find(" -b ") + 4);
			name = name.substr(0, name.find(" -b "));

		}

		else {

			brand = "none";
		}

	}

	void checkForProportionValidity(string proportion, string& proportionType, double& proportionValue) { //Gets used after n, "", null are already checked!!!

		if (proportion[0] == 'x') {

			proportionType = "x";
			proportionValue = stod(proportion.substr(1));

		}

		else {

			proportionType = proportion.substr(proportion.find_first_not_of(ALLDECIMALS));


			if (proportionType != "mL" && proportionType != "L" && proportionType != "tsp" && proportionType != "tbsp" && proportionType != "cup" && proportionType != "fl oz" && proportionType != "g" && proportionType != "kg" && proportionType != "oz" && proportionType != "lb") {

				cout << "Sorry, please pick a correct proportion!";

				abort();

			}

			proportionValue = stod(proportion);

		}



	}


	void clampToOneProportion(Food* f, string proportionType) {

		if ((proportionType == "g" || proportionType == "kg" || proportionType == "oz" || proportionType == "lb") && f->propValues[0] != -1) {

			f->propValues[1] = -1;
			f->propValues[2] = -1;

			f->propValues[3] = 0;

		}

		else if ((proportionType == "mL" || proportionType == "L" || proportionType == "tsp" || proportionType == "tbsp" || proportionType == "cup" || proportionType == "fl oz") && f->propValues[1] != -1) {


			f->propValues[0] = -1;
			f->propValues[2] = -1;

			f->propValues[3] = 1;

		}

		else if (proportionType == "x" && f->propValues[2] != -1) {

			f->propValues[0] = -1;
			f->propValues[1] = -1;

			f->propValues[3] = 2;

		}

		else {

			string foodType = (typeid(*f) == typeid(Ingredient)) ? "Ingredient" : "Meal";

			cout << "Sorry, the proportion group type for the " + foodType + " | " + f->name + " | you want to use (weight, volumetric, tally) doesn't match with the ingredient proportion(s) stored in the Database. Your proportion input was: " + proportionType + ".";

			abort();

		}

	}

	void checkIndex(int index, Food& f, double ratio) {

		if (index == -1) {

			for (int x = 0; x < 3; x++) {

				if (f.propValues[x] != -1) {

					f.propValues[x] *= ratio;

				}

			}

		}

		else {

			f.propValues[index] *= ratio;

		}

	}


	double toML(double& propValue, string& propType) {

		if (propType == "L") {

			propValue *= 1000;
		}

		else if (propType == "tsp") {

			propValue *= 4.928922;
		}

		else if (propType == "tbsp") {

			propValue *= 14.786765;
		}

		else if (propType == "cup") {

			propValue *= 236.588236;

		}

		else if (propType == "fl_oz") {

			propValue *= 29.573532;

		}

		propType = "mL";

		return propValue;

	}

	double toG(double& propValue, string& propType) {

		if (propType == "kg") {

			propValue *= 1000;

		}

		else if (propType == "oz") {

			propValue *= 28.34952;

		}

		else if (propType == "lb") {

			propValue *= 453.59232;

		}

		propType = "g";

		return propValue;

	}

	double* findAllWeights(double g) {

		double arr[4];

		arr[0] = g;

		arr[1] = g / 1000; //kg

		arr[2] = g * 0.00220462; //lb

		arr[3] = g / 28.3495; //oz

		return arr;
	}

	double* findAllVolumes(double mL) {

		double arr[6];

		arr[0] = mL;

		arr[1] = mL / 1000; //L

		arr[2] = mL / 4.92892 ; //tsp

		arr[3] = mL / 14.7868; //tbsp

		arr[4] = mL / 236.588; //cup

		arr[5] = mL / 29.5735; //fl_oz

		return arr;

	}

	string myRound(double d, int decimalPlace) {

		decimalPlace = pow(10, decimalPlace);

		d = round(d * decimalPlace) / decimalPlace;

		string d_str = to_string(d);

		d_str = d_str.substr(0, d_str.find_last_not_of('0') + 1);

		if (d_str.find('.') == d_str.size() - 1) {

			d_str = d_str.substr(0, d_str.size() - 1);

		}

		return d_str;

	}

	string displayFood(string name, double cal, double protein, double carbs, double fat, string proportion, bool toConsole = false) { //Dont like how the proportion has to be reassembled from proportion value and proportion type to then be disassembled inside the function... is a little redundant

		string output;

		if (proportion == "null" || proportion == "n" || proportion == "") {

			output = name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";

			if (toConsole) {

				cout << output;

			}

			return output;

		}


		if (proportion[0] == 'x') {

			proportion = "x" + myRound(stod(proportion.substr(1)), 1);

		}

		else {

			string proportionType;
			double proportionValue = 0;

			proportionValue = stod(proportion);
			proportionType = proportion.substr(proportion.find_first_not_of(ALLDECIMALS));

			proportion = myRound(proportionValue, 1) + proportionType;


		}

		output = name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

		if (toConsole) {

			cout << output;

		}

		return output;

	}

	double findRatio(Food* f, string proportionType, double proportionValue) {

		double ratio = 0;
		double numerator, denominator;

		if (proportionType == "x") {

			if (f->propValues[2] == -1) {

				ratio = proportionValue;
			}

			else {

				ratio = proportionValue / f->propValues[2];
			}

		}

		else if ((proportionType == "g" || proportionType == "lb" || proportionType == "kg" || proportionType == "oz") && f->propValues[0] != -1) {


			numerator = proportionType != "g" ? toG(proportionValue, proportionType) : proportionValue;
			denominator = f->propTypes[0] != "g" ? toG(f->propValues[0], f->propTypes[0]) : f->propValues[0];


			ratio = numerator / denominator;

		}

		else if ((proportionType == "mL" || proportionType == "L" || proportionType == "tsp" || proportionType == "tbsp" || proportionType == "cup" || proportionType == "fl_oz") && f->propValues[1] != -1) {


			numerator = proportionType != "mL" ? toML(proportionValue, proportionType) : proportionValue;
			denominator = f->propTypes[1] != "mL" ? toML(f->propValues[1], f->propTypes[1]) : f->propValues[1];

			ratio = numerator / denominator;

		}

		else {

			string foodType = (typeid(*f) == typeid(Ingredient)) ? "Ingredient" : "Meal";

			cout << "Incompatible Proportion Types :/, for the " + foodType + " |" + f->name + " [" + to_string(f->id) + "]|";

			abort();

		}

		return ratio;

	}



	void initializeSqlite3(sqlite3* db, const char* sql, int& rc, char* zErrMsg) {

		if (rc) {

			cout << "Can't open Database [Food Stats.db] :( !!!";

			return;
		}

		else {

			sql = "PRAGMA foreign_keys = ON;";

			rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

			if (rc != SQLITE_OK) {

				cout << "Error: " << zErrMsg << "\n\n";
				sqlite3_free(zErrMsg);

				sqlite3_close(db);
				return;

			}


		}


	}

	void checkErrSqlite3(string marker, char* zErrMsg) {

		cout << "Error - " + marker + ": " << (const char*) zErrMsg;
		sqlite3_free(zErrMsg);

	}


	//CALLBACK FUNCTIONS FOR SQLITE3

	static int callback_print(void* notUsed, int colNbr, char** fields, char** colNames) { //void*= data provided from 4th argument of sqlite3_exec(), colNbr= number of columns, fields= field values for each column as c_string, colNames= the names of each column

		for (int i = 0; i < colNbr; i++) {

			if (!fields[i]) { //If argv[x] is Null, then its a nullpointer!

				cout << colNames[i] << ": NULL\n";
				continue;

			}

			cout << colNames[i] << ": " << fields[i] << "\n";

		}

		return 0;

	}

	//INGREDIENT CALLBACKS

	static int callback_ingID(void* ing, int colNbr, char** fields, char** colNames) {

		string field(fields[0]);

		((Ingredient*)ing)->id = stoi(field);

		return 0;

	}

	static int Icallback(void* tracker, int colNbr, char** fields, char** colNames) {  

		string name, brand;

		int id;

		double cal, protein, carbs, fat;

		double propValues[3];

		for (int i = 0; i < colNbr; i++) {

			string field(fields[i] ? fields[i] : "NULL");

			switch (i) {

			case 0:
				name = field;
				break;

			case 1:
				brand = field;
				break;

			case 2:
				id = stoi(field);
				break;

			case 3:
				cal = stod(field);
				break;

			case 4:
				protein = stod(field);
				break;

			case 5:
				carbs = stod(field);
				break;

			case 6:
				fat = stod(field);
				break;

			case 7:

				propValues[0] = field != "NULL" ? stod(field) : -1; //g
				break;

			case 8:

				propValues[1] = field != "NULL" ? stod(field) : -1; //mL
				break;

			case 9:

				propValues[2] = field != "NULL" ? stod(field) : -1; //x
				break;

			default:
				break;

			}


		}

		((Tracker*)tracker)->allIngredients[id] = Ingredient(name, cal, protein, carbs, fat, brand, id, propValues);

		return 0;

	}


	static int Mcallback_ing(void* tracker, int colNbr, char** fields, char** colNames) {

		string name, varDescription;

		int id, variation;

		double cal, protein, carbs, fat;

		double propValues[3];

		bool& mealIsLogged = ((Tracker*)tracker)->sql_mealIsLogged;
		int& mealID = ((Tracker*)tracker)->sql_mealID;

		string id_test(fields[3]);

		if (mealID != stoi(id_test)) {

			mealIsLogged = false;

		}

		if (!mealIsLogged) {

			for (int i = 0; i < 11; i++) {

				string field(fields[i] ? fields[i] : "NULL");

				switch (i) {

				case 0:
					name = field;
					break;

				case 1:
					variation = stoi(field);
					break;

				case 2:
					varDescription = field;
					break;

				case 3:
					id = stoi(field);
					break;

				case 4:
					cal = stod(field);
					break;

				case 5:
					protein = stod(field);
					break;

				case 6:
					carbs = stod(field);
					break;

				case 7:
					fat = stod(field);
					break;

				case 8:

					propValues[0] = field != "NULL" ? stod(field) : -1; //g
					break;

				case 9:

					propValues[1] = field != "NULL" ? stod(field) : -1; //mL
					break;

				case 10:

					propValues[2] = field != "NULL" ? stod(field) : -1; //x
					break;

				default:
					break;

				}

			}

			((Tracker*)tracker)->allMeals[id] = Meal(name, cal, protein, carbs, fat, id, variation, varDescription, propValues);

			mealID = id;
			mealIsLogged = true;

		}

		string proportionType;

		int ing_id;

		double value;

		for (int i = 11; i < 14; i++) {

			if (!fields[i]) {

				return 0;
			}

			string field(fields[i]);

			switch (i) {

			case 11:
				ing_id = stoi(field);
				break;

			case 12:
				value = stod(field);
				break;

			case 13:
				proportionType = field;
				break;

			default:
				break;

			}

		}

		Ingredient ing = ((Tracker*)tracker)->allIngredients[ing_id];

		Meal& m = ((Tracker*)tracker)->allMeals[mealID];

		((Tracker*)tracker)->updateIngredientInMeal(ing, m, value, proportionType, false);

		m.ingredientList.push_back(ing);

		return 0;

	}

	//Meal CALLBACKS

	static int callback_mealID(void* meal, int colNbr, char** fields, char** colNames) {

		string field(fields[0]);

		((Meal*)meal)->id = stoi(field);

		return 0;

	}

	static int callback_mealID1(void* id, int colNbr, char** fields, char** colNames) {

		string field(fields[0]);

		*((int*)id) = stoi(field);

		return 0;

	}

};


