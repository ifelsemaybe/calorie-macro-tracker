#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "Ingredient.h"
#include "Utility.h";

using namespace std;


class Meal {

public:

	string name, proportion; //proportion only in grams

	double cal;

	double carbs, protein, fat;

	vector<Ingredient> ingredientList;

	Meal();

	Meal(string name, vector<Ingredient> ingredientList, string proportion);

	Meal(string name, vector<Ingredient> ingredientList, double cal, double protein, double carbs, double fat, string proportion);

	friend ostream& operator << (ostream& out, const Meal& meal);

};