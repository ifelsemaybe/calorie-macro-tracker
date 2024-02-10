#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "Ingredient.h"

using namespace std;


class Meal : public Food{

public:

	string varDescription;

	int variation;

	vector<Ingredient> ingredientList;

	vector<Meal> mealList;

	Meal();

	Meal(string name, vector<Ingredient> ingredientList, vector<Meal> mealList, double propValues[]);

	Meal(string name, vector<Ingredient> ingredientList, vector<Meal> mealList, double cal, double protein, double carbs, double fat, int id, int variation, string varDescription, double propValues[]);

	Meal(string name, double cal, double protein, double carbs, double fat, int id, int variation, string varDescription, double propValues[]);

	friend ostream& operator << (ostream& out, const Meal& meal);

};