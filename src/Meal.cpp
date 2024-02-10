#include "Meal.h"
#include "Utility.h";


Meal::Meal() {

	cal = 0;
	carbs = 0;
	protein = 0;
	fat = 0;
	id = -1;
	variation = -1;

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";

	this->propValues[0] = -1;
	this->propValues[1] = -1;
	this->propValues[2] = -1;
	this->propValues[3] = -1;

}

Meal::Meal(string name, vector<Ingredient> ingredientList, vector<Meal> mealList, double propValues[]) {

	this->name = name;
	this->ingredientList = ingredientList;
	this->mealList = mealList;
	this->propValues[0] = propValues[0];
	this->propValues[1] = propValues[1];
	this->propValues[2] = propValues[2];
	this->propValues[3] = -1;

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";

}

Meal::Meal(string name, vector<Ingredient> ingredientList, vector<Meal> mealList, double cal, double protein, double carbs, double fat, int id, int variation, string varDescription, double propValues[]) {

	this->id = id;
	this->variation = variation;
	this->name = name;
	this->ingredientList = ingredientList;
	this->mealList = mealList;
	this->cal = cal;
	this->protein = protein;
	this->carbs = carbs;
	this->fat = fat;
	this->propValues[0] = propValues[0];
	this->propValues[1] = propValues[1];
	this->propValues[2] = propValues[2];
	this->propValues[3] = -1;

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";



}

Meal::Meal(string name, double cal, double protein, double carbs, double fat, int id, int variation, string varDescription, double propValues[]) {

	this->id = id;
	this->variation = variation;
	this->name = name;
	this->cal = cal;
	this->protein = protein;
	this->carbs = carbs;
	this->fat = fat;
	this->propValues[0] = propValues[0];
	this->propValues[1] = propValues[1];
	this->propValues[2] = propValues[2];
	this->propValues[3] = -1;

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";



}

ostream& operator << (ostream& out, const Meal& meal) {

	string proportion;
	int index = -1;

	if (meal.propValues[3] == -1) {

		for (int x = 0; x < 3; x++) {

			if (meal.propValues[x] != -1) {

				index = x;
				break;

			}

		}

	}

	else {

		index = meal.propValues[3];

	}

	if (index == 2) {

		proportion = "x" + to_string(meal.propValues[2]);

	}

	else {


		proportion = (index == -1) ? "" : to_string(meal.propValues[index]) + meal.propTypes[index];


	}


	out << displayFood(meal.name, meal.cal, meal.protein, meal.carbs, meal.fat, proportion)+ "\n\n";
	out << "List of Ingredients: \n\n\n";

	for (auto& el : meal.ingredientList) {

		out << el << "\n\n";

	}

	return out;

}