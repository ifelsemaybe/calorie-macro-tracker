#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>
#include <iterator>
#include <limits>
#include <cmath>

using namespace std;

class Ingredient {

public:

	int cal;

	double carbs, protein, fat;

	string name, proportion;

	Ingredient();

	Ingredient(string name, int cal, double protein, double carbs, double fat, string proportion);

	Ingredient(const Ingredient& copy);

	friend ostream& operator << (ostream& out, const Ingredient& ing);

};

class Meal {

public:

	string name, proportion; //proportion only in grams

	int cal;

	double carbs, protein, fat;

	vector<Ingredient> ingredientList;

	Meal();

	Meal(string name, vector<Ingredient> ingredientList, string proportion);

	Meal(string name, vector<Ingredient> ingredientList, int cal, double protein, double carbs, double fat, string proportion);

	friend ostream& operator << (ostream& out, const Meal& meal);

};

class Tracker {

public:

	unordered_map<string, Ingredient> allIngredients;

	unordered_map<string, Meal> mealList;

	list<string> text;

	int lineCount;

	void readStats();

	void inputIngredient();

	void inputMeal();

	void debug();

	void track();


	void updateIngredientInMeal(Ingredient &ing, Meal &m, string proportion, bool roundProportion);

};