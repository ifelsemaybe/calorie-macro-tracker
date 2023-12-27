#include "Meal.h"

Meal::Meal() {

	cal = 0;
	carbs = 0;
	protein = 0;
	fat = 0;

}

Meal::Meal(string name, vector<Ingredient> ingredientList, string proportion) {

	this->name = name;
	this->ingredientList = ingredientList;
	this->proportion = proportion;

}

Meal::Meal(string name, vector<Ingredient> ingredientList, double cal, double protein, double carbs, double fat, string proportion) {

	this->name = name;
	this->ingredientList = ingredientList;
	this->cal = cal;
	this->protein = protein;
	this->carbs = carbs;
	this->fat = fat;
	this->proportion = proportion;

}

ostream& operator << (ostream& out, const Meal& meal) {

	out << displayFood(meal.name, meal.cal, meal.protein, meal.carbs, meal.fat, meal.proportion) + "\n\n";
	out << "List of Ingredients: \n\n\n";

	for (auto& el : meal.ingredientList) {

		out << el << "\n\n";

	}

	return out;

}